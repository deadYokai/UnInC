cmake_minimum_required(VERSION 3.26.3)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_GENERATOR "Ninja")

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

if(MSVC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif()

project(deadYokaiInstaller LANGUAGES CXX C)

include("${PROJECT_SOURCE_DIR}/CMakeRC.cmake")

cmrc_add_resource_library(
    installer-resources

    ALIAS installer::rc
    NAMESPACE installer

    res/e-Ukraine-Light.ttf
    res/text.vs
    res/text.fs
    )

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

if(WIN32)
  find_package(unofficial-brotli CONFIG REQUIRED) # From VCPKG
  find_package(PNG REQUIRED)
  find_package(BZip2 REQUIRED)
  set(CMAKE_EXECUTABLE_SUFFIX ".exe")
  set(OTHER_PKGS PNG::PNG BZip2::BZip2 unofficial::brotli::brotlidec glm::glm)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(installer src/gl.c src/shader.cpp installer.cpp)

if(MSVC)
  set_property(TARGET installer APPEND_STRING PROPERTY COMPILE_FLAGS "/utf-8 /Zc:wchar_t")
endif()

target_include_directories(installer PRIVATE ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

target_link_libraries(installer ${OPENGL_gl_LIBRARY} glfw ${OTHER_PKGS} ${FREETYPE_LIBRARIES} installer::rc)

file(GLOB sources CONFIGURE_DEPENDS *.cpp *.h)
target_sources(installer PUBLIC ${sources})


