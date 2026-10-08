cmake_minimum_required(VERSION 3.16)
project(pda_simulator VERSION 1.0 LANGUAGES CXX)

# C++17 obligatorio y sin extensiones del compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Todos los .cc de src/ forman el ejecutable
file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cc)

add_executable(pda ${SOURCES})

# Las cabeceras están en include/
target_include_directories(pda PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(pda PRIVATE -Wall -Wextra -pedantic)
