# GUI module sources
set(gui_SOURCES
    DatabaseWidget.cpp
    DatabaseTabWidget.cpp
    MainWindow.cpp
    AboutDialog.cpp
    SettingsWidget.cpp
    entry/EntryView.cpp
    entry/EntryModel.cpp
    entry/EditEntryWidget.cpp
    group/GroupView.cpp
    group/GroupModel.cpp
    group/EditGroupWidget.cpp
    IconModels.cpp
    KeePass1OpenWidget.cpp
    KMessageWidget.cpp
    MessageBox.cpp
    PasswordEdit.cpp
    PasswordGeneratorWidget.cpp
    PasswordWidget.cpp
    WelcomeWidget.cpp
    DatabaseOpenWidget.cpp
    csvImport/CsvImportWidget.cpp
    csvImport/CsvImportWizard.cpp
    csvImport/CsvParserModel.cpp
    keys/KeyFileEditWidget.cpp
    keys/PasswordEditWidget.cpp
    keys/ChallengeResponseEditWidget.cpp
    keys/FingerprintEditWidget.cpp
    dbsettings/DatabaseSettingsWidget.cpp
    dbsettings/DatabaseSettingsWidgetGeneral.cpp
    dbsettings/DatabaseSettingsWidgetEncryption.cpp
    dbsettings/DatabaseSettingsWidgetBrowserIntegration.cpp
    dbsettings/DatabaseSettingsWidgetMaintenance.cpp
    dbsettings/DatabaseSettingsWidgetRemote.cpp
    reports/ReportsWidget.cpp
    reports/ReportsWidgetHealthcheck.cpp
    reports/ReportsWidgetStatistics.cpp
    reports/ReportsWidgetBrowserStatistics.cpp
    reports/ReportsWidgetHibp.cpp
    wizard/NewDatabaseWizard.cpp
    wizard/NewDatabaseWizardPage.cpp
    wizard/NewDatabaseWizardPageSetup.cpp
    wizard/NewDatabaseWizardPageEncryption.cpp
    wizard/NewDatabaseWizardPageMasterKey.cpp
)

set(gui_MOC_SOURCES
    DatabaseWidget.h
    DatabaseTabWidget.h
    MainWindow.h
    AboutDialog.h
    SettingsWidget.h
    entry/EntryView.h
    entry/EntryModel.h
    entry/EditEntryWidget.h
    group/GroupView.h
    group/GroupModel.h
    group/EditGroupWidget.h
    IconModels.h
    KeePass1OpenWidget.h
    KMessageWidget.h
    MessageBox.h
    PasswordEdit.h
    PasswordGeneratorWidget.h
    PasswordWidget.h
    WelcomeWidget.h
    DatabaseOpenWidget.h
    csvImport/CsvImportWidget.h
    csvImport/CsvImportWizard.h
    csvImport/CsvParserModel.h
    keys/KeyFileEditWidget.h
    keys/PasswordEditWidget.h
    keys/ChallengeResponseEditWidget.h
    keys/FingerprintEditWidget.h
    dbsettings/DatabaseSettingsWidget.h
    dbsettings/DatabaseSettingsWidgetGeneral.h
    dbsettings/DatabaseSettingsWidgetEncryption.h
    dbsettings/DatabaseSettingsWidgetBrowserIntegration.h
    dbsettings/DatabaseSettingsWidgetMaintenance.h
    dbsettings/DatabaseSettingsWidgetRemote.h
    reports/ReportsWidget.h
    reports/ReportsWidgetHealthcheck.h
    reports/ReportsWidgetStatistics.h
    reports/ReportsWidgetBrowserStatistics.h
    reports/ReportsWidgetHibp.h
    wizard/NewDatabaseWizard.h
    wizard/NewDatabaseWizardPage.h
    wizard/NewDatabaseWizardPageSetup.h
    wizard/NewDatabaseWizardPageEncryption.h
    wizard/NewDatabaseWizardPageMasterKey.h
)

# UI files
set(gui_FORMS
    MainWindow.ui
    SettingsWidget.ui
    entry/EditEntryWidget.ui
    group/EditGroupWidget.ui
    KeePass1OpenWidget.ui
    PasswordGeneratorWidget.ui
    WelcomeWidget.ui
    DatabaseOpenWidget.ui
    csvImport/CsvImportWidget.ui
    keys/KeyFileEditWidget.ui
    keys/PasswordEditWidget.ui
    keys/ChallengeResponseEditWidget.ui
    keys/FingerprintEditWidget.ui
    dbsettings/DatabaseSettingsWidgetGeneral.ui
    dbsettings/DatabaseSettingsWidgetEncryption.ui
    dbsettings/DatabaseSettingsWidgetBrowserIntegration.ui
    dbsettings/DatabaseSettingsWidgetMaintenance.ui
    dbsettings/DatabaseSettingsWidgetRemote.ui
    reports/ReportsWidget.ui
    reports/ReportsWidgetHealthcheck.ui
    reports/ReportsWidgetStatistics.ui
    reports/ReportsWidgetBrowserStatistics.ui
    reports/ReportsWidgetHibp.ui
    wizard/NewDatabaseWizardPageSetup.ui
    wizard/NewDatabaseWizardPageEncryption.ui
    wizard/NewDatabaseWizardPageMasterKey.ui
)

# Resources
set(gui_RESOURCES
    ${CMAKE_SOURCE_DIR}/share/icons/icons.qrc
    ${CMAKE_SOURCE_DIR}/share/welcomescreen/welcomescreen.qrc
)

# Generate Qt MOC files
qt5_wrap_cpp(gui_MOC_SOURCES ${gui_MOC_SOURCES})

# Generate UI files
qt5_wrap_ui(gui_FORMS ${gui_FORMS})

# Generate resource files
qt5_add_resources(gui_RESOURCES_RCC ${gui_RESOURCES})

# Create GUI library
add_library(gui STATIC
    ${gui_SOURCES}
    ${gui_MOC_SOURCES}
    ${gui_FORMS}
    ${gui_RESOURCES_RCC}
)

# Include directories
target_include_directories(gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${BOTAN_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${ARGON2_INCLUDE_DIRS}
)

# Add optional include directories
if(QR_ENCODE_FOUND)
    target_include_directories(gui PUBLIC ${QR_ENCODE_INCLUDE_DIRS})
endif()

if(FINGERPRINT_FOUND)
    target_include_directories(gui PUBLIC ${FINGERPRINT_INCLUDE_DIRS})
endif()

# Link libraries
target_link_libraries(gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    ${BOTAN_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${ARGON2_LIBRARIES}
)

# Add optional libraries
if(QR_ENCODE_FOUND)
    target_link_libraries(gui ${QR_ENCODE_LIBRARIES})
endif()

if(FINGERPRINT_FOUND)
    target_link_libraries(gui ${FINGERPRINT_LIBRARIES})
endif()

# Compile definitions
target_compile_definitions(gui PRIVATE
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

# Add optional compile definitions
if(QR_ENCODE_FOUND)
    target_compile_definitions(gui PRIVATE -DHAVE_QRCODE)
endif()

if(FINGERPRINT_FOUND)
    target_compile_definitions(gui PRIVATE -DHAVE_FINGERPRINT)
endif()