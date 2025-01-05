cmake_minimum_required(VERSION 3.10)

project(MegaMan)

set(SOURCE_DIR src)

set(GLAD_DIR "lib/glad")
set(GLFW_DIR "lib/glfw")
set(CGLM_DIR "lib/cglm")
set(CJSON_DIR "lib/cjson")
set(STB_IMAGE_DIR "lib/stb_image")

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h")

include_directories(
    ${GLFW_DIR}/include 
    ${GLAD_DIR}/include  
    ${CGLM_DIR}/include
    ${CJSON_DIR}
    ${STB_IMAGE_DIR}
    src/service 
    src/graphics 
    src/entity
    src/memory 
    src/game 
    src/util
    src/pool
    src/scenes
    src/event
)

link_directories(${GLFW_DIR}/lib-mingw-w64)

add_executable(MegaMan ${SOURCE_FILES})

target_link_libraries(MegaMan glfw3 opengl32)
