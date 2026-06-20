# FindFingerprint.cmake
# Find fingerprint reader support for Linux
#
# This module defines:
#  FINGERPRINT_FOUND - System has fingerprint support
#  FINGERPRINT_INCLUDE_DIRS - The fingerprint include directories
#  FINGERPRINT_LIBRARIES - The libraries needed to use fingerprint
#  FINGERPRINT_DEFINITIONS - Compiler switches required for using fingerprint

if(APPLE)
    # macOS uses Touch ID through LocalAuthentication framework
    set(FINGERPRINT_FOUND FALSE)
    return()
endif()

if(WIN32)
    # Windows uses Windows Hello fingerprint API
    set(FINGERPRINT_FOUND FALSE)
    return()
endif()

# Linux fingerprint support using fprint
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(FPRINT QUIET libfprint-2)
    if(NOT FPRINT_FOUND)
        pkg_check_modules(FPRINT QUIET libfprint)
    endif()
endif()

# Check for fprint header files
find_path(FPRINT_INCLUDE_DIR
    NAMES fprint.h
    PATHS
        /usr/include
        /usr/local/include
        /usr/include/libfprint-2
        /usr/include/libfprint
    PATH_SUFFIXES
        libfprint-2
        libfprint
)

# Find the fprint library
find_library(FPRINT_LIBRARY
    NAMES fprint-2 fprint
    PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/aarch64-linux-gnu
)

if(FPRINT_INCLUDE_DIR AND FPRINT_LIBRARY)
    set(FINGERPRINT_FOUND TRUE)
    set(FINGERPRINT_INCLUDE_DIRS ${FPRINT_INCLUDE_DIR})
    set(FINGERPRINT_LIBRARIES ${FPRINT_LIBRARY})
    set(FINGERPRINT_DEFINITIONS "-DHAVE_FINGERPRINT=1")
else()
    set(FINGERPRINT_FOUND FALSE)
endif()

# Check for PAM fingerprint support
find_path(PAM_INCLUDE_DIR
    NAMES pam_modules.h
    PATHS
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        security
)

find_library(PAM_LIBRARY
    NAMES pam
    PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/aarch64-linux-gnu
)

if(PAM_INCLUDE_DIR AND PAM_LIBRARY)
    set(HAVE_PAM TRUE)
    list(APPEND FINGERPRINT_INCLUDE_DIRS ${PAM_INCLUDE_DIR})
    list(APPEND FINGERPRINT_LIBRARIES ${PAM_LIBRARY})
    list(APPEND FINGERPRINT_DEFINITIONS "-DHAVE_PAM=1")
endif()

mark_as_advanced(FPRINT_INCLUDE_DIR FPRINT_LIBRARY PAM_INCLUDE_DIR PAM_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fingerprint
    REQUIRED_VARS FPRINT_INCLUDE_DIR FPRINT_LIBRARY
)