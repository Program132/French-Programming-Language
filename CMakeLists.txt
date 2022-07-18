cmake_minimum_required(VERSION 3.22)
project(French_Programming_Language)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

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
