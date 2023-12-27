//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include "arxml/model_elements.hpp"

namespace arxml::utilities::parser {

    class IModelComponentFactory {
    public:
        ~IModelComponentFactory() = default;
        virtual std::unique_ptr<model::elements::IAutosarRoot> createRoot() const noexcept = 0;
        virtual std::unique_ptr<model::elements::IModelUnit> createModelUnit(std::string unit_name) const noexcept = 0;
        virtual std::unique_ptr<model::elements::IAutosarPackages> createPackages() const noexcept = 0;
        virtual std::unique_ptr<model::elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::elements::IAutosarElements> elements) const noexcept = 0;
        virtual std::unique_ptr<model::elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::elements::IAutosarPackages> packages) const noexcept = 0;
        virtual std::unique_ptr<model::elements::IAutosarElements> createElements() const noexcept = 0;
        virtual std::unique_ptr<model::elements::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept = 0;
        virtual std::unique_ptr<model::elements::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept = 0;
        virtual std::unique_ptr<model::elements::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept = 0;
        virtual std::unique_ptr<model::elements::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept = 0;
        virtual std::unique_ptr<model::elements::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept = 0;
    };

    class ModelComponentFactory : public IModelComponentFactory {
    public:
        [[nodiscard]] std::unique_ptr<model::elements::IAutosarRoot> createRoot() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::IModelUnit> createModelUnit(std::string unit_name) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::IAutosarPackages> createPackages() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::elements::IAutosarElements> elements) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::elements::IAutosarPackages> packages) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::IAutosarElements> createElements() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::elements::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept override;
    };
}
