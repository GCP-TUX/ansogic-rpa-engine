cmake_minimum_required(VERSION 3.16)

project(ansogic-rpa-engine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar los módulos necesarios de Qt (Core y Gui para manejo de pantallas e hilos)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Indicar los únicos dos archivos de tu PoC apuntando a la carpeta src
add_executable(ansogic-rpa-engine
    src/main.cpp
    src/DesktopAutomationEngine.h
    src/MainWindow.cpp
)

# Enlazar las librerías de Qt
target_link_libraries(ansogic-rpa-engine PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
