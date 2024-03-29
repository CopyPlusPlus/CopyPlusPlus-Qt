cmake_minimum_required(VERSION 3.5)

# Set this at VSCode settings.json, for the convenience of diabling it at Github Actions
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# This need to be set before project() call
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OSX deployment version")

project(CopyPlusPlus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable C4819 warning
if(MSVC)
    add_compile_options(/wd4819)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "Qt5_DIR: ${Qt5_DIR}")

set(COMPONENT_SOURCES
    src/components/mykeysequenceedit.cpp
    src/components/mykeysequenceedit.h

    # src/components/mytoggle.cpp
    # src/components/mytoggle.h
)

set(UI_SOURCES
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/settingswindow.cpp
    src/ui/settingswindow.h
    src/ui/settingswindow.ui
    src/components/toggle.h
    src/components/toggle.cpp
    src/components/toggle.ui
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

# Translations
set(TS_FILES translations/CopyPlusPlus_zh_CN.ts)
# Set qm file output path. Reference url: https://github.com/barry-ran/QtScrcpy/blob/dev/QtScrcpy/res/i18n/CMakeLists.txt
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/translations")
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/src ${TS_FILES})

# Add executable and link Qt5 libraries
add_executable(CopyPlusPlus ${SOURCES} ${QM_FILES} resources.qrc)
target_link_libraries(CopyPlusPlus PRIVATE Qt5::Widgets)

# Include directories
set(MY_DIRECTORY "src" "src/components" "src/ui" "src/utils")
target_include_directories(CopyPlusPlus PUBLIC ${MY_DIRECTORY})

# Add and link QHotkey
add_subdirectory("3rdparty/QHotkey")
target_link_libraries(CopyPlusPlus PRIVATE QHotkey::QHotkey)

# Add and link Material Widgets
add_subdirectory("3rdparty/qt-material-widgets/components")
target_link_libraries(CopyPlusPlus PRIVATE components)

# add_subdirectory("3rdparty/qt-material-widgets/examples")
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
