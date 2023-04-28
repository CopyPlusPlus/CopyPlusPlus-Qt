cmake_minimum_required(VERSION 3.5)

# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg/scripts/buildsystems/vcpkg.cmake"
# CACHE STRING "Vcpkg toolchain file")

# This need to be set before project() call
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")

project(CopyPlusPlus VERSION 0.1.0 LANGUAGES CXX)

message(STATUS "CMAKE_OSX_DEPLOYMENT_TARGET: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Before find: QT_DIR: ${QT_DIR}")

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

message(STATUS "After find: QT_DIR: ${QT_DIR}")

set(COMPONENT_SOURCES
    src/components/mykeysequenceedit.cpp
    src/components/mykeysequenceedit.h
)

set(UI_SOURCES
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/settingswindow.cpp
    src/ui/settingswindow.h
    src/ui/settingswindow.ui
)

set(UTILS_SOURCES
    src/utils/language.cpp
    src/utils/language.h
    src/utils/textprocessor.cpp
    src/utils/textprocessor.h
)

set(SOURCES src/main.cpp
    ${COMPONENT_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
)

set(TS_FILES translations/CopyPlusPlus_en_US.ts)

set(PROJECT_SOURCES
    ${SOURCES}
    ${TS_FILES}
)

add_executable(CopyPlusPlus
    ${PROJECT_SOURCES}
)

# Include directories
set(MY_DIRECTORY "src" "src/components" "src/ui" "src/utils")
target_include_directories(CopyPlusPlus PUBLIC ${MY_DIRECTORY})

# Add and link QHotkey
add_subdirectory("3rdparty/QHotkey")
target_link_libraries(CopyPlusPlus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QHotkey::QHotkey)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/translations ${TS_FILES})

set_target_properties(CopyPlusPlus PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME CopyPlusPlus
    MACOSX_BUNDLE_GUI_IDENTIFIER wykj.CopyPlusPlus
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CopyPlusPlus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
