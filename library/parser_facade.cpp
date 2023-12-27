//
// Created by Paweł Jarosz on 27.12.2023.
//

#include <arxml/utilities/parser_facade.hpp>

namespace arxml::utilities {
    void DefaultParserFacade::parse(const std::string& filename) {
        io::FileSource source{filename};
        m_parser.parseSource(filename, source);
    }
}
