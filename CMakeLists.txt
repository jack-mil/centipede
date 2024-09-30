# Minimum CMake version required
cmake_minimum_required(VERSION 3.28)

# Project name and version
project(centipede LANGUAGES CXX VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)

# Configure SFML options (use static linking)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
# option(SFML_BUILD_AUDIO FALSE)
set(SFML_STATIC_LIBRARIES TRUE)

# Fetch the SFML source code dependencies from github
include(FetchContent)
FetchContent_Declare(SFML
GIT_REPOSITORY https://github.com/SFML/SFML.git
GIT_TAG 2.6.x
GIT_SHALLOW ON
EXCLUDE_FROM_ALL
SYSTEM)
FetchContent_MakeAvailable(SFML)


# Add the executable
add_executable(${PROJECT_NAME}
                src/main.cpp
                src/Engine.cpp
                src/TextureManager.cpp
                src/Player.cpp
                src/Laser.cpp
                src/Mushrooms.cpp
                src/Spider.cpp
                src/Centipede.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Enable warning and errors
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wsign-conversion -Wconversion -Wextra -Werror -pedantic -pedantic-errors)

# ensure assets are copied to build directory
# need a better solution in the code
# to solve paths relative to cwd problem
file(COPY ${PROJECT_SOURCE_DIR}/graphics
     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
