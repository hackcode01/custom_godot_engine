cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(PROJECT_NAME godot_engine)
project(${PROJECT_NAME} VERSION 0.0.1)

file(GLOB_RECURSE SOURCE_FILES
    "*.h" ".hpp" ".c" ".cpp"
)

add_executable(${PROJECT_NAME} ./main.cpp)
