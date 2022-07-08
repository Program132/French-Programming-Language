cmake_minimum_required(VERSION 3.22)
project(French_Programming_Language)

set(CMAKE_CXX_STANDARD 20)

add_executable(French_Programming_Language
        src/TokenBuilding.cpp
        src/TokenBuilding.h
        src/Parser.cpp
        src/Parser.h
        src/FonctionDefinition.cpp
        src/FonctionDefinition.h
        src/TypeManager.cpp
        src/TypeManager.h
        src/VariableDefinition.cpp
        src/VariableDefinition.h
        main.cpp)
