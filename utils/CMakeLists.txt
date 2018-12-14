
add_executable(${NIFTI_PACKAGE_PREFIX}nifti_stats nifti_stats.c)
add_executable(${NIFTI_PACKAGE_PREFIX}nifti1_tool nifti1_tool.c)
add_executable(${NIFTI_PACKAGE_PREFIX}nifti1_test nifti1_test.c)
target_link_libraries( ${NIFTI_PACKAGE_PREFIX}nifti_stats PUBLIC ${NIFTI_PACKAGE_PREFIX}niftiio ${NIFTI_PACKAGE_PREFIX}nifticdf)
target_link_libraries( ${NIFTI_PACKAGE_PREFIX}nifti1_tool PUBLIC ${NIFTI_PACKAGE_PREFIX}niftiio)
target_link_libraries( ${NIFTI_PACKAGE_PREFIX}nifti1_test PUBLIC ${NIFTI_PACKAGE_PREFIX}niftiio)

if(NOT NIFTI_INSTALL_NO_DEVELOPMENT)
  install(TARGETS ${NIFTI_PACKAGE_PREFIX}nifti_stats ${NIFTI_PACKAGE_PREFIX}nifti1_tool ${NIFTI_PACKAGE_PREFIX}nifti1_test
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
    DESTINATION ${NIFTI_INSTALL_INCLUDE_DIR} COMPONENT Development)
endif()

add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_help ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test)
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_n1 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test  -n1  ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/n1 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_n2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test  -n2  ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/n2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_a2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test  -a2  ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/a2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_zn1 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test -zn1 ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/zn1 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_zn2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test -zn2 ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/zn2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_test_za2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_test -za2 ${NIFTI_SOURCE_DIR}/Testing/Data/ATestReferenceImageForReadingAndWriting.nii.gz ${NIFTI_BINARY_DIR}/utils/za2 )

#The help screens always return 1; add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool1_help ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool)
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_n1 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool  -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/n1 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_n2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool  -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/n2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_a2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool  -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/a2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_zn1 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/zn1 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_zn2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/zn2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_za2 ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -disp_hdr -infile ${NIFTI_BINARY_DIR}/utils/za2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_diff_hdr ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -diff_hdr -infile ${NIFTI_BINARY_DIR}/utils/zn1 ${NIFTI_BINARY_DIR}/utils/zn1 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_diff_nims ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -diff_nim -infile ${NIFTI_BINARY_DIR}/utils/zn1 ${NIFTI_BINARY_DIR}/utils/zn1 )

add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_copy_brick_list ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -cbl -prefix cbl_zn1.nii -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz[0,1])
#add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_copy_collapsed_image /bin/sh ${NIFTI_SOURCE_DIR}/utils/rmthenrun cci_zn1.nii ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -cci 2 2 2 -1 -1 -1 -1 -prefix cci_zn1.nii -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_disp_ci ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -disp_ci 2 2 2 -1 0 0 0  -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_disp_ts ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -disp_ts 2 2 2 -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_strip_extras ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -strip_extras -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz )

# This test needs a file that has extensions to remove
#add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_rm_ext /bin/sh ${NIFTI_SOURCE_DIR}/utils/rmthenrun rm_ext.nii ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -rm_ext ALL -prefix rm_ext.nii -infile ${NIFTI_BINARY_DIR}/utils/zn1.nii.gz )

add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_check_hdr ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -check_hdr -infile ${NIFTI_BINARY_DIR}/utils/za2 )
add_test(${NIFTI_PACKAGE_PREFIX}nifti1_tool_check_nim ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti1_tool -check_nim -infile ${NIFTI_BINARY_DIR}/utils/za2 )

foreach(DISTRIBUTION CORREL TTEST FTEST ZSCORE CHISQ BETA BINOM GAMMA POISSON NORMAL FTEST_NONC CHISQ_NONC LOGISTIC LAPLACE UNIFORM TTEST_NONC WEIBULL CHI INVGAUSS EXTVAL PVAL LOGPVAL LOG10PVAL )
    add_test(${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats 0:4:1 ${DISTRIBUTION})
    add_test(q${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -q 0:4:1 ${DISTRIBUTION})
    add_test(q1${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -q 0:4:1 ${DISTRIBUTION} 1 1 1)
    add_test(d${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -d 0:4:1 ${DISTRIBUTION})
    add_test(d1${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -d 0:4:1 ${DISTRIBUTION} 1 1 1)
    add_test(1${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -1 0:4:1 ${DISTRIBUTION})
    add_test(11${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -1 0:4:1 ${DISTRIBUTION} 1 1 1)
    add_test(z${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -z 0:4:1 ${DISTRIBUTION})
    add_test(z1${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -z 0:4:1 ${DISTRIBUTION} 1 1 1)
    add_test(h${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -h 0:4:1 ${DISTRIBUTION})
    add_test(h1${NIFTI_PACKAGE_PREFIX}nifti_stats_${DISTRIBUTION}_test ${NIFTI_BINARY_DIR}/utils/${NIFTI_PACKAGE_PREFIX}nifti_stats -h 0:4:1 ${DISTRIBUTION} 1 1 1)
endforeach()

