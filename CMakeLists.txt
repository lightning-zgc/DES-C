cmake_minimum_required(VERSION 3.6)
project(DES2_0)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c Encrypt.c Encrypt.h require.h DES_Resourse.h FileReader.c FileReader.h)
add_executable(DES2_0 ${SOURCE_FILES} Encrypt.c Encrypt.h require.h DES_Resourse.h FileReader.c FileReader.h)