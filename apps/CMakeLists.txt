add_executable(arxml_tool main.cpp program_selector.cpp subprograms/program_selector_builder.cpp
                          subprograms/dump_tree_subprogram.cpp)
target_link_libraries(arxml_tool arxml)
message(STATUS "${CMAKE_SOURCE_DIR}/apps/includes")
include_directories(${CMAKE_SOURCE_DIR}/apps/includes)
