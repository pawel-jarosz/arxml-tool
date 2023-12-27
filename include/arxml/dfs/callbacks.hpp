//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml/elements.hpp>

namespace arxml::dfs {

    class TraverseModelCallback {
    public:
        virtual ~TraverseModelCallback() = default;
        virtual void visit(model::IAutosarModel& root) {}
        virtual void visit(model::IModelEntry& root) {}
        virtual void visit(model::IAutosarPackages& packages) {}
        virtual void visit(model::IAutosarPackage& package) {}
        virtual void visit(model::IAutosarElements& elements) {}
        virtual void visit(model::IAutosarElement& element) {}

        virtual void close(model::IAutosarModel& root) {}
        virtual void close(model::IModelEntry& root) {}
        virtual void close(model::IAutosarPackages& packages) {}
        virtual void close(model::IAutosarPackage& package) {}
        virtual void close(model::IAutosarElements& elements) {}
        virtual void close(model::IAutosarElement& element) {}
    };
}
