cmake_minimum_required(VERSION 3.26.3)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

project(deadYokaiInstaller LANGUAGES CXX C)

if(WIN32)
  set(CMAKE_EXECUTABLE_SUFFIX ".exe")
endif()


include("${PROJECT_SOURCE_DIR}/CMakeRC.cmake")

cmrc_add_resource_library(
    installer-resources

    ALIAS installer::rc
    NAMESPACE installer

    res/e-Ukraine-Light.otf
    res/text.vs
    res/text.fs
    )

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)



include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(installer src/gl.c src/shader.cpp installer.cpp)

target_include_directories(installer PRIVATE ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(installer ${FREETYPE_LIBRARIES} ${OPENGL_gl_LIBRARY} glfw installer::rc)

file(GLOB sources CONFIGURE_DEPENDS *.cpp *.h)
target_sources(installer PUBLIC ${sources})


