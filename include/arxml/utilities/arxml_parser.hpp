//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include "model_component_factory.hpp"
#include "arxml/model_elements.hpp"
#include "arxml/utilities/input_source.hpp"

namespace arxml::utilities::parser {

    class ArxmlModelBuilder {
    public:
        explicit ArxmlModelBuilder(IModelComponentFactory& element_factory)
        : m_element_factory{element_factory}
        , m_root{}
        {

        }

        void addModelUnitFromSource(const std::string& unit_name, io::IInputSource& source);

        std::unique_ptr<model::IAutosarRoot> build() {
            return std::move(m_root);
        }

    private:
        IModelComponentFactory& m_element_factory;
        std::unique_ptr<model::IAutosarRoot> m_root;
    };

}