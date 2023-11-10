//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include <arxml/model_component_factory.hpp>
#include <arxml/model_elements.hpp>
#include <arxml/model_source.hpp>

namespace arxml {

    class ArxmlModelBuilder {
    public:
        explicit ArxmlModelBuilder(model::IModelComponentFactory& element_factory)
        : m_element_factory{element_factory}
        , m_root{}
        {

        }

        void addModelUnitFromSource(const std::string& unit_name, io::IModelSource& source);

        std::unique_ptr<model::elements::IAutosarRoot> build() {
            return std::move(m_root);
        }

    private:
        model::IModelComponentFactory& m_element_factory;
        std::unique_ptr<model::elements::IAutosarRoot> m_root;
    };

}