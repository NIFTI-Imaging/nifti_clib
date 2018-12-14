set(NIFTI_NIFTILIB2_NAME ${NIFTI_PACKAGE_PREFIX}nifti2)
set(NIFTILIB2_SRC nifti2_io.c)

add_library(${NIFTI_NIFTILIB2_NAME} ${NIFTILIB2_SRC} )
target_link_libraries( ${NIFTI_NIFTILIB2_NAME} PUBLIC ${NIFTI_PACKAGE_PREFIX}niftiio)
target_include_directories(${NIFTI_NIFTILIB2_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${NIFTI_INSTALL_INCLUDE_DIR}>
        )

if(NIFTI_BUILD_APPLICATIONS)
    add_executable(${NIFTI_PACKAGE_PREFIX}clib_02_nifti2 clib_02_nifti2.c)
    target_link_libraries(${NIFTI_PACKAGE_PREFIX}clib_02_nifti2 PUBLIC ${NIFTI_NIFTILIB2_NAME})
    add_executable(${NIFTI_PACKAGE_PREFIX}nifti_tool nifti_tool.c)
    target_link_libraries(${NIFTI_PACKAGE_PREFIX}nifti_tool PUBLIC ${NIFTI_NIFTILIB2_NAME})
    set(NIFTI2_APPS ${NIFTI_PACKAGE_PREFIX}clib_02_nifti2 ${NIFTI_PACKAGE_PREFIX}nifti_tool)
endif()

# Set lib version when buildung shared libs.
if(BUILD_SHARED_LIBS)
    set_target_properties(${NIFTI_NIFTILIB2_NAME} PROPERTIES ${NIFTI_LIBRARY_PROPERTIES})
endif()

if(NOT NIFTI_INSTALL_NO_LIBRARIES)
    install(TARGETS ${NIFTI_NIFTILIB2_NAME} ${NIFTI2_APPS}
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