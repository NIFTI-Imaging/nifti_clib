
include_directories(${NIFTI_SOURCE_DIR}/niftilib ${NIFTI_SOURCE_DIR}/znzlib
  ${NIFTI_SOURCE_DIR}/nifticdf ${NIFTI_SOURCE_DIR}/utils)

add_executable(nifti_first_test_program nifti_test.c)
target_link_libraries(nifti_first_test_program ${NIFTI_PACKAGE_PREFIX}niftiio )

add_executable(nifti_second_test_program nifti_test2.c)
target_link_libraries(nifti_second_test_program ${NIFTI_PACKAGE_PREFIX}niftiio )


add_test(nifti_first_test nifti_first_test_program )
add_test(nifti_second_test nifti_second_test_program )

