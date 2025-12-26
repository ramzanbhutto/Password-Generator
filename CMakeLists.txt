cmake_minimum_required(VERSION 3.10)
project(SecurePasswordManager)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL library
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(password_manager main.cpp)

# Link OpenSSL libraries
target_link_libraries(password_manager OpenSSL::SSL OpenSSL::Crypto)

# Optional: Add compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(password_manager PRIVATE -Wall -Wextra -pedantic)
endif()
