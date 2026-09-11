add_executable(GEP26S
    main.cpp
    game.cpp
    game.hpp
)

# Otsikkotiedostot löytyvät tästä hakemistosta.
target_include_directories(GEP26S PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Linkitys vendor-hakemistossa haettuun SDL3:een.
target_link_libraries(GEP26S PRIVATE SDL3::SDL3)

# Windowsilla SDL3.dll on kopioitava exen viereen, muuten
# ohjelma ei käynnisty ("SDL3.dll was not found").
if(WIN32)
    add_custom_command(TARGET GEP26S POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:GEP26S>
            $<TARGET_FILE_DIR:GEP26S>
        COMMAND_EXPAND_LISTS
        COMMENT "Kopioidaan SDL3:n DLL:t suoritettavan tiedoston viereen"
    )
endif()
