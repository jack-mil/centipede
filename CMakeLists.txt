# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab1 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)


# Add the executable
add_executable(centipede code/main.cpp code/Engine.cpp code/TextureManager.cpp code/Player.cpp code/Laser.cpp)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Link the executable to the libraries in the lib directory
target_link_libraries(centipede PUBLIC sfml-graphics sfml-system sfml-window)

set_target_properties(
    centipede PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
# Copy DLLs using file(COPY ...)
# file(COPY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/bin/x64/openal32.dll
#      DESTINATION "${COMMON_OUTPUT_DIR}/bin")
# file(COPY ${PROJECT_SOURCE_DIR}/fonts
#      DESTINATION "${COMMON_OUTPUT_DIR}/bin")
file(COPY ${PROJECT_SOURCE_DIR}/graphics
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
#  file(COPY "${PROJECT_SOURCE_DIR}/low res"
#  DESTINATION "${COMMON_OUTPUT_DIR}/bin")
#  file(COPY ${PROJECT_SOURCE_DIR}/sound
#  DESTINATION "${COMMON_OUTPUT_DIR}/bin")