//
// Created by Paweł Jarosz on 09.11.2023.
//

#pragma once

#include <optional>
#include <variant>
#include <vector>
#include <memory>
#include <map>

namespace arxml::model::elements {

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
    class ISimpleAutosarElement;
    class ICompositeAutosarElement;
    class IStringAutosarElement;
    class INumberAutosarElement;

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

    class IAutosarPackages : public IAutosarModel {
    public:
        using PackagePtr = std::unique_ptr<IAutosarPackage>;
        using PackagePtrContainer = std::vector<PackagePtr>;

        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::PACKAGES; }

        virtual PackagePtrContainer& getPackages() noexcept = 0;
        virtual void addPackage(PackagePtr package) noexcept = 0;
    };

    class IModelUnit : public IAutosarPackages {
    public:
        virtual std::string getModelUnitName() = 0;
    };

    class IAutosarPackage : public IAutosarModel {
    public:
        [[nodiscard]] virtual const std::string& getName() const noexcept = 0;
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::PACKAGE; }
        [[nodiscard]] virtual CollectionType getCollectionType() const noexcept = 0;
    };

    class IAutosarElements : public IAutosarModel {
    public:
        [[nodiscard]] EntryType getType() const noexcept override { return EntryType::ELEMENTS; }
        virtual std::vector<std::unique_ptr<INamedAutosarElement>>& getElements() noexcept = 0;
        virtual void addElement(std::unique_ptr<INamedAutosarElement> element) noexcept = 0;
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

    class INumberAutosarElement : public ISimpleAutosarElement {
    public:
        virtual int getInteger() = 0;
        virtual double getFloating() = 0;
    };

    class IStringAutosarElement : public ISimpleAutosarElement {
    public:
        virtual const std::string& getText() = 0;

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
}
