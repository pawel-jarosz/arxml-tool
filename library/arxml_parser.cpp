//
// Created by Paweł Jarosz on 10.11.2023.
//

#include <arxml/arxml_parser.hpp>

#include <iostream>
#include <iomanip>

#include <tinyxml2.h>

namespace arxml {

    static void parse_model(tinyxml2::XMLElement* element, int indent = 0) {
        std::cout << std::setw(indent) << " " << "=> " << element->Name() << std::endl;
        auto it = element->FirstChildElement();
        while (it != 0) {
            parse_model(it, indent + 4);
            it = it->NextSiblingElement();
        }

    }

    void ArxmlModelBuilder::addModelUnitFromSource(const std::string& unit_name, io::IModelSource& source) {
        if (not m_root) {
            m_root = std::move(m_element_factory.createRoot());
        }
        auto model_unit = m_element_factory.createModelUnit(std::string(unit_name));
        tinyxml2::XMLDocument xml;
        xml.Parse(source.getContent().c_str());
        parse_model(xml.RootElement());
        m_root->registerModelUnit(unit_name, std::move(model_unit));
    }

}
