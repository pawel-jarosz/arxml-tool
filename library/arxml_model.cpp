//
// Created by Paweł Jarosz on 09.11.2023.
//

#include <arxml/arxml_model.hpp>

namespace arxml::model::elements {

    std::optional<std::string> AbstractSimpleAutosarElement::getAttribute(std::string_view name) {
        auto comparer = [&](const AttributePair& attribute) { return attribute.first == name; };
        const auto found_item = std::find_if(m_attributes.begin(), m_attributes.end(), comparer);
        return (found_item == m_attributes.end() ? std::optional<std::string>() : std::make_optional(found_item->second));
    }

}

