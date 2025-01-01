cmake_minimum_required(VERSION 3.10)

project(MegaMan)

set(SOURCE_DIR src)

set(GLAD_DIR "C:/Libraries/glad")
set(GLFW_DIR "C:/Libraries/glfw")
set(CGLM_DIR "C:/Libraries/cglm")
set(STB_IMAGE_DIR "C:/Libraries/stb_image")

file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.h")

include_directories(
    ${GLFW_DIR}/include 
    ${GLAD_DIR}/include  
    ${CGLM_DIR}/include
    ${STB_IMAGE_DIR}
    src/service 
    src/graphics 
    src/entity
    src/memory 
    src/game 
    src/util
    src/pool
    src/scenes
)

link_directories(${GLFW_DIR}/lib-mingw-w64)

add_executable(MegaMan ${SOURCE_FILES})

target_link_libraries(MegaMan glfw3 opengl32)
