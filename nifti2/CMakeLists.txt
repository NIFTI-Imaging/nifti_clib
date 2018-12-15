set(NIFTI_NIFTILIB2_NAME ${NIFTI_PACKAGE_PREFIX}nifti2)
set(NIFTILIB2_SRC nifti2_io.c)

add_library(${NIFTI_NIFTILIB2_NAME} ${NIFTILIB2_SRC} )
target_link_libraries( ${NIFTI_NIFTILIB2_NAME} PUBLIC ${NIFTI_PACKAGE_PREFIX}niftiio)
target_include_directories(${NIFTI_NIFTILIB2_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${NIFTI_INSTALL_INCLUDE_DIR}>
        )

if(NIFTI_BUILD_APPLICATIONS)
    add_executable(${NIFTI_PACKAGE_PREFIX}nifti_tool nifti_tool.c)
    target_link_libraries(${NIFTI_PACKAGE_PREFIX}nifti_tool PUBLIC ${NIFTI_NIFTILIB2_NAME})
    set(NIFTI2_APPS ${NIFTI_PACKAGE_PREFIX}nifti_tool)
endif()

# Set lib version when buildung shared libs.
if(BUILD_SHARED_LIBS)
    set_target_properties(${NIFTI_NIFTILIB2_NAME} PROPERTIES ${NIFTI_LIBRARY_PROPERTIES})
endif()

if(NOT NIFTI_INSTALL_NO_LIBRARIES)
    install(TARGETS ${NIFTI_NIFTILIB2_NAME} ${NIFTI2_APPS}
            EXPORT NIFTITargets
            RUNTIME DESTINATION ${NIFTI_INSTALL_RUNTIME_DIR} COMPONENT RuntimeLibraries
            LIBRARY DESTINATION ${NIFTI_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
            ARCHIVE DESTINATION ${NIFTI_INSTALL_LIBRARY_DIR} COMPONENT Development
            INCLUDES DESTINATION ${NIFTI_INSTALL_INCLUDE_DIR}
            )
endif()

if(NOT NIFTI_INSTALL_NO_DEVELOPMENT)
    file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    install(FILES ${__files}
            DESTINATION ${NIFTI_INSTALL_INCLUDE_DIR}
            COMPONENT Development)
endif()


if(NIFTI_BUILD_TESTING AND NIFTI_BUILD_APPLICATIONS)
    add_executable(${NIFTI_PACKAGE_PREFIX}clib_02_nifti2 clib_02_nifti2.c)
    target_link_libraries(${NIFTI_PACKAGE_PREFIX}clib_02_nifti2 PUBLIC ${NIFTI_NIFTILIB2_NAME})

    # Do all regression tests

    set(NIFTI_TEST_DATA_DIR  ${NIFTI_BINARY_DIR}/Testing/nifti_regress_test/nifti_regress_data)
    set(NIFTI_TEST_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR}/nifti_regress_test/cmake_testscripts)

    add_test( NAME nifti_ver COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -nifti_ver)
    add_test( NAME nifti_tool_ver COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -ver)
    add_test( NAME nifti_help COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -help)
    add_test( NAME nifti_nifti_hist COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -nifti_hist)
    add_test( NAME nifti_hist COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -hist)

    add_test( NAME fetch_data
            COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/fetch_data_test.sh ${NIFTI_TEST_DATA_DIR}
            )

    add_test( NAME nifti_disp_hdr COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -disp_hdr -infiles ${NIFTI_TEST_DATA_DIR}/anat0.nii )
    set_tests_properties( nifti_disp_hdr PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_disp_nim COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -disp_nim -infiles ${NIFTI_TEST_DATA_DIR}/anat0.nii )
    set_tests_properties( nifti_disp_nim PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_disp_ext COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -disp_ext -infiles ${NIFTI_TEST_DATA_DIR}/anat0.nii )
    set_tests_properties( nifti_disp_ext PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_modhdr_exts COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/mod_header_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_modhdr_exts PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_bricks_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/bricks_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_bricks_test PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_dts_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/dts_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_dts_test PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_dci_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/dci_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_dci_test PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_comment_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/comment_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_comment_test PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_header_check COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -check_hdr -infiles ${NIFTI_TEST_DATA_DIR}/anat0.nii )
    set_tests_properties( nifti_header_check PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_nim_check COMMAND $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> -check_nim -infiles ${NIFTI_TEST_DATA_DIR}/anat0.nii )
    set_tests_properties( nifti_nim_check PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_dsets_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/dsets_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_dsets_test PROPERTIES DEPENDS fetch_data)

    add_test( NAME nifti_newfiles_test COMMAND sh ${NIFTI_TEST_SCRIPT_DIR}/newfiles_test.sh $<TARGET_FILE:${NIFTI_PACKAGE_PREFIX}nifti_tool> ${NIFTI_TEST_DATA_DIR} )
    set_tests_properties( nifti_newfiles_test PROPERTIES DEPENDS fetch_data)
endif()