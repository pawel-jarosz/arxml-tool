//
// Created by Paweł Jarosz on 10.11.2023.
//

#include "arxml/utilities/model_component_factory.hpp"

#include "model_elements_impl.hpp"

namespace arxml::utilities::parser {
    std::unique_ptr<model::elements::IAutosarRoot> ModelComponentFactory::createRoot() const noexcept {
        std::unique_ptr<model::elements::IAutosarRoot> result(new model::elements::AutosarRoot);
        return result;
    }

    std::unique_ptr<model::elements::IModelUnit> ModelComponentFactory::createModelUnit(std::string unit_name) const noexcept {
        return std::unique_ptr<model::elements::IModelUnit>(new model::elements::AutosarModelUnit(unit_name));
    }

    std::unique_ptr<model::elements::IAutosarPackages> ModelComponentFactory::createPackages() const noexcept {
        return std::unique_ptr<model::elements::IAutosarPackages>(new model::elements::AutosarPackages());
    }

    std::unique_ptr<model::elements::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<model::elements::IAutosarElements> elements) const noexcept {
        return std::unique_ptr<model::elements::IAutosarPackage>(new model::elements::AutosarPackage(std::move(name), std::move(elements)));
    }

    std::unique_ptr<model::elements::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<model::elements::IAutosarPackages> packages) const noexcept {
        return std::unique_ptr<model::elements::IAutosarPackage>(new model::elements::AutosarPackage(std::move(name), std::move(packages)));
    }

    std::unique_ptr<model::elements::IAutosarElements> ModelComponentFactory::createElements() const noexcept {
        return std::unique_ptr<model::elements::IAutosarElements>(new model::elements::AutosarElements());
    }

    std::unique_ptr<model::elements::ICompositeAutosarElement> ModelComponentFactory::createCompositeElement(std::string tag) const noexcept {
        return std::unique_ptr<model::elements::ICompositeAutosarElement>(new model::elements::CompositeAutosarElement(std::move(tag)));
    }

    std::unique_ptr<model::elements::INamedAutosarElement>
    ModelComponentFactory::createNamedCompositeElement(std::string tag, std::string name) const noexcept {
        return std::unique_ptr<model::elements::INamedAutosarElement>(new model::elements::NamedAutosarElement(std::move(tag), std::move(name)));
    }

    std::unique_ptr<model::elements::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, double value) const noexcept {
        return std::unique_ptr<model::elements::ISimpleAutosarElement>(new model::elements::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<model::elements::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, int value) const noexcept {
        return std::unique_ptr<model::elements::ISimpleAutosarElement>(new model::elements::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<model::elements::ISimpleAutosarElement> ModelComponentFactory::createStringElement(std::string tag, std::string value) const noexcept {
        return std::unique_ptr<model::elements::ISimpleAutosarElement>(new model::elements::StringAutosarElement(std::move(tag), std::move(value)));
    }

}
