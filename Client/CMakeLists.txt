cmake_minimum_required(VERSION 3.28.3)
project(Client C)

# Find necessary packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

# Set the C standard
set(CMAKE_C_STANDARD 11)

add_executable(Client main.c
        RSA/rsa.h
        RSA/rsa.c
)

# Link the libraries, including GLFW directly
target_link_libraries(Client ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} glfw m)
