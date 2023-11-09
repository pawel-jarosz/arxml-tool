//
// Created by Paweł Jarosz on 09.11.2023.
//

#pragma once

#include <optional>
#include <variant>
#include <vector>
#include <memory>
#include <map>

namespace arxml {

    enum class EntryType {
        AUTOSAR,
        PACKAGES,
        PACKAGE,
        ELEMENTS,
        GENERIC_ELEMENT,
        COMPOSITE_ELEMENT,
        NAMED_ELEMENT,
        INTEGER_ELEMENT,
        FLOATING_ELEMENT,
        STRING_ELEMENT
    };

    enum class CollectionType {
        ElementsCollection,
        PackagesCollection
    };

    class IAutosarModel;
    class IModelUnit;
    class IAutosarPackages;
    class IAutosarPackage;
    class IAutosarElements;
    class IAutosarElement;
    class INamedAutosarElement;

    class IAutosarModel {
    public:
        virtual ~IAutosarModel() = default;
        [[nodiscard]] virtual EntryType getType() const noexcept = 0;
    };

    class IAutosarRoot : public IAutosarModel {
    public:
        using ModelUnitMap = std::map<std::string, std::unique_ptr<IModelUnit>>;

        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::AUTOSAR; }
        virtual IModelUnit& getModelUnitByName(const std::string& entry_name) = 0;
        virtual void registerModelUnit(const std::string& entry_name, std::unique_ptr<IModelUnit> package) = 0;
        [[nodiscard]] virtual ModelUnitMap& getModelUnits() noexcept = 0;
    };

    class AutosarRoot : public IAutosarRoot {
    public:
        void registerModelUnit(const std::string& entry_name, std::unique_ptr<IModelUnit> package) override { m_packages[entry_name] = std::move(package);}
        IModelUnit& getModelUnitByName(const std::string& entry_name) override { return *m_packages.at(entry_name); }
        [[nodiscard]] ModelUnitMap& getModelUnits() noexcept override { return m_packages; }
    private:
        ModelUnitMap m_packages;
    };

    class IAutosarPackages : public IAutosarModel {
    public:
        using PackagePtr = std::unique_ptr<IAutosarPackage>;
        using PackagePtrContainer = std::vector<PackagePtr>;

        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::PACKAGES; }

        virtual PackagePtrContainer& getPackages() noexcept = 0;
        virtual void addPackage(PackagePtr package) noexcept = 0;
    };

    class AutosarPackages : public IAutosarPackages {
    public:
        PackagePtrContainer& getPackages() noexcept override { return m_container; }
        void addPackage(PackagePtr package) noexcept override { m_container.emplace_back(std::move(package)); }
    private:
        PackagePtrContainer m_container;
    };

    class IModelUnit : public IAutosarPackages {
    public:
        virtual std::string getModelUnitName() = 0;
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

    class IAutosarPackage : public IAutosarModel {
    public:
        [[nodiscard]] virtual const std::string& getName() const noexcept = 0;
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::PACKAGE; }
        [[nodiscard]] virtual CollectionType getCollectionType() const noexcept = 0;
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

    class IAutosarElements : public IAutosarModel {
    public:
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::ELEMENTS; }
        virtual std::vector<std::unique_ptr<INamedAutosarElement>>& getElements() noexcept = 0;
        virtual void addElement(std::unique_ptr<INamedAutosarElement> element) noexcept = 0;
    };

    class AutosarElements : public IAutosarElements {
    public:
        [[nodiscard]] std::vector<std::unique_ptr<INamedAutosarElement>>& getElements() noexcept override { return m_elements; }
        void addElement(std::unique_ptr<INamedAutosarElement> element) noexcept override { m_elements.emplace_back(std::move(element)); }
    private:
        std::vector<std::unique_ptr<INamedAutosarElement>> m_elements;
    };

    class IAutosarElement : public IAutosarModel {
    public:
        [[nodiscard]] virtual std::string getTag() const noexcept = 0;
        [[nodiscard]] virtual bool isComposite() const noexcept { return false; }
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::GENERIC_ELEMENT; }
    };

    class ISimpleAutosarElement : public IAutosarElement {
    public:
        using AttributePair = std::pair<std::string, std::string>;

        virtual void addAttribute(std::string name, std::string value) noexcept = 0;
        virtual std::optional<std::string> getAttribute(std::string_view name) = 0;
        virtual const std::vector<AttributePair>& getAttributes() const noexcept = 0;
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

    class NumberAutosarElement : public AbstractSimpleAutosarElement {
    public:
        explicit NumberAutosarElement(std::string tag, double value)
                : AbstractSimpleAutosarElement(std::move(tag))
                , m_floating_value{value}
                , m_type{EntryType::FLOATING_ELEMENT} {}

        explicit NumberAutosarElement(std::string tag, int value)
                : AbstractSimpleAutosarElement(std::move(tag))
                , m_integer_value{value}
                , m_type{EntryType::INTEGER_ELEMENT} {}

        int getInteger() { return m_integer_value; }
        double getFloating() { return m_floating_value; }
        EntryType getType() const noexcept override { return m_type; }
    private:
        union {
            double m_floating_value;
            int m_integer_value;
        };
        EntryType m_type;
    };

    class StringAutosarElement : public AbstractSimpleAutosarElement {
    public:
        explicit StringAutosarElement(std::string tag, std::string text)
                : AbstractSimpleAutosarElement(std::move(tag))
                , m_text{std::move(text)} {}

        EntryType getType() const noexcept override { return EntryType::STRING_ELEMENT; }
        const std::string& getName() { return m_text; }
    private:
        std::string m_text;
    };

    class ICompositeAutosarElement : public IAutosarElement {
    public:
        virtual void addSubElement(std::unique_ptr<IAutosarElement> element) noexcept = 0;
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::COMPOSITE_ELEMENT; }
        virtual std::vector<std::unique_ptr<IAutosarElement>>& getSubElements() noexcept = 0;
        [[nodiscard]] bool isComposite() const noexcept override { return true; }
    };

    class INamedAutosarElement : public ICompositeAutosarElement {
    public:
        [[nodiscard]] virtual std::string getName() const noexcept = 0;
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
