cmake_minimum_required(VERSION 3.10)

project(MegaMan)

set(SOURCE_DIR src)

set(GLAD_DIR "lib/glad")
set(OPENAL_DIR "lib/openal")
set(GLFW_DIR "lib/glfw")
set(CGLM_DIR "lib/cglm")
set(CJSON_DIR "lib/cjson")
set(STB_IMAGE_DIR "lib/stb_image")
set(STB_VORBIS_DIR "lib/stb_vorbis")

set(EXTERNAL_SOURCES
    ${STB_VORBIS_DIR}/stb_vorbis.c
)

file(GLOB_RECURSE ENEMY_SOURCE_FILES 
    "${SOURCE_DIR}/entity/enemies/*.c"
    "${SOURCE_DIR}/entity/megaman/weapons/*.c"
    "${SOURCE_DIR}/entity/vfx/*.c"
)


file(GLOB SOURCE_FILES 
    "${SOURCE_DIR}/*.c" 
    "${SOURCE_DIR}/*.h"
    "${SOURCE_DIR}/entity/megaman/*.c" 
    "${SOURCE_DIR}/entity/hud/*.c" 
    "${SOURCE_DIR}/entity/vfx/*.c" 
    "${SOURCE_DIR}/entity/enemies/*.c" 
)

list(APPEND SOURCE_FILES ${ENEMY_SOURCE_FILES})


include_directories(
    ${GLFW_DIR}/include 
    ${OPENAL_DIR}/include 
    ${GLAD_DIR}/include  
    ${CGLM_DIR}/include
    ${CJSON_DIR}
    ${STB_IMAGE_DIR}
    ${STB_VORBIS_DIR}
    src/service 
    src/graphics 
    src/entity
    src/entity/megaman
    src/entity/enemies
    src/entity/vfx
    src/entity/hud
    src/memory 
    src/game 
    src/util
    src/pool
    src/scenes
    src/event
    src/audio
)

link_directories(${GLFW_DIR}/lib-mingw-w64 ${OPENAL_DIR})

add_executable(MegaMan ${SOURCE_FILES} ${EXTERNAL_SOURCES})

target_link_libraries(MegaMan glfw3 opengl32 OpenAL32.lib)
