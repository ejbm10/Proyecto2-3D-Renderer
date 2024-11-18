cmake_minimum_required(VERSION 3.28.3)
project(Server C)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)


include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

set(CMAKE_C_COMPILER mpicc)

add_executable(Server main.c
        RSA/rsa.h
        RSA/rsa.c
        Renderer/Renderer_alg.c
        Renderer/Renderer_alg.h
        Renderer/stb_image.h
        Renderer/STL_file_handler.c
        Renderer/STL_file_handler.h
        Renderer/STL_to_h_converter.c
        Renderer/STL_to_h_converter.h
        Validation/syntax.h
        Validation/syntax.c
        biblioteca.h
        biblioteca.c
)

target_link_libraries(Server ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} glfw m)