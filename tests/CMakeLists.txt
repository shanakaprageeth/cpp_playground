# CMakeLists.txt for tests directory
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/tests)

file(GLOB cpp_files RELATIVE
     "${CMAKE_CURRENT_SOURCE_DIR}"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

add_executable(test_runner ${cpp_files})

# Link Google Test libraries
target_link_libraries(test_runner gtest gmock ${libraries} pthread)

set_target_properties(test_runner
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests"
)
