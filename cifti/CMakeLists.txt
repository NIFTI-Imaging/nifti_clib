include(FindEXPAT)
include_directories(${NIFTI_SOURCE_DIR}/nifti2)
add_executable(${PACKAGE_PREFIX}cifti_tool cifti_tool.c afni_xml.c afni_xml_io.c)
target_include_directories(${PACKAGE_PREFIX}cifti_tool PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${NIFTI_INSTALL_INCLUDE_DIR}>
        )
target_link_libraries(${PACKAGE_PREFIX}cifti_tool ${PACKAGE_PREFIX}nifti2io EXPAT::EXPAT)


# Exporting and installing
if(NOT NIFTI_INSTALL_NO_LIBRARIES)
  install(TARGETS ${PACKAGE_PREFIX}cifti_tool
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
