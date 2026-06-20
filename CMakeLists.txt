cmake_minimum_required(VERSION 3.10)

project(KeePassXC
    VERSION 2.8.0
    LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Set minimum required versions
set(QT_MIN_VERSION "5.15.0")
set(ZLIB_MIN_VERSION "1.2.11")
set(BOTAN_MIN_VERSION "2.12.0")
set(ARGON2_MIN_VERSION "20171227")

# Include required modules
include(GNUInstallDirs)
include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(FindPkgConfig)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
include(cmake/CompilerFlags.cmake)

# Version information
configure_file(cmake/VersionInfo.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/config-keepassx.h)

# Find required packages
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core Gui Widgets Test LinguistTools Network
)

find_package(ZLIB ${ZLIB_MIN_VERSION} REQUIRED)
find_package(Botan ${BOTAN_MIN_VERSION} REQUIRED)
find_package(Argon2 ${ARGON2_MIN_VERSION} REQUIRED)
find_package(Zxcvbn REQUIRED)

# Optional packages
find_package(QREncode)
find_package(Readline)
find_package(PCSC)
find_package(Fingerprint)
find_package(Minizip)

# Set definitions
add_definitions(
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

# Set compiler definitions for optional features
if(QR_ENCODE_FOUND)
    add_definitions(-DHAVE_QRCODE)
endif()

if(READLINE_FOUND)
    add_definitions(-DHAVE_READLINE)
endif()

if(PCSC_FOUND)
    add_definitions(-DHAVE_PCSC)
endif()

if(FINGERPRINT_FOUND)
    add_definitions(-DHAVE_FINGERPRINT)
endif()

if(MINIZIP_FOUND)
    add_definitions(-DHAVE_MINIZIP)
endif()

# Source directories
add_subdirectory(src)
add_subdirectory(share)
add_subdirectory(docs)

# Install headers
install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/keepassx
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "src/core/private" EXCLUDE
    PATTERN "src/autotype" EXCLUDE
    PATTERN "src/keys/drivers" EXCLUDE
)