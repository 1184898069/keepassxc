# Database settings widget sources
set(dbsettings_SOURCES
    DatabaseSettingsWidget.cpp
    DatabaseSettingsWidgetGeneral.cpp
    DatabaseSettingsWidgetEncryption.cpp
    DatabaseSettingsWidgetBrowserIntegration.cpp
    DatabaseSettingsWidgetMaintenance.cpp
    DatabaseSettingsWidgetRemote.cpp
)

set(dbsettings_MOC_SOURCES
    DatabaseSettingsWidget.h
    DatabaseSettingsWidgetGeneral.h
    DatabaseSettingsWidgetEncryption.h
    DatabaseSettingsWidgetBrowserIntegration.h
    DatabaseSettingsWidgetMaintenance.h
    DatabaseSettingsWidgetRemote.h
)

# UI files
set(dbsettings_FORMS
    DatabaseSettingsWidgetGeneral.ui
    DatabaseSettingsWidgetEncryption.ui
    DatabaseSettingsWidgetBrowserIntegration.ui
    DatabaseSettingsWidgetMaintenance.ui
    DatabaseSettingsWidgetRemote.ui
)

# Generate Qt MOC files
qt5_wrap_cpp(dbsettings_MOC_SOURCES ${dbsettings_MOC_SOURCES})

# Generate UI files
qt5_wrap_ui(dbsettings_FORMS ${dbsettings_FORMS})

# Create database settings library
add_library(dbsettings STATIC
    ${dbsettings_SOURCES}
    ${dbsettings_MOC_SOURCES}
    ${dbsettings_FORMS}
)

# Include directories
target_include_directories(dbsettings PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(dbsettings
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# Compile definitions
target_compile_definitions(dbsettings PRIVATE
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)