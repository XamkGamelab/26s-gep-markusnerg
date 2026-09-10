# Määrittää, minkä CMake-version sääntöjä (policies) noudatetaan.
# Tämän on oltava tiedoston ensimmäinen komento.
cmake_minimum_required(VERSION 3.28)

# Määrittää projektin nimen ja aktivoi kääntäjätarkistukset.
# LANGUAGES CXX estää CMakea etsimästä turhaan C-kääntäjää.
project(GEP26S
    VERSION 0.1.0
    DESCRIPTION "Game Engine Programming 26S baseline"
    LANGUAGES CXX)

# C++20 -standardi, ei hiljaista putoamista vanhempaan versioon.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Luo suoritettavan tiedoston. Ensimmäinen argumentti on target,
# johon SDL3 linkitetään myöhemmin komennolla target_link_libraries.
add_executable(GEP26S main.cpp)
