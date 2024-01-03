//
// Created by Paweł Jarosz on 29.12.2023.
//

#include <arxml/helpers/finders.hpp>
#include <sstream>

namespace arxml::helpers {

    namespace {
        std::string toString(const std::vector<std::string>& vec) {
            std::stringstream ss;
            std::for_each(vec.begin(), vec.end(), [&](const std::string& str) {
                ss << '/' << str;
            });
            return ss.str();
        }

        bool is_root_element(const std::vector<std::string>& current_element,
                             const std::vector<std::string>& full_path) {
            if (current_element.size() > full_path.size()) {
                return false;
            }
            int equal_counter = 0;
            while (current_element[equal_counter] == full_path[equal_counter]) {
                ++equal_counter;
            }
            return (equal_counter == current_element.size());
        }

        std::vector<std::string> splitPath(std::string path) {
            // TODO: Simplify
            std::vector<std::string> result;
            for(auto& it: path) {
                if (it == '/') {
                    it = ' ';
                }
            }
            std::stringstream ss(path);
            std::string temp;
            while(ss) {
                ss >> temp;
                result.emplace_back(std::move(temp));
            }
            if (result[result.size() - 1] == "") {
                result.pop_back();
            }

            return result;
        }
    }

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

    RootElementFinder::RootElementFinder(std::optional<std::pair<std::string, model::INamedAutosarElement&>>& result,
                                         const std::string& full_path)
    : m_result{result}
    , m_full_path{splitPath(full_path)}
    {

    }


    void RootElementFinder::visit(model::IAutosarPackage& package) {
        m_path.push_back(package.getName());
    }

    void RootElementFinder::visit(model::IAutosarElements& elements) {
        for (auto& it: elements.getElements()) {
            m_path.push_back(it->getName());
            if (is_root_element(m_path, m_full_path)) {
                std::stringstream ss;
                std::for_each(m_path.begin(), m_path.end(), [&](const std::string& str) {
                    ss << '/' << str;
                });

                m_result = std::make_pair(ss.str(), std::ref(*it));
            }
            m_path.pop_back();
        }
    }

    void RootElementFinder::close(model::IAutosarPackage& package) {
        m_path.pop_back();
    }

}