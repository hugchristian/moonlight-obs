# Tests for Moonlight OBS Plugin

# Enable testing
enable_testing()

# Basic compilation test - ensures source files compile
add_executable(test_compilation
    test_compilation.c
)

# Link against the same libraries as the main plugin
target_include_directories(test_compilation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Add test
add_test(NAME test_compilation COMMAND test_compilation)
