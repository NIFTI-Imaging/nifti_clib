
include_directories(${NIFTI_SOURCE_DIR}/niftilib ${NIFTI_SOURCE_DIR}/znzlib ${NIFTI_SOURCE_DIR}/nifticdf)

add_executable(  ${NIFTI_PACKAGE_PREFIX}fsl_api_driver fsl_api_driver.c )
target_link_libraries( fsl_api_driver PUBLIC ${NIFTI_PACKAGE_PREFIX}fslio)

if(NOT NIFTI_INSTALL_NO_LIBRARIES)
  install(TARGETS ${NIFTI_PACKAGE_PREFIX}fsl_api_driver
          EXPORT NIFTITargets
    RUNTIME DESTINATION ${NIFTI_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${NIFTI_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${NIFTI_INSTALL_LIB_DIR} COMPONENT Development)
endif()
