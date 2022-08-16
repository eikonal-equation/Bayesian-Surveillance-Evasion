file(GLOB_RECURSE cpp_files ./*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(main ${cpp_files})

target_link_libraries(main lib${PROJECT_NAME}.dylib)