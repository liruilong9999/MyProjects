# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.12)

# 设置项目名称
project(Reflection)

# C++标准
add_compile_options(/std:c++14)

add_compile_definitions(UNICODE _UNICODE)

# 开启使用文件夹功能，将所有默认目标放入到名为 CMakePredefinedTargets 的文件夹中
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMakePredefinedTargets")

# 设置根目录
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${ROOT_DIR}/build)

# 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/../bin/lib) # .lib and .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/../bin/lib) # .so and .dylib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}/../bin) # .exe and .dll
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR}/../bin/lib) # .lib and .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR}/../bin/lib) # .so and .dylib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR}/../bin) # .exe and .dll

# 添加子目录，子目录下必须有CMakeLists.txt
add_subdirectory(src)