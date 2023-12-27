//
// Created by Paweł Jarosz on 10.11.2023.
//

#include "arxml/utilities/input_source.hpp"

#include <sstream>

namespace arxml::utilities::io {
    bool FileSource::open(std::string_view filename) {
        m_input.open(std::string(filename));
        if (m_input.is_open()) {
            m_opened = true;
            return true;
        }
        return false;
    }

    std::string FileSource::getContent() {
        if (not m_opened) {
            return "";
        }
        std::stringstream ss;
        std::string line;
        while (m_input) {
            std::getline(m_input, line);
            ss << line;
        }
        return ss.str();
    }

    bool StringSource::open(std::string_view input) {
        m_content = input;
        return true;
    }

}
