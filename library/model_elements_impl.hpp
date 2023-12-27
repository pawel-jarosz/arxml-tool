//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include <arxml/model_elements.hpp>

namespace arxml::model{

    class AutosarRoot : public IAutosarRoot {
    public:
        void registerModelUnit(const std::string& entry_name, std::unique_ptr<IModelUnit> package) override { m_packages[entry_name] = std::move(package);}
        IModelUnit& getModelUnitByName(const std::string& entry_name) override { return *m_packages.at(entry_name); }
        [[nodiscard]] ModelUnitMap& getModelUnits() noexcept override { return m_packages; }
    private:
        ModelUnitMap m_packages;
    };

    class AutosarPackages : public IAutosarPackages {
    public:
        PackagePtrContainer& getPackages() noexcept override { return m_container; }
        void addPackage(PackagePtr package) noexcept override { m_container.emplace_back(std::move(package)); }
    private:
        PackagePtrContainer m_container;
    };

    class AutosarModelUnit : virtual public IModelUnit {
    public:
        explicit AutosarModelUnit(std::string source)
                : m_source{std::move(source)}
                , m_packages{} {
        }

        std::string getModelUnitName() override { return m_source; }

        PackagePtrContainer& getPackages() noexcept override { return m_packages.getPackages(); }
        void addPackage(PackagePtr package) noexcept override { m_packages.addPackage(std::move(package)); }

    private:
        std::string m_source;
        AutosarPackages m_packages;
    };

    class AutosarPackage : public IAutosarPackage {
    public:
        AutosarPackage(std::string name, std::unique_ptr<IAutosarElements> elements)
                : m_name{std::move(name)}, m_elements{std::move(elements)}
                , m_collection_type{CollectionType::ElementsCollection} {}

        AutosarPackage(std::string name, std::unique_ptr<IAutosarPackages> packages)
                : m_name{std::move(name)}, m_elements{std::move(packages)}
                , m_collection_type{CollectionType::PackagesCollection} {}

        [[nodiscard]] CollectionType getCollectionType() const noexcept override { return m_collection_type; }
        [[nodiscard]] const std::string& getName() const noexcept override { return m_name; }
        IAutosarElements& getElements() { return *std::get<std::unique_ptr<IAutosarElements>>(m_elements); }
        IAutosarPackages& getPackages() { return *std::get<std::unique_ptr<IAutosarPackages>>(m_elements); }

    private:
        std::string m_name;
        std::variant<std::unique_ptr<IAutosarElements>, std::unique_ptr<IAutosarPackages>> m_elements;
        CollectionType m_collection_type;
    };

    class AutosarElements : public IAutosarElements {
    public:
        [[nodiscard]] std::vector<std::unique_ptr<INamedAutosarElement>>& getElements() noexcept override { return m_elements; }
        void addElement(std::unique_ptr<INamedAutosarElement> element) noexcept override { m_elements.emplace_back(std::move(element)); }
    private:
        std::vector<std::unique_ptr<INamedAutosarElement>> m_elements;
    };

    class AbstractSimpleAutosarElement : public ISimpleAutosarElement {
    public:
        explicit AbstractSimpleAutosarElement(std::string tag) : m_tag{std::move(tag)} {}
        std::string getTag() const noexcept override { return m_tag; }
        void addAttribute(std::string name, std::string value) noexcept override { m_attributes.emplace_back(std::move(name), std::move(value)); }
        [[nodiscard]] const std::vector<AttributePair>& getAttributes() const noexcept override { return m_attributes; }
        std::optional<std::string> getAttribute(std::string_view name) override;
    private:
        std::string m_tag;
        std::vector<AttributePair> m_attributes;
    };

    class NumberAutosarElement : public INumberAutosarElement {
    public:
        explicit NumberAutosarElement(std::string tag, double value)
                : m_element(std::move(tag))
                , m_floating_value{value}
                , m_type{EntryType::FLOATING_ELEMENT} {}

        explicit NumberAutosarElement(std::string tag, int value)
                : m_element(std::move(tag))
                , m_integer_value{value}
                , m_type{EntryType::INTEGER_ELEMENT} {}

        // TODO: Add error handling due to invalid number type
        int getInteger() override { return m_integer_value; }
        double getFloating() override { return m_floating_value; }
        EntryType getType() const noexcept override { return m_type; }

        void addAttribute(std::string name, std::string value) noexcept override {
            m_element.addAttribute(std::move(name),
                                   std::move(value));
        }
        std::optional<std::string> getAttribute(std::string_view name) override { return m_element.getAttribute(name); }
        const std::vector<AttributePair>& getAttributes() const noexcept override { return m_element.getAttributes(); }
        std::string getTag() const noexcept override { return m_element.getTag(); }
    private:
        AbstractSimpleAutosarElement m_element;
        union {
            double m_floating_value;
            int m_integer_value;
        };
        EntryType m_type;
    };

    class StringAutosarElement : public IStringAutosarElement {
    public:
        explicit StringAutosarElement(std::string tag, std::string text)
                : m_element(std::move(tag))
                , m_text{std::move(text)} {}

        EntryType getType() const noexcept override { return EntryType::STRING_ELEMENT; }
        const std::string& getText() { return m_text; }

        void addAttribute(std::string name, std::string value) noexcept override {
            m_element.addAttribute(std::move(name),
                                   std::move(value));
        }
        std::optional<std::string> getAttribute(std::string_view name) override { return m_element.getAttribute(name); }
        const std::vector<AttributePair>& getAttributes() const noexcept override { return m_element.getAttributes(); }
        std::string getTag() const noexcept override { return m_element.getTag(); }

    private:
        AbstractSimpleAutosarElement m_element;
        std::string m_text;
    };

    class CompositeAutosarElement : public ICompositeAutosarElement {
    public:
        explicit CompositeAutosarElement(std::string tag)
                : m_tag{std::move(tag)} {}

        std::string getTag() const noexcept override { return m_tag; }
        void addSubElement(std::unique_ptr<IAutosarElement> element) noexcept override { m_subelements.emplace_back(std::move(element)); }
        std::vector<std::unique_ptr<IAutosarElement>>& getSubElements() noexcept override { return m_subelements; }
    private:
        std::string m_tag;
        std::vector<std::unique_ptr<IAutosarElement>> m_subelements;
    };

    class NamedAutosarElement : public virtual INamedAutosarElement {
    public:
        NamedAutosarElement(std::string tag, std::string name)
                : m_composite{std::move(tag)}
                , m_name{std::move(name)} {}

        EntryType getType() const noexcept override { return EntryType::NAMED_ELEMENT; }
        std::string getName() const noexcept override { return m_name; }

        void addSubElement(std::unique_ptr<IAutosarElement> element) noexcept override { m_composite.addSubElement(std::move(element)); }
        std::vector<std::unique_ptr<IAutosarElement>> & getSubElements() noexcept override { return m_composite.getSubElements(); }
        std::string getTag() const noexcept override { return m_composite.getTag(); }
    private:
        CompositeAutosarElement m_composite;
        std::string m_name;
    };

}
