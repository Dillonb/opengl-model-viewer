add_custom_command(OUTPUT shader_vert.h
        COMMAND xxd -n shader_vert_src -i ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert shader_vert.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert)

add_custom_command(OUTPUT shader_frag.h
        COMMAND xxd -n shader_frag_src -i ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag shader_frag.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag)


add_executable(mv main.cpp window.cpp window.h render.cpp render.h shader_vert.h shader_frag.h)
target_include_directories(mv PUBLIC ${SDL2_INCLUDE_DIR} ../external/include ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mv PUBLIC ${SDL2_LIBRARY})