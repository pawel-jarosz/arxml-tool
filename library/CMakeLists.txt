add_library(arxml model_elements_impl.cpp model_component_factory.cpp arxml_parser.cpp input_source.cpp
        project.cpp traversal.cpp printer.cpp parser_facade.cpp finders.cpp)
target_link_libraries(arxml PRIVATE ${TINYXML2_LIBRARIES})