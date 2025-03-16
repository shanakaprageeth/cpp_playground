# CMakeLists.txt for src directory
include_directories(${CMAKE_SOURCE_DIR}/include)

set(libraries ${libraries})

add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server PRIVATE ${libraries})
target_link_libraries(client PRIVATE ${libraries})

set_target_properties(server
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples"
)
set_target_properties(client
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples"
)