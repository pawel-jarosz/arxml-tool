//
// Created by Paweł Jarosz on 10.11.2023.
//

#include <arxml/model_component_factory.hpp>

namespace arxml::model {
    std::unique_ptr<elements::IAutosarRoot> ModelComponentFactory::createRoot() const noexcept {
        std::unique_ptr<elements::IAutosarRoot> result(new elements::AutosarRoot);
        return result;
    }

    std::unique_ptr<elements::IModelUnit> ModelComponentFactory::createModelUnit(std::string unit_name) const noexcept {
        return std::unique_ptr<elements::IModelUnit>(new elements::AutosarModelUnit(unit_name));
    }

    std::unique_ptr<elements::IAutosarPackages> ModelComponentFactory::createPackages() const noexcept {
        return std::unique_ptr<elements::IAutosarPackages>(new elements::AutosarPackages());
    }

    std::unique_ptr<elements::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<elements::IAutosarElements> elements) const noexcept {
        return std::unique_ptr<elements::IAutosarPackage>(new elements::AutosarPackage(std::move(name), std::move(elements)));
    }

    std::unique_ptr<elements::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<elements::IAutosarPackages> packages) const noexcept {
        return std::unique_ptr<elements::IAutosarPackage>(new elements::AutosarPackage(std::move(name), std::move(packages)));
    }

    std::unique_ptr<elements::ICompositeAutosarElement> ModelComponentFactory::createCompositeElement(std::string tag) const noexcept {
        return std::unique_ptr<elements::ICompositeAutosarElement>(new elements::CompositeAutosarElement(std::move(tag)));
    }

    std::unique_ptr<elements::INamedAutosarElement>
    ModelComponentFactory::createNamedCompositeElement(std::string tag, std::string name) const noexcept {
        return std::unique_ptr<elements::INamedAutosarElement>(new elements::NamedAutosarElement(std::move(tag), std::move(name)));
    }

    std::unique_ptr<elements::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, double value) const noexcept {
        return std::unique_ptr<elements::ISimpleAutosarElement>(new elements::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<elements::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, int value) const noexcept {
        return std::unique_ptr<elements::ISimpleAutosarElement>(new elements::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<elements::ISimpleAutosarElement> ModelComponentFactory::createStringElement(std::string tag, std::string value) const noexcept {
        return std::unique_ptr<elements::ISimpleAutosarElement>(new elements::StringAutosarElement(std::move(tag), std::move(value)));
    }

}
