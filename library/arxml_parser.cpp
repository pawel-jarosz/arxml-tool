//
// Created by Paweł Jarosz on 10.11.2023.
//

#include <arxml/arxml_parser.hpp>

#include <tinyxml2.h>

namespace arxml {

    static void traverse_xml(tinyxml2::XMLElement* element) {

    }

    void ArxmlModelBuilder::addModelUnitFromSource(const std::string& unit_name, io::IModelSource& source) {
        if (not m_root) {
            m_root = std::move(m_element_factory.createRoot());
        }
        auto model_unit = m_element_factory.createModelUnit(std::string(unit_name));
        tinyxml2::XMLDocument xml;
        xml.Parse(source.getContent().c_str());
        m_root->registerModelUnit(unit_name, std::move(model_unit));
    }

}
