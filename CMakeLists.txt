cmake_minimum_required(VERSION 3.16)

project(tosu-overlay-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DPROJECT_VERSION=${CMAKE_PROJECT_VERSION})
add_definitions(-DPROJECT_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_definitions(-DPROJECT_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_definitions(-DPROJECT_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})
add_definitions(-DPROJECT_NAME=${CMAKE_PROJECT_NAME})

find_package(Qt6 REQUIRED COMPONENTS Core DBus Widgets WebEngineWidgets)
find_package(LayerShellQt REQUIRED)

set(PROJECT_SOURCES main.cpp
    resources.qrc
    overlay.h overlay.cpp
    tosuwebview.h tosuwebview.cpp
    systemtray.h systemtray.cpp
    cli.h cli.cpp
    ipc.h ipc.cpp
)

configure_file(version.h.in ${CMAKE_BINARY_DIR}/generated/version.h)
include_directories(${CMAKE_BINARY_DIR}/generated/)

qt_add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::DBus Qt6::Widgets Qt6::WebEngineWidgets LayerShellQt::Interface)

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
