cmake_minimum_required(VERSION 3.10)
project(Password-Generator)

# List source files
set(SOURCES
    main.cpp
)

# List all header files 
set(HEADERS
    json.h
)

# Add executable target
add_executable(password_generator ${SOURCES} ${HEADERS})

# Include current directory for headers 
target_include_directories(password_generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
