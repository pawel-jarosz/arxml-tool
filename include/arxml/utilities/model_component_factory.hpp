//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include "arxml/model_elements.hpp"

namespace arxml::utilities::parser {

    class IModelComponentFactory {
    public:
        ~IModelComponentFactory() = default;
        virtual std::unique_ptr<model::IAutosarRoot> createRoot() const noexcept = 0;
        virtual std::unique_ptr<model::IModelUnit> createModelUnit(std::string unit_name) const noexcept = 0;
        virtual std::unique_ptr<model::IAutosarPackages> createPackages() const noexcept = 0;
        virtual std::unique_ptr<model::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::IAutosarElements> elements) const noexcept = 0;
        virtual std::unique_ptr<model::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::IAutosarPackages> packages) const noexcept = 0;
        virtual std::unique_ptr<model::IAutosarElements> createElements() const noexcept = 0;
        virtual std::unique_ptr<model::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept = 0;
        virtual std::unique_ptr<model::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept = 0;
        virtual std::unique_ptr<model::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept = 0;
        virtual std::unique_ptr<model::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept = 0;
        virtual std::unique_ptr<model::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept = 0;
    };

    class ModelComponentFactory : public IModelComponentFactory {
    public:
        [[nodiscard]] std::unique_ptr<model::IAutosarRoot> createRoot() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::IModelUnit> createModelUnit(std::string unit_name) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::IAutosarPackages> createPackages() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::IAutosarElements> elements) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::IAutosarPackage> createPackage(std::string name, std::unique_ptr<model::IAutosarPackages> packages) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::IAutosarElements> createElements() const noexcept override;
        [[nodiscard]] std::unique_ptr<model::ICompositeAutosarElement> createCompositeElement(std::string tag) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::INamedAutosarElement> createNamedCompositeElement(std::string tag, std::string name) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::ISimpleAutosarElement> createNumberElement(std::string tag, double value) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::ISimpleAutosarElement> createNumberElement(std::string tag, int value) const noexcept override;
        [[nodiscard]] std::unique_ptr<model::ISimpleAutosarElement> createStringElement(std::string tag, std::string value) const noexcept override;
    };
}
