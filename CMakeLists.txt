cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(PROJECT_NAME godot_engine)
project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_DEFAULT 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_compile_options(-Wall)

file(GLOB_RECURSE SOURCE_FILES_ENGINE
    "core/*.h" "core/*.hpp" "core/*.c" "core/*.cpp"
    "core/error/*.h" "core/error/*.hpp" "core/error/*.c" "core/error/*.cpp"
    "core/io/*.h" "core/io/*.hpp" "core/io/*.c" "core/io/*.cpp"
    "core/object/*.h" "core/object/*.hpp" "core/object/*.c" "core/object/*.cpp"
    "core/os/*.h" "core/os/*.hpp" "core/os/*.c" "core/os/*.cpp"
    "core/string/*.h" "core/string/*.hpp" "core/string/*.c" "core/string/*.cpp"
    "core/templates/*.h" "core/templates/*.hpp" "core/templates/*.c" "core/templates/*.cpp"
    "documentations/*.*"
    "drivers/*.*"
    "editor/*.*"
    "main/*.*"
    "misc/*.*"
    "modules/*.*"
    "platform/*.*"
    "scene/*.*"
    "servers/*.*"
    "tests/*.*"
    "thirdparty/*.*"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES_ENGINE} ./main.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
)
