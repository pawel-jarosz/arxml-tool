//
// Created by Paweł Jarosz on 29.12.2023.
//

#include <arxml/helpers/finders.hpp>
#include <arxml/printer.hpp>
#include <sstream>

namespace arxml::helpers {

    void ElementByTagFinder::visit(model::IAutosarPackage& package) {
        m_path.push_back(package.getName());
    }

    void ElementByTagFinder::visit(model::IAutosarElements& elements) {
        for (auto& it: elements.getElements()) {
            if (it->getTag() == m_expected_tag) {
                std::stringstream ss;
                for (auto& part: m_path) {
                    ss << "/" << part;
                }
                ss << "/" << it->getName();
                m_result.emplace(ss.str(), std::ref(*it));
            }
        }
    }

    void ElementByTagFinder::close(model::IAutosarPackage& package) {
        m_path.pop_back();
    }

    void ElementByIdFinder::visit(model::IAutosarElement& element) {
        if (element.getType() == arxml::model::EntryType::NAMED_ELEMENT) {
            m_path.push_back(dynamic_cast<model::INamedAutosarElement&>(element).getName());
            std::stringstream ss;
            std::for_each(m_path.begin(), m_path.end(), [&](const std::string& value) {
                ss << "/" << value;
            });

            if (m_expected_id == ss.str()) {
                m_result.push_back(dynamic_cast<model::INamedAutosarElement&>(element));
            }
        }
    }

    void ElementByIdFinder::visit(model::IAutosarPackage& package) {
        m_path.push_back(package.getName());
    }

    void ElementByIdFinder::close(model::IAutosarPackage &package) {
        m_path.pop_back();
    }

    void ElementByIdFinder::close(model::IAutosarElement& element) {
        if (element.getType() == arxml::model::EntryType::NAMED_ELEMENT) {
            m_path.pop_back();
        }
    }

    void ElementByReferenceFinder::visit(model::IAutosarElement& element) {
        using namespace arxml::model;

        switch (element.getType()) {
            case model::EntryType::NAMED_ELEMENT: {
                m_path.push_back(dynamic_cast<INamedAutosarElement &>(element).getName());
                break;
            }
            case model::EntryType::STRING_ELEMENT: {
                auto& string_element = dynamic_cast<IStringAutosarElement&>(element);
                if (string_element.getAttribute("DEST").has_value()
                    and string_element.getText() == m_expected_id) {
                    std::stringstream ss;
                    std::for_each(m_path.begin(),
                                  m_path.end(),
                                  [&](const std::string& p) {
                                      ss << "/" << p;
                                  });
                    m_result.push_back(ss.str());
                }
            }
            default: break;
        }
    }

    void ElementByReferenceFinder::visit(model::IAutosarPackage& package) {
        m_path.push_back(package.getName());
    }

    void ElementByReferenceFinder::close(model::IAutosarPackage &package) {
        m_path.pop_back();
    }

    void ElementByReferenceFinder::close(model::IAutosarElement& element) {
        if (element.getType() == arxml::model::EntryType::NAMED_ELEMENT) {
            m_path.pop_back();
        }
    }
}