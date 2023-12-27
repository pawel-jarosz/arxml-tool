add_executable(arxml_tool main.cpp program_selector.cpp)
target_link_libraries(arxml_tool)
include_directories(${CMAKE_SOURCE_DIR}/apps/includes)
