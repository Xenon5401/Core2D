cmake_minimum_required(VERSION 3.13)
project(Core2D VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Par défaut Release si aucun type précisé
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Sources : recherche récursive dans src/
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

if(NOT SOURCE_FILES)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directory (si vous avez un dossier include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Demander les caractéristiques du langage
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Trouver et lier SDL2 via pkg-config (Linux)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})