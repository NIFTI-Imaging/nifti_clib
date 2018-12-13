set(NIFTI_NIFTI2LIB_NAME ${PACKAGE_PREFIX}nifti2io)

add_library(${NIFTI_NIFTI2LIB_NAME} nifti2_io.c)
target_include_directories(${PACKAGE_PREFIX}nifti2io PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${NIFTI_INSTALL_INCLUDE_DIR}>
        )

if(BUILD_SHARED_LIBS)
  set_target_properties(${NIFTI_NIFTI2LIB_NAME} PROPERTIES ${NIFTI_LIBRARY_PROPERTIES})
endif()
target_link_libraries(${NIFTI_NIFTI2LIB_NAME} niftiio)

# Nifti1 header required:
target_link_libraries(${PACKAGE_PREFIX}nifti2io ${PACKAGE_PREFIX}niftiio)

# Exporting and installing
if(NOT NIFTI_INSTALL_NO_LIBRARIES)
  install(TARGETS ${NIFTI_NIFTI2LIB_NAME}
          EXPORT NIFTITargets
    RUNTIME DESTINATION ${NIFTI_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${NIFTI_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${NIFTI_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT NIFTI_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files}
    DESTINATION ${NIFTI_INSTALL_INCLUDE_DIR}
    COMPONENT Development)
endif()
