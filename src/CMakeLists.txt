# Source files for KeePassXC
set(keepassx_SOURCES
    core/Database.cpp
    core/DatabaseIcons.cpp
    core/DatabaseWidget.cpp
    core/Entry.cpp
    core/EntryAttachments.cpp
    core/EntryAttributes.cpp
    core/Group.cpp
    core/Metadata.cpp
    core/TimeDelta.cpp
    core/TimeInfo.cpp
    core/Uuid.cpp
    core/AutoTypeAssociations.cpp
    core/AutoTypeMatch.cpp
    core/Config.cpp
    core/CustomData.cpp
    core/Bootstrap.cpp
    core/Global.cpp
    core/Tools.cpp
    core/FileWatcher.cpp
    core/FilePath.cpp
    core/IconDownloader.cpp
    core/OpenSSHKey.cpp
    core/AsyncTask.cpp
    core/Clock.cpp
    core/HibpOffline.cpp
    core/NetworkManager.cpp
    core/Statistics.cpp
    core/Totp.cpp
    core/PasswordHealth.cpp
    core/PassphraseGenerator.cpp
    core/PasswordGenerator.cpp
    core/CharacterSubstitutions.cpp
    core/Base32.cpp
    core/Base64.cpp
    core/QCodeArea.cpp
    core/qrcode/BitBuffer.cpp
    core/qrcode/QrCode.cpp
    core/qrcode/QrSegment.cpp
    
    keys/CompositeKey.cpp
    keys/CompositeKey_p.h
    keys/FileKey.cpp
    keys/PasswordKey.cpp
    keys/ChallengeResponseKey.cpp
    keys/drivers/ChallengeResponseKeyInterface.cpp
    keys/drivers/YubiKeyInterface.cpp
    keys/drivers/YubiKeyInterfacePCSC.cpp
    keys/drivers/YubiKeyInterfaceUSB.cpp
    keys/drivers/FingerprintKey.cpp
    
    streams/HashedBlockStream.cpp
    streams/HmacBlockStream.cpp
    streams/LayeredStream.cpp
    streams/qcompressor.cpp
    streams/StoreDataStream.cpp
    streams/QtIOCompressor.cpp
    
    format/KeePass2.cpp
    format/KeePass2RandomStream.cpp
    format/KeePass2XmlReader.cpp
    format/KeePass2XmlWriter.cpp
    format/KeePass2Repair.cpp
    format/Kdbx3Reader.cpp
    format/Kdbx3Writer.cpp
    format/Kdbx4Reader.cpp
    format/Kdbx4Writer.cpp
    format/KdbxXmlReader.cpp
    format/KdbxXmlWriter.cpp
    format/KdbxDatabase.cpp
    
    crypto/CryptoHash.cpp
    crypto/Crypto.cpp
    crypto/SymmetricCipher.cpp
    crypto/SymmetricCipherGcrypt.cpp
    crypto/SymmetricCipherBackend.cpp
    crypto/CryptoManager.cpp
    crypto/kdf/Kdf.cpp
    crypto/kdf/KdfFactory.cpp
    crypto/kdf/AesKdf.cpp
    crypto/kdf/Argon2Kdf.cpp
    crypto/kdf/Argon2Kdf_p.h
    
    crypto/keys/FileKeyChallenge.cpp
    
    icons/KeePassXCIcons.cpp
    
    zlib/zlib.h
    zlib/adler32.c
    zlib/compress.c
    zlib/crc32.c
    zlib/deflate.c
    zlib/gzclose.c
    zlib/gzlib.c
    zlib/gzread.c
    zlib/gzwrite.c
    zlib/infback.c
    zlib/inffast.c
    zlib/inflate.c
    zlib/inftrees.c
    zlib/trees.c
    zlib/uncompr.c
    zlib/zutil.c
)

set(keepassx_SOURCES ${keepassx_SOURCES}
    # Autotype
    autotype/AutoType.cpp
    autotype/AutoTypePlatformPlugin.h
    autotype/AutoTypeSelectDialog.cpp
    autotype/AutoTypeSelectDialog.h
    autotype/AutoTypeSelectView.cpp
    autotype/AutoTypeSelectView.h
    autotype/AutoTypeMatchModel.cpp
    autotype/AutoTypeMatchModel.h
    autotype/AutoTypeMatchView.cpp
    autotype/AutoTypeMatchView.h
    
    # Browser integration
    browser/BrowserService.cpp
    browser/BrowserService.h
    browser/BrowserSettings.cpp
    browser/BrowserSettings.h
    browser/BrowserHost.cpp
    browser/BrowserHost.h
    browser/BrowserMessage.cpp
    browser/BrowserMessage.h
    browser/BrowserAction.cpp
    browser/BrowserAction.h
    browser/BrowserEntryConfig.cpp
    browser/BrowserEntryConfig.h
    browser/BrowserEntryMatcher.cpp
    browser/BrowserEntryMatcher.h
    
    # CLI
    cli/Utils.cpp
    cli/Utils.h
    cli/TextStream.cpp
    cli/TextStream.h
    cli/DatabaseTabs.cpp
    cli/DatabaseTabs.h
    cli/KeepassxcCli.cpp
    cli/KeepassxcCli.h
    
    # GUI
    gui/DatabaseWidget.cpp
    gui/DatabaseWidget.h
    gui/DatabaseTabWidget.cpp
    gui/DatabaseTabWidget.h
    gui/MainWindow.cpp
    gui/MainWindow.h
    gui/AboutDialog.cpp
    gui/AboutDialog.h
    gui/SettingsWidget.cpp
    gui/SettingsWidget.h
    gui/MainWindow.ui
    gui/SettingsWidget.ui
    gui/entry/EntryView.cpp
    gui/entry/EntryView.h
    gui/entry/EntryModel.cpp
    gui/entry/EntryModel.h
    gui/entry/EditEntryWidget.cpp
    gui/entry/EditEntryWidget.h
    gui/entry/EditEntryWidget.ui
    gui/group/GroupView.cpp
    gui/group/GroupView.h
    gui/group/GroupModel.cpp
    gui/group/GroupModel.h
    gui/group/EditGroupWidget.cpp
    gui/group/EditGroupWidget.h
    gui/group/EditGroupWidget.ui
    gui/IconModels.cpp
    gui/IconModels.h
    gui/KeePass1OpenWidget.cpp
    gui/KeePass1OpenWidget.h
    gui/KeePass1OpenWidget.ui
    gui/KMessageWidget.cpp
    gui/KMessageWidget.h
    gui/MessageBox.cpp
    gui/MessageBox.h
    gui/PasswordEdit.cpp
    gui/PasswordEdit.h
    gui/PasswordGeneratorWidget.cpp
    gui/PasswordGeneratorWidget.h
    gui/PasswordGeneratorWidget.ui
    gui/PasswordWidget.cpp
    gui/PasswordWidget.h
    gui/WelcomeWidget.cpp
    gui/WelcomeWidget.h
    gui/WelcomeWidget.ui
    gui/DatabaseOpenWidget.cpp
    gui/DatabaseOpenWidget.h
    gui/DatabaseOpenWidget.ui
    gui/csvImport/CsvImportWidget.cpp
    gui/csvImport/CsvImportWidget.h
    gui/csvImport/CsvImportWidget.ui
    gui/csvImport/CsvImportWizard.cpp
    gui/csvImport/CsvImportWizard.h
    gui/csvImport/CsvParserModel.cpp
    gui/csvImport/CsvParserModel.h
    gui/keys/KeyFileEditWidget.cpp
    gui/keys/KeyFileEditWidget.h
    gui/keys/KeyFileEditWidget.ui
    gui/keys/PasswordEditWidget.cpp
    gui/keys/PasswordEditWidget.h
    gui/keys/PasswordEditWidget.ui
    gui/keys/ChallengeResponseEditWidget.cpp
    gui/keys/ChallengeResponseEditWidget.h
    gui/keys/ChallengeResponseEditWidget.ui
    gui/keys/FingerprintEditWidget.cpp
    gui/keys/FingerprintEditWidget.h
    gui/keys/FingerprintEditWidget.ui
    gui/dbsettings/DatabaseSettingsWidget.cpp
    gui/dbsettings/DatabaseSettingsWidget.h
    gui/dbsettings/DatabaseSettingsWidgetGeneral.cpp
    gui/dbsettings/DatabaseSettingsWidgetGeneral.h
    gui/dbsettings/DatabaseSettingsWidgetGeneral.ui
    gui/dbsettings/DatabaseSettingsWidgetEncryption.cpp
    gui/dbsettings/DatabaseSettingsWidgetEncryption.h
    gui/dbsettings/DatabaseSettingsWidgetEncryption.ui
    gui/dbsettings/DatabaseSettingsWidgetBrowserIntegration.cpp
    gui/dbsettings/DatabaseSettingsWidgetBrowserIntegration.h
    gui/dbsettings/DatabaseSettingsWidgetBrowserIntegration.ui
    gui/dbsettings/DatabaseSettingsWidgetMaintenance.cpp
    gui/dbsettings/DatabaseSettingsWidgetMaintenance.h
    gui/dbsettings/DatabaseSettingsWidgetMaintenance.ui
    gui/dbsettings/DatabaseSettingsWidgetRemote.cpp
    gui/dbsettings/DatabaseSettingsWidgetRemote.cpp
    gui/dbsettings/DatabaseSettingsWidgetRemote.h
    gui/dbsettings/DatabaseSettingsWidgetRemote.ui
    gui/reports/ReportsWidget.cpp
    gui/reports/ReportsWidget.h
    gui/reports/ReportsWidget.ui
    gui/reports/ReportsWidgetHealthcheck.cpp
    gui/reports/ReportsWidgetHealthcheck.h
    gui/reports/ReportsWidgetHealthcheck.ui
    gui/reports/ReportsWidgetStatistics.cpp
    gui/reports/ReportsWidgetStatistics.h
    gui/reports/ReportsWidgetStatistics.ui
    gui/reports/ReportsWidgetBrowserStatistics.cpp
    gui/reports/ReportsWidgetBrowserStatistics.h
    gui/reports/ReportsWidgetBrowserStatistics.ui
    gui/reports/ReportsWidgetHibp.cpp
    gui/reports/ReportsWidgetHibp.h
    gui/reports/ReportsWidgetHibp.ui
    gui/wizard/NewDatabaseWizard.cpp
    gui/wizard/NewDatabaseWizard.h
    gui/wizard/NewDatabaseWizardPage.cpp
    gui/wizard/NewDatabaseWizardPage.h
    gui/wizard/NewDatabaseWizardPageSetup.cpp
    gui/wizard/NewDatabaseWizardPageSetup.h
    gui/wizard/NewDatabaseWizardPageSetup.ui
    gui/wizard/NewDatabaseWizardPageEncryption.cpp
    gui/wizard/NewDatabaseWizardPageEncryption.h
    gui/wizard/NewDatabaseWizardPageEncryption.ui
    gui/wizard/NewDatabaseWizardPageMasterKey.cpp
    gui/wizard/NewDatabaseWizardPageMasterKey.h
    gui/wizard/NewDatabaseWizardPageMasterKey.ui
    
    # Network
    network/NetworkManager.cpp
    network/NetworkManager.h
    network/HibpDownloader.cpp
    network/HibpDownloader.h
    network/IconDownloader.cpp
    network/IconDownloader.h
    
    # Streams
    streams/HashedBlockStream.cpp
    streams/HmacBlockStream.cpp
    streams/LayeredStream.cpp
    streams/qcompressor.cpp
    streams/StoreDataStream.cpp
    streams/QtIOCompressor.cpp
)

# Qt MOC sources
set(keepassx_MOC_SOURCES
    core/Database.h
    core/DatabaseWidget.h
    core/Entry.h
    core/Group.h
    core/Metadata.h
    core/Config.h
    core/CustomData.h
    core/FilePath.h
    core/FileWatcher.h
    core/IconDownloader.h
    core/NetworkManager.h
    core/AsyncTask.h
    core/HibpOffline.h
    core/Totp.h
    
    keys/CompositeKey.h
    keys/FileKey.h
    keys/PasswordKey.h
    keys/ChallengeResponseKey.h
    keys/drivers/ChallengeResponseKeyInterface.h
    keys/drivers/YubiKeyInterface.h
    keys/drivers/YubiKeyInterfacePCSC.h
    keys/drivers/YubiKeyInterfaceUSB.h
    keys/drivers/FingerprintKey.h
    
    autotype/AutoType.h
    autotype/AutoTypeSelectDialog.h
    autotype/AutoTypeSelectView.h
    autotype/AutoTypeMatchModel.h
    autotype/AutoTypeMatchView.h
    
    browser/BrowserService.h
    browser/BrowserSettings.h
    browser/BrowserHost.h
    browser/BrowserMessage.h
    browser/BrowserAction.h
    browser/BrowserEntryConfig.h
    browser/BrowserEntryMatcher.h
    
    cli/Utils.h
    cli/TextStream.h
    cli/DatabaseTabs.h
    cli/KeepassxcCli.h
    
    gui/DatabaseWidget.h
    gui/DatabaseTabWidget.h
    gui/MainWindow.h
    gui/AboutDialog.h
    gui/SettingsWidget.h
    gui/entry/EntryView.h
    gui/entry/EntryModel.h
    gui/entry/EditEntryWidget.h
    gui/group/GroupView.h
    gui/group/GroupModel.h
    gui/group/EditGroupWidget.h
    gui/IconModels.h
    gui/KeePass1OpenWidget.h
    gui/KMessageWidget.h
    gui/MessageBox.h
    gui/PasswordEdit.h
    gui/PasswordGeneratorWidget.h
    gui/PasswordWidget.h
    gui/WelcomeWidget.h
    gui/DatabaseOpenWidget.h
    gui/csvImport/CsvImportWidget.h
    gui/csvImport/CsvImportWizard.h
    gui/csvImport/CsvParserModel.h
    gui/keys/KeyFileEditWidget.h
    gui/keys/PasswordEditWidget.h
    gui/keys/ChallengeResponseEditWidget.h
    gui/keys/FingerprintEditWidget.h
    gui/dbsettings/DatabaseSettingsWidget.h
    gui/dbsettings/DatabaseSettingsWidgetGeneral.h
    gui/dbsettings/DatabaseSettingsWidgetEncryption.h
    gui/dbsettings/DatabaseSettingsWidgetBrowserIntegration.h
    gui/dbsettings/DatabaseSettingsWidgetMaintenance.h
    gui/dbsettings/DatabaseSettingsWidgetRemote.h
    gui/reports/ReportsWidget.h
    gui/reports/ReportsWidgetHealthcheck.h
    gui/reports/ReportsWidgetStatistics.h
    gui/reports/ReportsWidgetBrowserStatistics.h
    gui/reports/ReportsWidgetHibp.h
    gui/wizard/NewDatabaseWizard.h
    gui/wizard/NewDatabaseWizardPage.h
    gui/wizard/NewDatabaseWizardPageSetup.h
    gui/wizard/NewDatabaseWizardPageEncryption.h
    gui/wizard/NewDatabaseWizardPageMasterKey.h
    
    network/NetworkManager.h
    network/HibpDownloader.h
    network/IconDownloader.h
)

# UI files
set(keepassx_FORMS
    gui/MainWindow.ui
    gui/SettingsWidget.ui
    gui/entry/EditEntryWidget.ui
    gui/group/EditGroupWidget.ui
    gui/KeePass1OpenWidget.ui
    gui/PasswordGeneratorWidget.ui
    gui/WelcomeWidget.ui
    gui/DatabaseOpenWidget.ui
    gui/csvImport/CsvImportWidget.ui
    gui/keys/KeyFileEditWidget.ui
    gui/keys/PasswordEditWidget.ui
    gui/keys/ChallengeResponseEditWidget.ui
    gui/keys/FingerprintEditWidget.ui
    gui/dbsettings/DatabaseSettingsWidgetGeneral.ui
    gui/dbsettings/DatabaseSettingsWidgetEncryption.ui
    gui/dbsettings/DatabaseSettingsWidgetBrowserIntegration.ui
    gui/dbsettings/DatabaseSettingsWidgetMaintenance.ui
    gui/dbsettings/DatabaseSettingsWidgetRemote.ui
    gui/reports/ReportsWidget.ui
    gui/reports/ReportsWidgetHealthcheck.ui
    gui/reports/ReportsWidgetStatistics.ui
    gui/reports/ReportsWidgetBrowserStatistics.ui
    gui/reports/ReportsWidgetHibp.ui
    gui/wizard/NewDatabaseWizardPageSetup.ui
    gui/wizard/NewDatabaseWizardPageEncryption.ui
    gui/wizard/NewDatabaseWizardPageMasterKey.ui
)

# Resources
set(keepassx_RESOURCES
    ${CMAKE_SOURCE_DIR}/share/icons/icons.qrc
    ${CMAKE_SOURCE_DIR}/share/translations/translations.qrc
    ${CMAKE_SOURCE_DIR}/share/welcomescreen/welcomescreen.qrc
    ${CMAKE_SOURCE_DIR}/share/icons/icons_kdbx.qrc
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${BOTAN_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${ARGON2_INCLUDE_DIRS}
    ${ZXCVBN_INCLUDE_DIRS}
)

# Add optional include directories
if(QR_ENCODE_FOUND)
    include_directories(${QR_ENCODE_INCLUDE_DIRS})
endif()

if(READLINE_FOUND)
    include_directories(${READLINE_INCLUDE_DIRS})
endif()

if(PCSC_FOUND)
    include_directories(${PCSC_INCLUDE_DIRS})
endif()

if(FINGERPRINT_FOUND)
    include_directories(${FINGERPRINT_INCLUDE_DIRS})
endif()

if(MINIZIP_FOUND)
    include_directories(${MINIZIP_INCLUDE_DIRS})
endif()

# Generate Qt MOC files
qt5_wrap_cpp(keepassx_MOC_SOURCES ${keepassx_MOC_SOURCES})

# Generate UI files
qt5_wrap_ui(keepassx_FORMS ${keepassx_FORMS})

# Generate resource files
qt5_add_resources(keepassx_RESOURCES_RCC ${keepassx_RESOURCES})

# Set library dependencies
set(keepassx_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    ${BOTAN_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${ARGON2_LIBRARIES}
    ${ZXCVBN_LIBRARIES}
)

# Add optional libraries
if(QR_ENCODE_FOUND)
    list(APPEND keepassx_LIBS ${QR_ENCODE_LIBRARIES})
endif()

if(READLINE_FOUND)
    list(APPEND keepassx_LIBS ${READLINE_LIBRARIES})
endif()

if(PCSC_FOUND)
    list(APPEND keepassx_LIBS ${PCSC_LIBRARIES})
endif()

if(FINGERPRINT_FOUND)
    list(APPEND keepassx_LIBS ${FINGERPRINT_LIBRARIES})
endif()

if(MINIZIP_FOUND)
    list(APPEND keepassx_LIBS ${MINIZIP_LIBRARIES})
endif()

# Create the main executable
add_executable(keepassxc
    ${keepassx_SOURCES}
    ${keepassx_MOC_SOURCES}
    ${keepassx_FORMS}
    ${keepassx_RESOURCES_RCC}
    main.cpp
)

# Link libraries
target_link_libraries(keepassxc ${keepassx_LIBS})

# Set compile definitions
target_compile_definitions(keepassxc PRIVATE
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

# Add optional compile definitions
if(QR_ENCODE_FOUND)
    target_compile_definitions(keepassxc PRIVATE -DHAVE_QRCODE)
endif()

if(READLINE_FOUND)
    target_compile_definitions(keepassxc PRIVATE -DHAVE_READLINE)
endif()

if(PCSC_FOUND)
    target_compile_definitions(keepassxc PRIVATE -DHAVE_PCSC)
endif()

if(FINGERPRINT_FOUND)
    target_compile_definitions(keepassxc PRIVATE -DHAVE_FINGERPRINT)
endif()

if(MINIZIP_FOUND)
    target_compile_definitions(keepassxc PRIVATE -DHAVE_MINIZIP)
endif()

# Install executable
install(TARGETS keepassxc
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Create man pages
if(UNIX AND NOT APPLE)
    install(FILES share/linux/org.keepassxc.KeePassXC.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )
    
    install(FILES share/linux/org.keepassxc.KeePassXC.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
    )
    
    install(FILES share/linux/org.keepassxc.KeePassXC.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
    )
endif()

# Create translations
if(${Qt5LinguistTools_FOUND})
    add_subdirectory(share/translations)
endif()

# Add tests
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()