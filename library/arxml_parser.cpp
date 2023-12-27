//
// Created by Paweł Jarosz on 10.11.2023.
//

#include "arxml/utilities/arxml_parser.hpp"

#include <iostream>
#include <iomanip>

#include <tinyxml2.h>
#include <cassert>

namespace arxml::utilities::parser {

    class ParserLogic;
    class ModelUnitParser;
    class PackageParser;
    class PackagesParser;
    class ElementsParser;
    class NamedElementParser;

    class ParserLogic {
    public:
        explicit ParserLogic(IModelComponentFactory& element_factory)
        : m_element_factory{element_factory}
        {}

        virtual ~ParserLogic() = default;

        IModelComponentFactory& getComponentFactory() { return m_element_factory; }

        virtual void parse(tinyxml2::XMLElement* element) = 0;
    private:
        IModelComponentFactory& m_element_factory;
    };

    class ModelUnitParser : public ParserLogic {
    public:
        ModelUnitParser(IModelComponentFactory& element_factory, std::string filename)
                : ParserLogic(element_factory)
                , m_filename{std::move(filename)}
                , m_product{nullptr} {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::IModelUnit> getModelUnit() { return std::move(m_product); }

    private:
        std::string m_filename;
        std::unique_ptr<model::IModelUnit> m_product;
    };

    class PackagesParser : public ParserLogic {
    public:
        explicit PackagesParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::IAutosarPackages> getPackages() { return std::move(m_packages); }

    private:
        std::unique_ptr<model::IAutosarPackages> m_packages;
    };


    class PackageParser : public ParserLogic {
    public:
        explicit PackageParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::IAutosarPackage> getPackage() { return std::move(m_package); }

    private:
        std::unique_ptr<model::IAutosarPackage> m_package;
    };

    class ElementsParser : public ParserLogic {
    public:
        explicit ElementsParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::IAutosarElements> getElements() { return std::move(m_elements); }

    private:
        std::unique_ptr<model::IAutosarElements> m_elements;
    };

    class NamedElementParser : public ParserLogic {
    public:
        explicit NamedElementParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::INamedAutosarElement> getNamedElement() { return std::move(m_element); }

    private:
        std::unique_ptr<model::INamedAutosarElement> m_element;
    };

    void ModelUnitParser::parse(tinyxml2::XMLElement* element) {
        assert(element->Name() == std::string("AR-PACKAGES"));
        m_product = getComponentFactory().createModelUnit(m_filename);
        auto it = element->FirstChildElement("AR-PACKAGE");
        while (it != nullptr) {
            PackageParser parser{getComponentFactory()};
            m_product->addPackage(parser.getPackage());
            it = it->NextSiblingElement();
        }
    }

    void PackagesParser::parse(tinyxml2::XMLElement* element) {
        m_packages = getComponentFactory().createPackages();
        assert(element->Name() == std::string("AR-PACKAGES"));
        auto it = element->FirstChildElement("AR-PACKAGE");
        while (it != nullptr) {
            PackageParser parser{getComponentFactory()};
            m_packages->addPackage(parser.getPackage());
            it = it->NextSiblingElement();
        }
    }

    void PackageParser::parse(tinyxml2::XMLElement* element) {
        assert(element->Name() == std::string("AR-PACKAGE"));
        auto short_name = element->FirstChildElement("SHORT-NAME");
        assert(short_name->NextSiblingElement() == nullptr);

        auto packages = element->FirstChildElement("AR-PACKAGES");
        auto elements = element->FirstChildElement("ELEMENTS");

        if (packages) {
            assert (packages->NextSiblingElement() == nullptr);
            PackagesParser parser(getComponentFactory());
            parser.parse(packages);
            m_package = getComponentFactory().createPackage(short_name->GetText(), parser.getPackages());
        }
        else if (elements) {
            assert (elements->NextSiblingElement() == nullptr);
            ElementsParser parser(getComponentFactory());
            parser.parse(elements);
            m_package = getComponentFactory().createPackage(short_name->GetText(), parser.getElements());
        }
        else {
            assert(1 == 0); // INVALID BRANCH
        }
    }

    void ElementsParser::parse(tinyxml2::XMLElement *element) {
        assert(element->Name() == std::string("ELEMENTS"));
        m_elements = getComponentFactory().createElements();

        auto it = element->FirstChildElement();
        while (it != nullptr) {
            NamedElementParser named_element_parser(getComponentFactory());
            named_element_parser.parse(it);
            m_elements->addElement(named_element_parser.getNamedElement());
            it = it->NextSiblingElement();
        }
    }

    void NamedElementParser::parse(tinyxml2::XMLElement* element) {
        auto short_name = element->FirstChildElement("SHORT-NAME");
        assert(short_name->NextSiblingElement("SHORT-NAME") == nullptr);
        m_element = getComponentFactory().createNamedCompositeElement(element->Name(), short_name->GetText());

        auto it = element->FirstChildElement();
        while (it != nullptr) {
            if (it->Name() == std::string("SHORT-NAME")) {
                continue;
            }
            if (it->GetText() == nullptr) {
                NamedElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getNamedElement());
            }
            else {

            }
            it = it->NextSiblingElement();
        }
    }


    static void parse_model(tinyxml2::XMLElement* element, int indent = 0) {
        std::cout << std::setw(indent) << " " << "=> " << element->Name();
        if (element->GetText() != nullptr) {
            std::cout << ": " << element->GetText();
        }
        std::cout << std::endl;
        auto it = element->FirstChildElement();
        while (it != nullptr) {
            parse_model(it, indent + 4);
            it = it->NextSiblingElement();
        }
    }


    void ArxmlModelBuilder::addModelUnitFromSource(const std::string& unit_name, utilities::io::IInputSource& source) {
        if (not m_root) {
            m_root = std::move(m_element_factory.createRoot());
        }
        auto model_unit = m_element_factory.createModelUnit(std::string(unit_name));
        tinyxml2::XMLDocument xml;
        xml.Parse(source.getContent().c_str());
        ModelUnitParser model_unit_parser(m_element_factory, unit_name);
        assert(xml.RootElement()->FirstChildElement("AR-PACKAGES")->NextSiblingElement() == nullptr);
        model_unit_parser.parse(xml.RootElement()->FirstChildElement("AR-PACKAGES"));
        //parse_model(xml.RootElement()->FirstChildElement("AR-PACKAGES"));
        m_root->registerModelUnit(unit_name, std::move(model_unit));
    }

}
