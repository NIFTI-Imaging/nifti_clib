cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
  
include(FindEXPAT)
include(FetchContent)
FetchContent_Declare(
  nifti2io
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../nifti2
)
FetchContent_GetProperties(nifti2io)
if(NOT nifti2io_POPULATED)
  FetchContent_Populate(nifti2io)
  add_subdirectory(${nifti2io_SOURCE_DIR} ${nifti2io_BINARY_DIR})
endif()


add_executable(cifti_tool cifti_tool.c afni_xml.c afni_xml_io.c)
target_include_directories(cifti_tool PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
target_link_libraries(cifti_tool nifti2io EXPAT::EXPAT)


# Exporting and installing
if(NOT NIFTI_INSTALL_NO_LIBRARIES)
  install(TARGETS  cifti_tool
          EXPORT NIFTITargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
endif()

# if(NOT NIFTI_INSTALL_NO_DEVELOPMENT)
#   file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#   install(FILES ${__files}
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#     COMPONENT Development)
# endif()
