file(GLOB_RECURSE cpp_files "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME}_shared SHARED ${cpp_files})

set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}_shared 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)