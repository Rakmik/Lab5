cmake_minimum_required(VERSION 3.20)
project(SpaceShipSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

add_executable(SpaceShipSimulation main.cpp)

target_link_libraries(SpaceShipSimulation PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)