# Autotype plugin sources
set(autotype_SOURCES
    AutoType.cpp
    AutoTypeSelectDialog.cpp
    AutoTypeSelectView.cpp
    AutoTypeMatchModel.cpp
    AutoTypeMatchView.cpp
)

set(autotype_MOC_SOURCES
    AutoType.h
    AutoTypeSelectDialog.h
    AutoTypeSelectView.h
    AutoTypeMatchModel.h
    AutoTypeMatchView.h
)

# Generate Qt MOC files
qt5_wrap_cpp(autotype_MOC_SOURCES ${autotype_MOC_SOURCES})

# Create autotype library
add_library(autotype STATIC ${autotype_SOURCES} ${autotype_MOC_SOURCES})

# Include directories
target_include_directories(autotype PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(autotype
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)