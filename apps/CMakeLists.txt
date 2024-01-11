add_executable(arxml_tool main.cpp program_selector.cpp  project_file_parser.cpp
                          subprograms/program_selector_builder.cpp
                          subprograms/dump_tree_subprogram.cpp subprograms/finder_subprogram.cpp
                          subprograms/structure_dump_subprogram.cpp)
target_link_libraries(arxml_tool arxml yaml-cpp::yaml-cpp)
message(STATUS "${CMAKE_SOURCE_DIR}/apps/includes")
include_directories(${CMAKE_SOURCE_DIR}/apps/includes)
