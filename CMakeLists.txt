cmake_minimum_required(VERSION 3.10)
project(WinApiProject)

# 设置默认输出目录（可选）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 自动查找所有包含 CMakeLists.txt 的子目录
file(GLOB_RECURSE SUBDIRS RELATIVE ${CMAKE_SOURCE_DIR} */CMakeLists.txt)

foreach(subdir ${SUBDIRS})
    get_filename_component(dir ${subdir} DIRECTORY)
    add_subdirectory(${dir})
endforeach()
