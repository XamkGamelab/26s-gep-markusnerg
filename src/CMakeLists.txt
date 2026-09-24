add_executable(GEP26S
    main.cpp
    game.cpp
    game.hpp
    inputmanager.cpp
    inputmanager.hpp
)

target_include_directories(GEP26S PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GEP26S PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

add_custom_target(AssetCopy
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${ASSETS_INPUT_DIRECTORY}"
        $<TARGET_FILE_DIR:GEP26S>/assets
    COMMENT "Copying assets to build directory: $<TARGET_FILE_DIR:GEP26S>/assets"
)

add_dependencies(GEP26S AssetCopy)

if(WIN32)
    add_custom_command(TARGET GEP26S POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:GEP26S>
            $<TARGET_FILE_DIR:GEP26S>
        COMMAND_EXPAND_LISTS
    )
endif()
