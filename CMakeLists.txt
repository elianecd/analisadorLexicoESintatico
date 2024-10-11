cmake_minimum_required(VERSION 3.10)
project(analisadorSintatico)

set(CMAKE_C_STANDARD 11)

add_executable(analisadorSintatico main.c lexer.c parser.c syntaxTree.c tokens.h parser.h syntaxTree.h)
