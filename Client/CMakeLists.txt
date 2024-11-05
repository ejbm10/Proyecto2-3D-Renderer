cmake_minimum_required(VERSION 3.28.3)
project(Client C)

set(CMAKE_C_STANDARD 11)

add_executable(Client main.c
        RSA/rsa.h
        RSA/rsa.c)