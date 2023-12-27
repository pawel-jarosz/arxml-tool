//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include "model_component_factory.hpp"
#include "arxml/elements.hpp"
#include "arxml/utilities/input_source.hpp"

namespace arxml::utilities::parser {

    class ArxmlFileParser {
    public:
        explicit ArxmlFileParser(IModelComponentFactory& element_factory)
        : m_element_factory{element_factory}
        , m_root{}
        {

        }

        void addEntryFromSource(const std::string& unit_name, utilities::io::IInputSource &source);

        std::unique_ptr<model::IAutosarModel> build() {
            return std::move(m_root);
        }

    private:
        IModelComponentFactory& m_element_factory;
        std::unique_ptr<model::IAutosarModel> m_root;
    };

}