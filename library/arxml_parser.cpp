//
// Created by Paweł Jarosz on 10.11.2023.
//

#include "arxml/utilities/arxml_parser.hpp"

#include <cassert>

#include <boost/lexical_cast.hpp>

#include <tinyxml2.h>

namespace arxml::utilities::parser {

    namespace {
        enum class ValueType {
            FLOATING,
            INTEGER,
            STRING
        };

        struct ParsedValue {
            ValueType type;
            std::variant<double, int, std::string> value;

            explicit ParsedValue(double value) : type{ValueType::FLOATING}, value{value} {}
            explicit ParsedValue(int value) : type{ValueType::INTEGER}, value{value} {}
            explicit ParsedValue(std::string value) : type{ValueType::STRING}, value{std::move(value)} {}
        };

        static ParsedValue parseElementValue(std::string value) {
            try {
                auto double_value = boost::lexical_cast<double>(value);
                auto integer_value = boost::lexical_cast<int>(value);
                if (double_value == static_cast<double>(integer_value)) {
                    return ParsedValue(integer_value);
                }
                else {
                    return ParsedValue{double_value};
                }
            }
            catch (const boost::bad_lexical_cast& e) {
                return ParsedValue(std::move(value));
            }
        }
    }

    class ParserLogic;
    class ModelEntryParser;
    class PackageParser;
    class PackagesParser;
    class ElementsParser;
    class NamedElementParser;
    class CompositeElementParser;
    class SimpleElementParser;

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

    class ModelEntryParser : public ParserLogic {
    public:
        ModelEntryParser(IModelComponentFactory& element_factory, std::string filename, std::string xmlns,
                         std::string xmlns_xsi, std::string xmlns_schema_location)
                : ParserLogic(element_factory)
                , m_filename{std::move(filename)}
                , m_xmlns{std::move(xmlns)}
                , m_xmlns_xsi{std::move(xmlns_xsi)}
                , m_xmlns_schema_location{std::move(xmlns_schema_location)}
                , m_product{nullptr} {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::IModelEntry> getModelUnit() { return std::move(m_product); }

    private:
        std::string m_filename;
        std::string m_xmlns;
        std::string m_xmlns_xsi;
        std::string m_xmlns_schema_location;
        std::unique_ptr<model::IModelEntry> m_product;
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

    class CompositeElementParser : public ParserLogic {
    public:
        explicit CompositeElementParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::ICompositeAutosarElement> getCompositeElement() { return std::move(m_element); }

    private:
        std::unique_ptr<model::ICompositeAutosarElement> m_element;
    };

    class SimpleElementParser : public ParserLogic {
    public:
        explicit SimpleElementParser(IModelComponentFactory& element_factory)
                : ParserLogic(element_factory)
        {

        }

        void parse(tinyxml2::XMLElement* element) override;
        std::unique_ptr<model::ISimpleAutosarElement> getSimpleElement() { return std::move(m_element); }

    private:
        std::unique_ptr<model::ISimpleAutosarElement> m_element;
    };

    void ModelEntryParser::parse(tinyxml2::XMLElement* element) {
        assert(element->Name() == std::string("AR-PACKAGES"));
        m_product = getComponentFactory().createModelEntry(m_filename, m_xmlns, m_xmlns_xsi, m_xmlns_schema_location);
        auto it = element->FirstChildElement("AR-PACKAGE");
        while (it != nullptr) {
            PackageParser parser{getComponentFactory()};
            parser.parse(it);
            auto result = parser.getPackage();
            assert(result != nullptr);
            m_product->addPackage(std::move(result));
            it = it->NextSiblingElement();
        }
    }

    void PackagesParser::parse(tinyxml2::XMLElement* element) {
        m_packages = getComponentFactory().createPackages();
        assert(element->Name() == std::string("AR-PACKAGES"));
        auto it = element->FirstChildElement("AR-PACKAGE");
        while (it != nullptr) {
            PackageParser parser{getComponentFactory()};
            parser.parse(it);
            m_packages->addPackage(parser.getPackage());
            it = it->NextSiblingElement();
        }
    }

    void PackageParser::parse(tinyxml2::XMLElement* element) {
        assert(element->Name() == std::string("AR-PACKAGE"));
        auto short_name = element->FirstChildElement("SHORT-NAME");
        assert(short_name->NextSiblingElement("SHORT-NAME") == nullptr);

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
                it = it->NextSiblingElement();
                continue;
            }
            if (not it->GetText() and it->FirstChildElement("SHORT-NAME")) {
                NamedElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getNamedElement());
            }
            else if (not it->GetText()) {
                CompositeElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getCompositeElement());
            }
            else {
                SimpleElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getSimpleElement());
            }
            it = it->NextSiblingElement();
        }
    }

    void CompositeElementParser::parse(tinyxml2::XMLElement* element) {
        m_element = getComponentFactory().createCompositeElement(element->Name());

        auto it = element->FirstChildElement();
        while (it != nullptr) {
            if (not it->GetText() and it->FirstChildElement("SHORT-NAME")) {
                NamedElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getNamedElement());
            }
            else if (not it->GetText()) {
                CompositeElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getCompositeElement());
            }
            else {
                SimpleElementParser parser(getComponentFactory());
                parser.parse(it);
                m_element->addSubElement(parser.getSimpleElement());
            }
            it = it->NextSiblingElement();
        }
    }

    void SimpleElementParser::parse(tinyxml2::XMLElement* element) {
        auto tag_name = element->Name();
        std::string value{element->GetText()};

        auto parsing_value = parseElementValue(value);
        switch(parsing_value.type) {
            case ValueType::FLOATING: {
                m_element = getComponentFactory().createNumberElement(tag_name, std::get<double>(parsing_value.value));
                break;
            }
            case ValueType::INTEGER: {
                m_element = getComponentFactory().createNumberElement(tag_name, std::get<int>(parsing_value.value));
                break;
            }
            default:
                m_element = getComponentFactory().createStringElement(tag_name, std::move(value));
        }
        auto attribute_it = element->FirstAttribute();
        while(attribute_it != nullptr) {
            m_element->addAttribute(attribute_it->Name(), attribute_it->Value());
            attribute_it = attribute_it->Next();
        }
    }


    void ArxmlModelBuilder::addEntryFromSource(const std::string& unit_name, utilities::io::IInputSource& source) {
        if (not m_root) {
            m_root = std::move(m_element_factory.createRoot());
        }

        tinyxml2::XMLDocument xml;
        xml.Parse(source.getContent().c_str());

        // TODO: Check if all attributes are available
        assert(xml.RootElement()->Attribute("xmlns") != nullptr
               and xml.RootElement()->Attribute("xmlns:xsi") != nullptr
               and xml.RootElement()->Attribute("xsi:schemaLocation") != nullptr);

        ModelEntryParser model_unit_parser(m_element_factory, unit_name, xml.RootElement()->Attribute("xmlns"),
                                          xml.RootElement()->Attribute("xmlns:xsi"),
                                          xml.RootElement()->Attribute("xsi:schemaLocation"));
        assert(xml.RootElement() != nullptr);
        assert(xml.RootElement()->FirstChildElement("AR-PACKAGES") != nullptr);
        assert(xml.RootElement()->FirstChildElement("AR-PACKAGES")->NextSiblingElement() == nullptr);
        model_unit_parser.parse(xml.RootElement()->FirstChildElement("AR-PACKAGES"));
        m_root->registerModelEntry(unit_name, std::move(model_unit_parser.getModelUnit()));
    }

}
