get_filename_component(PROJ_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB SRC_FILES *.cpp)
add_executable(${PROJ_NAME} WIN32 ${SRC_FILES})