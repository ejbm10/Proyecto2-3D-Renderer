cmake_minimum_required(VERSION 3.28.3)
project(Server C)

set(CMAKE_C_COMPILER mpicc)

add_executable(Server main.c
        RSA/rsa.h
        RSA/rsa.c)
