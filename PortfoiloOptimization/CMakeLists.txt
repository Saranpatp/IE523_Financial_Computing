cmake_minimum_required(VERSION 3.23)
project(PortfoiloOptimization)

set(CMAKE_CXX_STANDARD 14)

add_executable(PortfoiloOptimization main.cpp)

find_library(CPP_GUROBI_LIB NAMES gurobi_c++
        PATHS   ${GUROBI_HOME}
        ${GUROBI_HOME}/lib
        )