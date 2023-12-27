//
// Created by Paweł Jarosz on 10.11.2023.
//

#include "arxml/utilities/model_component_factory.hpp"

#include "model_elements_impl.hpp"

namespace arxml::utilities::parser {
    std::unique_ptr<model::IAutosarModel> ModelComponentFactory::createRoot() const noexcept {
        std::unique_ptr<model::IAutosarModel> result(new model::AutosarModel);
        return result;
    }

    std::unique_ptr<model::IModelEntry> ModelComponentFactory::createModelUnit(std::string unit_name, std::string xmlns, std::string xmlns_xsi, std::string xmlns_schema_location) const noexcept {
        return std::unique_ptr<model::IModelEntry>(new model::AutosarModelEntry(std::move(unit_name), std::move(xmlns), std::move(xmlns_xsi), std::move(xmlns_schema_location)));
    }

    std::unique_ptr<model::IAutosarPackages> ModelComponentFactory::createPackages() const noexcept {
        return std::unique_ptr<model::IAutosarPackages>(new model::AutosarPackages());
    }

    std::unique_ptr<model::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<model::IAutosarElements> elements) const noexcept {
        return std::unique_ptr<model::IAutosarPackage>(new model::AutosarPackage(std::move(name), std::move(elements)));
    }

    std::unique_ptr<model::IAutosarPackage>
    ModelComponentFactory::createPackage(std::string name, std::unique_ptr<model::IAutosarPackages> packages) const noexcept {
        return std::unique_ptr<model::IAutosarPackage>(new model::AutosarPackage(std::move(name), std::move(packages)));
    }

    std::unique_ptr<model::IAutosarElements> ModelComponentFactory::createElements() const noexcept {
        return std::unique_ptr<model::IAutosarElements>(new model::AutosarElements());
    }

    std::unique_ptr<model::ICompositeAutosarElement> ModelComponentFactory::createCompositeElement(std::string tag) const noexcept {
        return std::unique_ptr<model::ICompositeAutosarElement>(new model::CompositeAutosarElement(std::move(tag)));
    }

    std::unique_ptr<model::INamedAutosarElement>
    ModelComponentFactory::createNamedCompositeElement(std::string tag, std::string name) const noexcept {
        return std::unique_ptr<model::INamedAutosarElement>(new model::NamedAutosarElement(std::move(tag), std::move(name)));
    }

    std::unique_ptr<model::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, double value) const noexcept {
        return std::unique_ptr<model::ISimpleAutosarElement>(new model::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<model::ISimpleAutosarElement> ModelComponentFactory::createNumberElement(std::string tag, int value) const noexcept {
        return std::unique_ptr<model::ISimpleAutosarElement>(new model::NumberAutosarElement(std::move(tag), value));
    }

    std::unique_ptr<model::ISimpleAutosarElement> ModelComponentFactory::createStringElement(std::string tag, std::string value) const noexcept {
        return std::unique_ptr<model::ISimpleAutosarElement>(new model::StringAutosarElement(std::move(tag), std::move(value)));
    }

}
