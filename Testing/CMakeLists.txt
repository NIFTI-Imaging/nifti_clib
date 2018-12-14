
include_directories(${NIFTI_SOURCE_DIR}/niftilib ${NIFTI_SOURCE_DIR}/znzlib ${NIFTI_SOURCE_DIR}/nifticdf)
add_subdirectory(niftilib)
add_subdirectory(nifti_regress_test)
