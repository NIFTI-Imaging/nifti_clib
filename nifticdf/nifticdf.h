
 /************************************************************************/
 /**  Functions to compute cumulative distributions and their inverses  **/
 /**  for the NIfTI-1 statistical types.  Much of this code is taken    **/
 /**  from other sources.  In particular, the cdflib functions by       **/
 /**  Brown and Lovato make up the bulk of this file.  That code        **/
 /**  was placed in the public domain.  The code by K. Krishnamoorthy   **/
 /**  is also released for unrestricted use.  Finally, the other parts  **/
 /**  of this file (by RW Cox) are released to the public domain.       **/
 /**                                                                    **/
 /**  Most of this file comprises a set of "static" functions, to be    **/
 /**  called by the user-level functions at the very end of the file.   **/
 /**  At the end of the file is a simple main program to drive these    **/
 /**  functions.                                                        **/
 /**                                                                    **/
 /**  To find the user-level functions, search forward for the string   **/
 /**  "nifti_", which will be at about line 11000.                      **/
 /************************************************************************/
 /*****==============================================================*****/
 /***** Neither the National Institutes of Health (NIH), the DFWG,   *****/
 /***** nor any of the members or employees of these institutions    *****/
 /***** imply any warranty of usefulness of this material for any    *****/
 /***** purpose, and do not assume any liability for damages,        *****/
 /***** incidental or otherwise, caused by any use of this document. *****/
 /***** If these conditions are not acceptable, do not use this!     *****/
 /*****==============================================================*****/
 /************************************************************************/

#include <stdlib.h>

/****************************************************************************
 Statistical codes implemented :

     NIFTI_INTENT_CORREL     = correlation statistic
     NIFTI_INTENT_TTEST      = t statistic (central)
     NIFTI_INTENT_FTEST      = F statistic (central)
     NIFTI_INTENT_ZSCORE     = N(0,1) statistic
     NIFTI_INTENT_CHISQ      = Chi-squared (central)
     NIFTI_INTENT_BETA       = Beta variable (central)
     NIFTI_INTENT_BINOM      = Binomial variable
     NIFTI_INTENT_GAMMA      = Gamma distribution
     NIFTI_INTENT_POISSON    = Poisson distribution
     NIFTI_INTENT_FTEST_NONC = noncentral F statistic
     NIFTI_INTENT_CHISQ_NONC = noncentral chi-squared
     NIFTI_INTENT_TTEST_NONC = noncentral t statistic
     NIFTI_INTENT_CHI        = Chi statistic (central)
     NIFTI_INTENT_INVGAUSS   = inverse Gaussian variable
     NIFTI_INTENT_WEIBULL    = Weibull distribution
     NIFTI_INTENT_EXTVAL     = Extreme value type I
     NIFTI_INTENT_NORMAL     = N(mu,variance) normal
     NIFTI_INTENT_LOGISTIC   = Logistic distribution
     NIFTI_INTENT_LAPLACE    = Laplace distribution
     NIFTI_INTENT_UNIFORM    = Uniform distribution
     NIFTI_INTENT_PVAL       = "p-value"
     NIFTI_INTENT_LOGPVAL    = -ln(p)
     NIFTI_INTENT_LOG10PVAL  = -log10(p)
*****************************************************************************/

#ifndef NCDF_API
  #if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(NIFTICDF_BUILD_SHARED)
      #ifdef __GNUC__
        #define NCDF_API __attribute__ ((dllexport))
      #else
        #define NCDF_API __declspec( dllexport )
      #endif
    #elif defined(NIFTICDF_USE_SHARED)
      #ifdef __GNUC__
        #define NCDF_API __attribute__ ((dllimport))
      #else
        #define NCDF_API __declspec( dllimport )
      #endif
    #else
      #define NCDF_API
    #endif
  #elif (defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__)
    #define NCDF_API __attribute__ ((visibility ("default")))
  #else
    #define NCDF_API
  #endif
#endif

NCDF_API extern char const * const inam[];

NCDF_API int nifti_intent_code( char *name );
NCDF_API double nifti_stat2cdf( double val, int code, double p1,double p2,double p3 );
NCDF_API double nifti_stat2rcdf( double val, int code, double p1,double p2,double p3 );
NCDF_API double nifti_cdf2stat( double p , int code, double p1,double p2,double p3 );
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API double nifti_rcdf2stat( double q , int code, double p1,double p2,double p3 );
#endif/*(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API double nifti_stat2zscore( double val , int code, double p1,double p2,double p3);
NCDF_API double nifti_stat2hzscore( double val, int code, double p1,double p2,double p3);

/** Prototypes for cdflib functions **/

NCDF_API double algdiv(const double*,const double*);
NCDF_API double alngam(const double*);
NCDF_API double alnrel(const double*);
NCDF_API double apser(const double*,double*,const double*,const double*);
NCDF_API double basym(double*,double*,const double*,const double*);
NCDF_API double bcorr(const double*,const double*);
NCDF_API double betaln(const double*,const double*);
NCDF_API double bfrac(double*,double*,double*,double*,const double*,const double*);
NCDF_API void bgrat(double*,double*,const double*,const double*,double*,double*,int*ierr);
NCDF_API double bpser(double*,double*,const double*,const double*);
NCDF_API void bratio(const double*,const double*,const double*,const double*,double*,double*,int*);
NCDF_API double brcmp1(int*,double*,double*,const double*,const double*);
NCDF_API double brcomp(double*,double*,const double*,const double*);
NCDF_API double bup(double*,double*,double*,double*,const int*,const double*);
NCDF_API void cdfbet(const int*,double*,double*,double*,double*,double*,double*,
                            int*,double*);
NCDF_API void cdfbin(const int*,double*,double*,double*,double*,double*,double*,
                            int*,double*);
NCDF_API void cdfchi(const int*,double*,double*,double*,double*,int*,double*);
NCDF_API void cdfchn(const int*,double*,double*,double*,double*,double*,int*,double*);
NCDF_API void cdff(const int*,double*,double*,double*,double*,double*,int*,double*);
NCDF_API void cdffnc(const int*,double*,double*,double*,double*,double*,double*,
                            int*status,double*);
NCDF_API void cdfgam(const int*,double*,double*,double*,double*,double*,int*,double*);
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API void cdfnbn(int*,double*,double*,double*,double*,double*,double*,
NCDF_API                    int*,double*);
NCDF_API void cdfnor(int*,double*,double*,double*,double*,double*,int*,double*);
#endif /*defined(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API void cdfpoi(const int*,double*,double*,double*,double*,int*,double*);
NCDF_API void cdft(const int*,double*,double*,double*,double*,int*,double*);
NCDF_API void cumbet(double*,double*,double*,double*,double*,double*);
NCDF_API void cumbin(const double*,const double*,double*,double*,double*,double*);
NCDF_API void cumchi(const double*,const double*,double*,double*);
NCDF_API void cumchn(double*,double*,const double*,double*,double*);
NCDF_API void cumf(const double*,const double*,const double*,double*,double*);
NCDF_API void cumfnc(double*,double*,double*,const double*,double*,double*);
NCDF_API void cumgam(double*,double*,double*,double*);
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API void cumnbn(double*,double*,double*,double*,double*,double*);
#endif /*defined(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API void cumnor(const double*,double*,double*);
NCDF_API void cumpoi(const double*,const double*,double*,double*);
NCDF_API void cumt(const double*,const double*,double*,double*);
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API double dbetrm(double*,double*);
#endif /*defined(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API double devlpl(const double [],const int*,const double*);
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API double dexpm1(double*);
NCDF_API double dinvnr(const double *p,const double *q);
#endif /*defined(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API void E0000(int,int*,double*,double*,unsigned long*,
                           unsigned long*,const double*,const double*,const double*,
                           const double*,const double*,const double*,const double*);
NCDF_API void dinvr(int*,double*,double*,unsigned long*,unsigned long*);
NCDF_API void dstinv(double*,double*,double*,double*,double*,double*,
                             double*);
#if defined(__COMPILE_UNUSED_FUNCTIONS__)
NCDF_API double dlanor(double*);
NCDF_API double dln1mx(double*);
NCDF_API double dln1px(double*);
NCDF_API double dlnbet(double*,double*);
NCDF_API double dlngam(double*);
NCDF_API double dstrem(double*);
#endif /*defined(__COMPILE_UNUSED_FUNCTIONS__)*/
NCDF_API double dt1(double*,double*,const double*);
NCDF_API void E0001(int,int*,double*,const double*,double*,double*,
                           unsigned long*,unsigned long*,const double*,const double*,
                           const double*,const double*);
NCDF_API void dzror(int*,double*,double*,double*,double *,
                           unsigned long*,unsigned long*);
NCDF_API void dstzr(double *zxlo,double *zxhi,double *zabstl,double *zreltl);
NCDF_API double erf1(const double*);
NCDF_API double erfc1(const int*,const double*);
NCDF_API double esum(const int*,const double*);
NCDF_API double exparg(const int*);
NCDF_API double fpser(const double*,const double*,const double*,const double*);
NCDF_API double gam1(const double*);
NCDF_API void gaminv(double*,double*,const double*,const double*,const double*,int*);
NCDF_API double gamln(double*);
NCDF_API double gamln1(const double*);
NCDF_API double Xgamm(const double*);
NCDF_API void grat1(double*,const double*,const double*,double*,double*,const double*);
NCDF_API void gratio(double*,const double*,double*,double*,const int*);
NCDF_API double gsumln(const double*,const double*);
NCDF_API double psi(const double*);
NCDF_API double rcomp(double*,const double*);
NCDF_API double rexp(const double*);
NCDF_API double rlog(const double*);
NCDF_API double rlog1(const double*);
NCDF_API double spmpar(const int*);
NCDF_API double stvaln(const double*);
NCDF_API double fifdint(double);
NCDF_API double fifdmax1(double,double);
NCDF_API double fifdmin1(double,double);
NCDF_API double fifdsign(double,double);
NCDF_API long fifidint(double);
NCDF_API long fifmod(long,long);
NCDF_API void ftnstop(const char*);
NCDF_API int ipmpar(const int*);

/** end: prototypes for cdflib functions **/
