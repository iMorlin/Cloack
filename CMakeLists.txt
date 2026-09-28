cmake_minimum_required(VERSION 3.16)

project(Cloack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Отдельное приложение — клиент.
add_executable(Client
    Common/constants.h
    Common/settings.h Common/settings.cpp
    Client/mainClient.cpp
    Client/mainwindowclient.h Client/mainwindowclient.cpp Client/mainwindowclient.ui
    Client/connectionclient.h Client/connectionclient.cpp
    Client/clockwidget.h Client/clockwidget.cpp
)

target_include_directories(Client PRIVATE Common)

target_link_libraries(Client PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# Отдельное приложение — сервер.
add_executable(Server
    Common/constants.h
    Common/settings.h Common/settings.cpp
    Server/mainServer.cpp
    Server/mainwindowserver.h Server/mainwindowserver.cpp Server/mainwindowserver.ui
    Server/connectionserver.h Server/connectionserver.cpp
)

target_include_directories(Server PRIVATE Common)

target_link_libraries(Server PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(MSVC)
    target_compile_options(Client PRIVATE /utf-8)
    target_compile_options(Server PRIVATE /utf-8)
endif()

configure_file(settings.json settings.json COPYONLY)
