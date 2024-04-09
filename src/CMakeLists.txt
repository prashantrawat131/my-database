cmake_minimum_required(VERSION 3.29.0)

project(my-database VERSION 1.0.0)

# Including the header files so that the compiler can find them
include_directories("${PROJECT_SOURCE_DIR}/../include/")

add_executable(my-database cmd.cpp db.cpp)

# Link the pthreads library
target_link_libraries(my-database)