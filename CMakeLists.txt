cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(NIFTI_MAX_VALIDATED_CMAKE_VERSION "3.13.1")
if("${CMAKE_VERSION}" VERSION_LESS_EQUAL "${NIFTI_MAX_VALIDATED_CMAKE_VERSION}")
  # As of 2018-12-04 NIFTI has been validated to build with cmake version 3.13.1 new policies.
  # Set and use the newest cmake policies that are validated to work
  set(NIFTI_CMAKE_POLICY_VERSION "${CMAKE_VERSION}")
else()
  set(NIFTI_CMAKE_POLICY_VERSION "${NIFTI_MAX_VALIDATED_CMAKE_VERSION}")
endif()
cmake_policy(VERSION ${NIFTI_CMAKE_POLICY_VERSION})


set(NIFTI_HOMEPAGE_URL "https://nifti-imaging.github.io")
project(NIFTI
        VERSION 2.1.0
        DESCRIPTION "Niftilib is a set of i/o libraries for reading and writing files in the nifti-1 data format. nifti-1 is a binary file format for storing medical image data, e.g. magnetic resonance image (MRI) and functional MRI (fMRI) brain images."
        LANGUAGES C)

macro(set_if_not_defined var defaultvalue)
# Macro allowing to set a variable to its default value if not already defined.
# The default value is set with:
#  (1) if set, the value environment variable <var>.
#  (2) if set, the value of local variable variable <var>.
#  (3) if none of the above, the value passed as a parameter.
# Setting the optional parameter 'OBFUSCATE' will display 'OBFUSCATED' instead of the real value.
  set(_obfuscate FALSE)
  foreach(arg ${ARGN})
    if(arg STREQUAL "OBFUSCATE")
      set(_obfuscate TRUE)
    endif()
  endforeach()
  if(DEFINED ENV{${var}} AND NOT DEFINED ${var})
    set(_value "$ENV{${var}}")
    if(_obfuscate)
      set(_value "OBFUSCATED")
    endif()
    message(STATUS "Setting '${var}' variable with environment variable value '${_value}'")
    set(${var} $ENV{${var}})
  endif()
  if(NOT DEFINED ${var})
    set(_value "${defaultvalue}")
    if(_obfuscate)
      set(_value "OBFUSCATED")
    endif()
    message(STATUS "Setting '${var}' variable with default value '${_value}'")
    set(${var} "${defaultvalue}")
  endif()
endmacro()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

add_definitions(-DHAVE_ZLIB)

# add option to build shared libraries. The default is OFF to maintain the
# current build behavior
option( BUILD_SHARED_LIBS "Toggle building shared libraries." OFF)


set(CPACK_PACKAGE_VERSION_MAJOR "${NIFTI_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NIFTI_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${NIFTI_VERSION_PATCH}")
###  USE AS STAND ALONE PACKAGE
find_package(ZLIB)
#  message(STATUS "---------------------SYSTEM ZLIB ${ZLIB_LIBRARY}")
set_if_not_defined(PACKAGE_PREFIX "")

# Set default shared library version
# This library version will be applied to all libraries in the package
# unless it is not explicitely for a certain lib.
set(NIFTI_SHAREDLIB_VERSION ${NIFTI_VERSION_MAJOR}.${NIFTI_VERSION_MINOR}.${NIFTI_VERSION_PATCH} )
string( REGEX MATCH "^[0-9]+" NIFTI_SHAREDLIB_SOVERSION ${NIFTI_SHAREDLIB_VERSION})
if(BUILD_SHARED_LIBS)
  set_if_not_defined(NIFTI_LIBRARY_PROPERTIES
    BUILD_SHARED_LIBS TRUE
    POSITION_INDEPENDENT_CODE TRUE
    VERSION ${NIFTI_SHAREDLIB_VERSION} SOVERSION ${NIFTI_SHAREDLIB_SOVERSION}
  )
endif()

set_if_not_defined(NIFTI_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set_if_not_defined(NIFTI_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set_if_not_defined(NIFTI_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/nifti")
set_if_not_defined(NIFTI_ZLIB_LIBRARIES ${ZLIB_LIBRARIES})
set_if_not_defined(NIFTI_INSTALL_NO_DEVELOPMENT 0)
set_if_not_defined(NIFTI_INSTALL_NO_LIBRARIES 0)

#Needs an if clause and more work before testing can take place.
add_subdirectory(Testing)
add_subdirectory(nifticdf utils)

option(USE_FSL_CODE "If OFF, The copyright of this code is questionable for inclusion with nifti." OFF)
mark_as_advanced(USE_FSL_CODE)

# the order of add_subdirectory is important! fsliolob has to preceed examples
# as otherwise FSLIOLIB_SOURCE_DIR is undefined and hence the examples
# will fail to compile
if(USE_FSL_CODE)
  add_subdirectory(fsliolib)
  add_subdirectory(examples)
endif()
enable_testing()
include(CTest)

####################################
# CMake itself and can use some CMake facilities for creating the package files.
# This allows for find_package(NIFTI 2.1.0 NO_MODULE) to work for pulling in
# NIFTI libraries into an external project
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/NIFTI/NIFTIConfigVersion.cmake"
        VERSION ${NIFTI_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT NIFTITargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/NIFTI/NIFTITargets.cmake"
        NAMESPACE NIFTI::
        )
configure_file(cmake/NIFTIConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/NIFTI/NIFTIConfig.cmake"
        COPYONLY
        )

set(ConfigPackageLocation lib/cmake/NIFTI)
install(EXPORT NIFTITargets
        FILE          NIFTITargets.cmake
        NAMESPACE     NIFTI::
        DESTINATION   ${ConfigPackageLocation}
        )
install(FILES
           cmake/NIFTIConfig.cmake
           "${CMAKE_CURRENT_BINARY_DIR}/NIFTI/NIFTIConfigVersion.cmake"
        DESTINATION   ${ConfigPackageLocation}
        COMPONENT     Development
)

####################################
### Define information necessary for packaging with CPACK (http://www.cmake.org/Wiki/CMake:Packaging_With_CPack)
### The last section is concerned with installing the binaries and making distributions.
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-dev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${NIFTI_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${NIFTI_SOURCE_DIR}/README")
#set(CPACK_RESOURCE_FILE_LICENSE "${NIFTI_SOURCE_DIR}/Copyright.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${NIFTI_SOURCE_DIR}/README")

set(CPACK_PACKAGE_DEFAULT_LOCATION "/opt/${CPACK_PACKAGE_NAME}")
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "NIFTI ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${NIFTI}")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CPACK_SET_DESTDIR ON)

set(CPACK_PACKAGE_VENDOR "NIFTI DFWG")

set(CPACK_SOURCE_GENERATOR "TGZ;TZ")
#set(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\.svn/;\\.swp$;\\.#;/#;\\.*~")
set(CPACK_SOURCE_IGNORE_FILES "")
set(CPACK_PACKAGE_EXECUTABLES "nifti_tool;NIFTI")

if(WIN32 AND NOT UNIX)
  set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY
"${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_PACKAGE_ICON
"${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} NIFTI Project")
  set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\www.nitrc.org")
  set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\www.nitrc.org")
  set(CPACK_NSIS_CONTACT "xyz@domain.edu")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_STRIP_FILES OFF)
  set(CPACK_SOURCE_STRIP_FILES OFF)
endif()
include(CPack)

add_subdirectory(znzlib)
add_subdirectory(niftilib)
