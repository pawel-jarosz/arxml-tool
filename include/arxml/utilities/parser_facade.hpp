//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml/utilities/model_component_factory.hpp>
#include <arxml/utilities/arxml_parser.hpp>
#include <arxml/utilities/input_source.hpp>
#include <arxml/elements.hpp>

namespace arxml::utilities {

    class IParserFacade {
    public:
        ~IParserFacade() = default;
        virtual void parse(const std::string& filename) = 0;
        virtual std::unique_ptr<model::IAutosarModel> getModel() = 0;
    };

    class DefaultParserFacade : public IParserFacade {
    public:
        DefaultParserFacade()
        : m_factory()
        , m_parser(m_factory)
        {

        }

        void parse(const std::string& filename) override;
        std::unique_ptr<model::IAutosarModel> getModel() override { return m_parser.build(); }
    private:
        utilities::parser::ModelComponentFactory m_factory;
        utilities::parser::ArxmlFileParser m_parser;
    };

}
