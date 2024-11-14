cmake_minimum_required(VERSION 3.29)
project(Client C)

# Find necessary packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Add the executable with all required source files
add_executable(Client
        Renderer/Renderer_alg.c
        Renderer/stb_image.h
        Renderer/Renderer_alg.h
        Renderer/STL_file_handler.c
        Renderer/STL_file_handler.h
        Renderer/STL_to_h_converter.c
        Renderer/STL_to_h_converter.h
)

# Link the libraries, including GLFW directly
target_link_libraries(Client ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} glfw m)
