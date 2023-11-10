//
// Created by Paweł Jarosz on 10.11.2023.
//

#include <arxml/arxml_parser.hpp>

namespace arxml {

    void ArxmlModelBuilder::addModelUnitFromSource(const std::string& unit_name, io::IModelSource& source) {
        if (not m_root) {
            m_root = std::move(m_element_factory.createRoot());
        }
        auto model_unit = m_element_factory.createModelUnit(std::string(unit_name));
        m_root->registerModelUnit(unit_name, std::move(model_unit));
    }

}
