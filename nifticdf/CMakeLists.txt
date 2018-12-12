cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

include(GNUInstallDirs)
include(FetchContent)
FetchContent_Declare(
  niftiio
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../niftilib
)
FetchContent_GetProperties(niftiio)
if(NOT niftiio_POPULATED)
  FetchContent_Populate(niftiio)
  add_subdirectory(${niftiio_SOURCE_DIR} ${niftiio_BINARY_DIR})
endif()


set(NIFTI_LIB_NAME ${PACKAGE_PREFIX}nifticdf)
add_library(${NIFTI_LIB_NAME} nifticdf.c)
target_include_directories(${NIFTI_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
if(BUILD_SHARED_LIBS)
  set_target_properties(${NIFTI_LIB_NAME} PROPERTIES ${NIFTI_LIBRARY_PROPERTIES})
endif()
target_link_libraries(${NIFTI_LIB_NAME} niftiio)

if(NOT NIFTI_INSTALL_NO_LIBRARIES)
  install(TARGETS ${NIFTI_LIB_NAME}
          EXPORT NIFTITargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
endif()

if(NOT NIFTI_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Development)
endif()
