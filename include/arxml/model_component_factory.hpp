//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include <arxml/model_elements.hpp>

namespace arxml::model {

    class IModelComponentFactory {
    public:
        ~IModelComponentFactory() = default;
        virtual std::unique_ptr<elements::IAutosarRoot> createRoot() const noexcept = 0;
        virtual std::unique_ptr<elements::IModelUnit> createModelUnit(std::string unit_name) const noexcept = 0;
        virtual std::unique_ptr<elements::IAutosarPackages> createPackages() const noexcept = 0;
        virtual std::unique_ptr<elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<IAutosarElements> elements) const noexcept = 0;
        virtual std::unique_ptr<elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<IAutosarPackages> packages) const noexcept = 0;
        virtual std::unique_ptr<elements::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept = 0;
        virtual std::unique_ptr<elements::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept = 0;
        virtual std::unique_ptr<elements::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept = 0;
        virtual std::unique_ptr<elements::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept = 0;
        virtual std::unique_ptr<elements::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept = 0;
    };

    class ModelComponentFactory : public IModelComponentFactory {
    public:
        std::unique_ptr<elements::IAutosarRoot> createRoot() const noexcept override;
        std::unique_ptr<elements::IModelUnit> createModelUnit(std::string unit_name) const noexcept override;
        std::unique_ptr<elements::IAutosarPackages> createPackages() const noexcept override;
        std::unique_ptr<elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<elements::IAutosarElements> elements) const noexcept override;
        std::unique_ptr<elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<elements::IAutosarPackages> packages) const noexcept override;
        std::unique_ptr<elements::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept override;
        std::unique_ptr<elements::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept override;
        std::unique_ptr<elements::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept override;
        std::unique_ptr<elements::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept override;
        std::unique_ptr<elements::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept override;
    };
}
