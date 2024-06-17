/** \file fslio.h
    \brief Data structures for using the fslio API.  Written by Mark Jenkinson, FMRIB.
 */

/*
    fslio.h  (Input and output routines for images in FSL)

    Mark Jenkinson
    FMRIB Image Analysis Group

*/


/*
    The fslio.h file was originally part of FSL - FMRIB's Software Library
    http://www.fmrib.ox.ac.uk/fsl
    fslio.h has now been placed in the public domain.

    Developed at FMRIB (Oxford Centre for Functional Magnetic Resonance
    Imaging of the Brain), Department of Clinical Neurology, Oxford
    University, Oxford, UK
*/

#if !defined(__FSLIO_H)
#define __FSLIO_H

#include <stdio.h>
#include "nifti1_io.h"
#include "znzlib.h"
#include "dbh.h"

#ifndef FSL_API
  #if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(NIFTI_FSL_BUILD_SHARED)
      #ifdef __GNUC__
        #define FSL_API __attribute__ ((dllexport))
      #else
        #define FSL_API __declspec( dllexport )
      #endif
    #elif defined(NIFTI_FSL_USE_SHARED)
      #ifdef __GNUC__
        #define FSL_API __attribute__ ((dllimport))
      #else
        #define FSL_API __declspec( dllimport )
      #endif
    #else
      #define FSL_API
    #endif
  #elif (defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__)
    #define FSL_API __attribute__ ((visibility ("default")))
  #else
    #define FSL_API
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


  /*
    Note that this library is similar to avwio but has changed in many ways.
    It is almost fully backwards compatible, but not quite, as it cannot write
    .nii.gz files using the old style functions.

    Recommended ways of reading and writing images are:

    Reading
    -------
      Use the FslOpen(), FslReadVolumes() and FslClose() functions.  e.g.
        FSLIO *fslio;
        void *buffer;
        int nvols;
        fslio = FslOpen("/some/path/name_of_file","rb");
          ... can now access header info via the FslGet calls ...
          ... allocate room for buffer ...
        FslReadVolumes(fslio,buffer,nvols);
          ... do something ...
        FslClose(fslio);


    Writing
    -------
      This is more complicated due to the nature of gzipped writing, which must be
      done in the correct order, and for single files (*.nii.gz) this means that
      the header information must be written before any image data.

    (1)
      The best method to use is almost backwards compatible, but requires
      an FslWriteHeader() call:

        FSLIO* fslio;
        fslio = FslOpen("/some/path/name_of_file","wb");
          ... set the appropriate header information using FslSet calls ...
        FslWriteHeader(fslio);

          ... now can write one or more volumes at a time using
              FslWriteVolumes(fslio,buffer,nvols) ...

        FslClose(fslio);

      This version is useful if your image data needs to be written from different blocks
      of memory.

    (2)
      Another method is available, but which is discouraged, is:
        FSLIO* fslio;
        fslio = FslOpen("/some/path/name_of_file","wb");

          ... set some appropriate header information using FslSet calls ...
          ... now can write one or more volumes at a time using
              FslWriteVolumes(fslio,buffer,nvols) ...
          ... set more appropriate header information using FslSet calls ...

        FslClose(fslio);

      WARNING: this cannot write .nii.gz files as the header information cannot be
      written by FslClose() after the image data is written, which is how the previous
      versions have worked.


   */

/*! \defgroup FSL_TYPE
    \brief FSL data format type codes
    @{
 */
#define FSL_TYPE_ANALYZE         0
#define FSL_TYPE_NIFTI           1
#define FSL_TYPE_NIFTI_PAIR      2
#define FSL_TYPE_MINC            4
#define FSL_TYPE_ANALYZE_GZ    100
#define FSL_TYPE_NIFTI_GZ      101
#define FSL_TYPE_NIFTI_PAIR_GZ 102
#define FSL_TYPE_MINC_GZ       104
/* @} */

#define FSL_RADIOLOGICAL        -1
#define FSL_NEUROLOGICAL         1


/*! \struct FSLIO
    \brief High level data structure for open datasets in the fslio API.
    \sa nifti_image
    \sa minc_image
 */
typedef struct
{
  znzFile fileptr;
  nifti_image *niftiptr;
#ifdef USE_MINC
  minc_image *mincptr;
#else
  void *mincptr;
#endif
  int file_mode;
  int write_mode;
  int written_hdr;
} FSLIO;


  /* basic file i/o commands */

FSL_API FSLIO *FslOpen(const char *filename, const char *opts);
FSL_API FSLIO *FslXOpen(const char *filename, const char *opts, int filetype);
FSL_API int FslSeekVolume(FSLIO *fslio, size_t vols);
FSL_API int FslClose(FSLIO *fslio);

  /* basic read and write commands */

FSL_API void* FslReadAllVolumes(FSLIO* fslio, char* filename);
FSL_API void  FslWriteAllVolumes(FSLIO *fslio, const void *buffer);

FSL_API size_t FslReadVolumes(FSLIO *fslio, void *buffer, size_t nvols);
FSL_API size_t FslWriteVolumes(FSLIO *fslio, const void *buffer, size_t nvols);

FSL_API void   FslWriteHeader(FSLIO *fslio);

  /* support functions for file names and types */

FSL_API int   FslFileExists(const char *filename);
FSL_API char *FslMakeBaseName(const char *fname);
FSL_API int   FslCheckForMultipleFileNames(const char* filename);
FSL_API int   FslGetEnvOutputType(void);

FSL_API void  FslSetIgnoreMFQ(int flag);
FSL_API int   FslGetIgnoreMFQ(void);
FSL_API void  FslSetOverrideOutputType(int type);
FSL_API int   FslGetOverrideOutputType(void);


FSL_API int  FslGetFileType(const FSLIO *fslio);
FSL_API void FslSetFileType(FSLIO *fslio, int filetype);
FSL_API int  FslIsSingleFileType(int filetype);
FSL_API int  FslIsCompressedFileType(int filetype);
FSL_API int  FslBaseFileType(int filetype);
FSL_API const char* FslFileTypeString(int filetype);

FSL_API int  FslGetWriteMode(const FSLIO *fslio);
FSL_API void FslSetWriteMode(FSLIO *fslio, int mode);

FSL_API void AvwSwapHeader(struct dsr *avw);
FSL_API int  FslReadRawHeader(void *buffer, const char* filename);


  /* simple creation and clone/copy operations */

FSL_API FSLIO *FslInit(void);
FSL_API void   FslInitHeader(FSLIO *fslio, short t,
                   size_t x, size_t y, size_t z, size_t v,
                   float vx, float vy, float vz, float tr,
                   size_t dim);
FSL_API void   FslSetInit(FSLIO* fslio);
FSL_API void   FslCloneHeader(FSLIO *dest, const FSLIO *src);


  /* get and set routines for properties */

FSL_API size_t FslGetVolSize(FSLIO *fslio);

FSL_API void FslSetDim(FSLIO *fslio, short x, short y, short z, short v);
FSL_API void FslGetDim(FSLIO *fslio, short *x, short *y, short *z, short *v);
FSL_API void FslSetDimensionality(FSLIO *fslio, size_t dim);
FSL_API void FslGetDimensionality(FSLIO *fslio, size_t *dim);
FSL_API void FslSetVoxDim(FSLIO *fslio, float x, float y, float z, float tr);
FSL_API void FslGetVoxDim(FSLIO *fslio, float *x, float *y, float *z, float *tr);
FSL_API void FslGetCalMinMax(FSLIO *fslio, float *min, float *max);
FSL_API void FslSetCalMinMax(FSLIO *fslio, float  min, float  max);
FSL_API void FslGetAuxFile(FSLIO *fslio,char *aux_file);
FSL_API void FslSetAuxFile(FSLIO *fslio,const char *aux_file);
FSL_API void FslSetTimeUnits(FSLIO *fslio, const char *units);
FSL_API void FslGetTimeUnits(FSLIO *fslio, char *units);
FSL_API void FslSetDataType(FSLIO *fslio, short t);
FSL_API size_t FslGetDataType(FSLIO *fslio, short *t);
FSL_API int    FslGetIntensityScaling(FSLIO *fslio, float *slope, float *intercept);
FSL_API void   FslSetIntent(FSLIO *fslio, short intent_code, float p1, float p2, float p3);
FSL_API short  FslGetIntent(FSLIO *fslio, short *intent_code, float *p1, float *p2,
                            float *p3);


FSL_API short FslGetStdXform(FSLIO *fslio, mat44 *stdmat);
FSL_API void  FslSetStdXform(FSLIO *fslio, short sform_code, mat44 stdmat);
FSL_API void  FslGetMMCoord(mat44 stdmat, float voxx, float voxy, float voxz,
                            float *mmx, float *mmy, float *mmz);

FSL_API void  FslGetVoxCoord(mat44 stdmat, float mmx, float mmy, float mmz,
                             float *voxx, float *voxy, float *voxz);
FSL_API short FslGetRigidXform(FSLIO *fslio, mat44 *rigidmat);
FSL_API void  FslSetRigidXform(FSLIO *fslio, short qform_code, mat44 rigidmat);
FSL_API int   FslGetLeftRightOrder(FSLIO *fslio);

  /* these two functions are deprecated with the nifti/analyze support */
  /* please do all spatial coordinate origins via the Std and Rigid Xforms */
FSL_API void  FslSetAnalyzeSform(FSLIO *fslio, const short *orig,
                                 float dx, float dy, float dz);
FSL_API void  FslGetAnalyzeOrigin(FSLIO *fslio, short orig[5]);

  /* other read and write commands */

FSL_API size_t FslReadSliceSeries(FSLIO *fslio, void *buffer,short slice, size_t nvols);
FSL_API size_t FslReadRowSeries(FSLIO *fslio, void *buffer, short row, short slice, size_t nvols);
FSL_API size_t FslReadTimeSeries(FSLIO *fslio, void *buffer, short xVox, short yVox, short zVox, size_t nvols);

  /* miscellaneous helper stuff */

FSL_API mat33 mat44_to_mat33(mat44 x);



/* added by KF pending discussion w/ Mark */
typedef unsigned char   THIS_UINT8;
typedef char            THIS_INT8;
typedef unsigned short  THIS_UINT16;
typedef short           THIS_INT16;
typedef unsigned int    THIS_UINT32;
typedef int             THIS_INT32;
typedef unsigned long   THIS_UINT64;
typedef long            THIS_INT64;
typedef float           THIS_FLOAT32;
typedef double          THIS_FLOAT64;

FSL_API FSLIO * FslReadHeader(char *fname);
FSL_API double ****FslGetBufferAsScaledDouble(FSLIO *fslio);
FSL_API double ***FslGetVolumeAsScaledDouble(FSLIO *fslio, int vol);
FSL_API int  convertBufferToScaledDouble(double *outbuf, void *inbuf, long len, float slope, float inter, int nifti_datatype ) ;
FSL_API double ****d4matrix(int th, int zh,  int yh, int xh);
FSL_API double ***d3matrix(int zh,  int yh, int xh);


#ifdef __cplusplus
}
#endif

#endif
