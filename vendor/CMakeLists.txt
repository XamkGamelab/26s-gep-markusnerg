include(FetchContent)

# SDL:n omat asetukset on asetettava ENNEN MakeAvailable-kutsua.
# FORCE tarvitaan, koska nämä ovat cache-muuttujia.
set(SDL_SHARED       ON  CACHE BOOL "" FORCE)
set(SDL_STATIC       OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS        OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL      OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.16
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

# Lataa ja lisää SDL:n oma CMake-projekti osaksi buildia.
# Tämän jälkeen käytettävissä on target SDL3::SDL3.
FetchContent_MakeAvailable(SDL3)
