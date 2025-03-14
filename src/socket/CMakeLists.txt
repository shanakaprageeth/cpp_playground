include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB cpp_files RELATIVE
     "${CMAKE_CURRENT_SOURCE_DIR}"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)
add_library(socket ${cpp_files})

target_link_libraries(socket
  ${libraries} pthread
)
set(libraries ${libraries} socket)
set(libraries ${libraries} PARENT_SCOPE)
