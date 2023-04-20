cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(CopyPlusPlus-QtWidget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES translations/CopyPlusPlus-QtWidget_en_US.ts)

file(GLOB SOURCES src/*.cpp src/*.h src/*.ui)

set(PROJECT_SOURCES
    ${SOURCES}
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CopyPlusPlus-QtWidget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Define target properties for Android with Qt 6 as:
    # set_property(TARGET CopyPlusPlus-QtWidget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(CopyPlusPlus-QtWidget SHARED
            ${PROJECT_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CopyPlusPlus-QtWidget
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_subdirectory("3rdparty/QHotkey")

target_link_libraries(CopyPlusPlus-QtWidget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QHotkey::QHotkey)

set(MY_DIRECTORY "src" "src/components" "src/ui" "src/utils")

target_include_directories(CopyPlusPlus-QtWidget PUBLIC ${MY_DIRECTORY})

set_target_properties(CopyPlusPlus-QtWidget PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CopyPlusPlus-QtWidget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CopyPlusPlus-QtWidget)
endif()
