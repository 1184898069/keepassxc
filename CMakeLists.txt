# KeePassXC - Cross-Platform Password Manager
# Copyright (C) 2024 KeePassXC Team <team@keepassxc.org>
# Copyright (C) 2010 Felix Geyer <debfx@fobos.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.14)

# define project version
set(KEEPASSXC_VERSION_MAJOR "2")
set(KEEPASSXC_VERSION_MINOR "8")
set(KEEPASSXC_VERSION_PATCH "0")
set(KEEPASSXC_VERSION "${KEEPASSXC_VERSION_MAJOR}.${KEEPASSXC_VERSION_MINOR}.${KEEPASSXC_VERSION_PATCH}")
set(KEEPASSXC_VERSION_FULL "${KEEPASSXC_VERSION}")

set(ENV{CLICOLOR_FORCE} 1)

project(KeePassXC VERSION ${KEEPASSXC_VERSION} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_GREATER "3.18")
    cmake_policy(SET CMP0100 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateProductVersion)
include(CompilerFlags)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    set(CMAKE_INSTALL_MANDIR "${CMAKE_INSTALL_FULL_MANDIR}")
    set(CMAKE_INSTALL_APPDATADIR "${CMAKE_INSTALL_FULL_DATADIR}/metainfo")
    set(CMAKE_INSTALL_APPDIR "${CMAKE_INSTALL_FULL_DATADIR}/applications")
    set(CMAKE_INSTALL_ICONDIR "${CMAKE_INSTALL_FULL_DATADIR}/icons")
    set(CMAKE_INSTALL_SOUNDDIR "${CMAKE_INSTALL_FULL_DATADIR}/sounds")
    set(CMAKE_INSTALL_XDG_CONFIGDIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}/xdg")
endif()

if(WIN32)
    set(CMAKE_INSTALL_MANDIR "share/man")
    set(CMAKE_INSTALL_APPDATADIR "share/metainfo")
    set(CMAKE_INSTALL_APPDIR "share/applications")
    set(CMAKE_INSTALL_ICONDIR "share/icons")
    set(CMAKE_INSTALL_SOUNDDIR "share/sounds")
    set(CMAKE_INSTALL_XDG_CONFIGDIR "share/xdg")
endif()

if(APPLE)
    set(CMAKE_INSTALL_MANDIR "share/man")
    set(CMAKE_INSTALL_APPDATADIR "share/metainfo")
    set(CMAKE_INSTALL_APPDIR "share/applications")
    set(CMAKE_INSTALL_ICONDIR "share/icons")
    set(CMAKE_INSTALL_SOUNDDIR "share/sounds")
    set(CMAKE_INSTALL_XDG_CONFIGDIR "share/xdg")
endif()

# Check required Qt version
find_package(Qt5 5.15.0 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Xml
    Concurrent
    Svg
    Test
    LinguistTools
)

find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5 COMPONENTS DBus QUIET)
find_package(ZLIB 1.2.11 REQUIRED)
find_package(Botan 2.12.0 REQUIRED)
find_package(Argon2 20171227 REQUIRED)
find_package(Readline)
find_package(ZXing)
find_package(QREncode)
find_package(PCSC)
find_package(Fingerprint)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Remote database support options
option(WITH_XC_HTTP "Include KeePassHTTP support for browser integration" OFF)
option(WITH_XC_BROWSER "Include KeePassXC-Browser extension support" OFF)
option(WITH_XC_NETWORKING "Include networking support (for updates, etc.)" OFF)
option(WITH_XC_AUTOTYPE "Include Autotype support" ON)
option(WITH_XC_YUBIKEY "Include YubiKey support" OFF)
option(WITH_XC_FINGERPRINT "Include fingerprint unlock support" OFF)

# Remote database support
option(WITH_XC_REMOTE_DB "Include remote database support (SSH, FTP, SFTP)" OFF)
set(REMOTE_DB_SOURCES "")
set(REMOTE_DB_LIBS "")

if(WITH_XC_REMOTE_DB)
    find_package(OpenSSL REQUIRED)
    find_package(CURL REQUIRED)
    find_package(LibSSH2 REQUIRED)
    
    add_definitions(-DWITH_XC_REMOTE_DB)
    set(REMOTE_DB_SOURCES
        src/core/RemoteDatabase.cpp
        src/core/RemoteDatabase.h
        src/core/RemoteDatabaseModel.cpp
        src/core/RemoteDatabaseModel.h
        src/core/RemoteDatabaseWidget.cpp
        src/core/RemoteDatabaseWidget.h
        src/core/RemoteDatabaseSettings.cpp
        src/core/RemoteDatabaseSettings.h
        src/core/RemoteDatabaseSync.cpp
        src/core/RemoteDatabaseSync.h
        src/core/RemoteDatabaseSyncDialog.cpp
        src/core/RemoteDatabaseSyncDialog.h
    )
    set(REMOTE_DB_LIBS
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
        LibSSH2::libssh2
    )
endif()

# Remote database support (alternative: FTP/FTPS/SFTP via curl)
option(WITH_XC_CURL "Include curl-based remote database support" OFF)
if(WITH_XC_CURL)
    find_package(CURL REQUIRED)
    add_definitions(-DWITH_XC_CURL)
    if(NOT REMOTE_DB_SOURCES)
        set(REMOTE_DB_SOURCES
            src/core/RemoteDatabase.cpp
            src/core/RemoteDatabase.h
            src/core/RemoteDatabaseModel.cpp
            src/core/RemoteDatabaseModel.h
        )
    endif()
    list(APPEND REMOTE_DB_LIBS CURL::libcurl)
endif()

# Set remote database sources variable
set(REMOTE_DB_SOURCES_LIST ${REMOTE_DB_SOURCES})

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--dynamicbase")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--nxcompat")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--high-entropy-va")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-s")
endif()

add_subdirectory(src)
add_subdirectory(share)
add_subdirectory(docs)

# Add remote database test files if enabled
if(WITH_XC_REMOTE_DB OR WITH_XC_CURL)
    add_subdirectory(tests/remote)
endif()

# Install the .desktop file for Linux
if(UNIX AND NOT APPLE)
    install(FILES share/linux/org.keepassxc.KeePassXC.desktop
            DESTINATION ${CMAKE_INSTALL_APPDIR})
    install(FILES share/linux/org.keepassxc.KeePassXC.appdata.xml
            DESTINATION ${CMAKE_INSTALL_APPDATADIR})
    install(FILES share/linux/keepassxc.xml
            DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/mime/packages)
    install(DIRECTORY share/linux/sounds/
            DESTINATION ${CMAKE_INSTALL_SOUNDDIR})
    install(DIRECTORY share/linux/xdg/
            DESTINATION ${CMAKE_INSTALL_XDG_CONFIGDIR})
endif()

# Generate Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()

# CPack configuration
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "KeePassXC - Cross-Platform Password Manager")
set(CPACK_PACKAGE_VENDOR "KeePassXC Team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${KEEPASSXC_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${KEEPASSXC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${KEEPASSXC_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "team@keepassxc.org")

if(WIN32 AND NOT UNIX)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "KeePassXC")
    set(CPACK_NSIS_HELP_LINK "https://keepassxc.org")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://keepassxc.org")
    set(CPACK_NSIS_CONTACT "team@keepassxc.org")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/share/windows/keepassxc.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/share/windows/keepassxc.ico")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\keepassxc.exe")
    set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '$DESKTOP\\\\KeePassXC.lnk' '$INSTDIR\\\\bin\\\\keepassxc.exe'")
    set(CPACK_NSIS_DELETE_ICONS "Delete '$DESKTOP\\\\KeePassXC.lnk'")
    set(CPACK_NSIS_MENU_LINKS "bin/keepassxc.exe" "KeePassXC")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "KeePassXC")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/share/macosx/background.tiff")
    set(CPACK_DMG_DS_STORE "${CMAKE_CURRENT_SOURCE_DIR}/share/macosx/DS_Store")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)