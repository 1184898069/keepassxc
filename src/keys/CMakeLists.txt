# Keys module sources
set(keys_SOURCES
    CompositeKey.cpp
    CompositeKey_p.h
    FileKey.cpp
    PasswordKey.cpp
    ChallengeResponseKey.cpp
    drivers/ChallengeResponseKeyInterface.cpp
    drivers/YubiKeyInterface.cpp
    drivers/YubiKeyInterfacePCSC.cpp
    drivers/YubiKeyInterfaceUSB.cpp
    drivers/FingerprintKey.cpp
)

set(keys_MOC_SOURCES
    CompositeKey.h
    FileKey.h
    PasswordKey.h
    ChallengeResponseKey.h
    drivers/ChallengeResponseKeyInterface.h
    drivers/YubiKeyInterface.h
    drivers/YubiKeyInterfacePCSC.h
    drivers/YubiKeyInterfaceUSB.h
    drivers/FingerprintKey.h
)

# Generate Qt MOC files
qt5_wrap_cpp(keys_MOC_SOURCES ${keys_MOC_SOURCES})

# Create keys library
add_library(keys STATIC ${keys_SOURCES} ${keys_MOC_SOURCES})

# Include directories
target_include_directories(keys PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${BOTAN_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${ARGON2_INCLUDE_DIRS}
)

# Add optional include directories
if(PCSC_FOUND)
    target_include_directories(keys PUBLIC ${PCSC_INCLUDE_DIRS})
endif()

if(FINGERPRINT_FOUND)
    target_include_directories(keys PUBLIC ${FINGERPRINT_INCLUDE_DIRS})
endif()

# Link libraries
target_link_libraries(keys
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${BOTAN_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${ARGON2_LIBRARIES}
)

# Add optional libraries
if(PCSC_FOUND)
    target_link_libraries(keys ${PCSC_LIBRARIES})
endif()

if(FINGERPRINT_FOUND)
    target_link_libraries(keys ${FINGERPRINT_LIBRARIES})
endif()

# Compile definitions
target_compile_definitions(keys PRIVATE
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

# Add optional compile definitions
if(PCSC_FOUND)
    target_compile_definitions(keys PRIVATE -DHAVE_PCSC)
endif()

if(FINGERPRINT_FOUND)
    target_compile_definitions(keys PRIVATE -DHAVE_FINGERPRINT)
endif()