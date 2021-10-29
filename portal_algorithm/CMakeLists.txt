cmake_minimum_required(VERSION 3.18)

project(PGR_examples_03)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
SET(CMAKE_CXX_STANDARD 17)

include_directories(../libs/glfw-3.1.2/include/)

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
add_subdirectory(../libs/SDL2-2.0.16 sdl2_build)
add_subdirectory(../libs/geGL gegl_build)
add_subdirectory(../libs/glm-0.9.9.8 glm_build)

set(SOURCES
  src/main.cpp
  src/common/shader.cpp
  src/common/shader.hpp
  src/common/objloader.cpp
  src/common/objloader.hpp
  )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
  SDL2-static
  SDL2::SDL2main
  geGL::geGL
  glm)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_custom_target(run "./${PROJECT_NAME}")
