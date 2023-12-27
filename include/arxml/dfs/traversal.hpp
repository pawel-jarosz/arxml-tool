//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml/dfs/callbacks.hpp>
#include <arxml/elements.hpp>

namespace arxml::dfs {

    void traverse_model(model::IAutosarModel& root, TraversalCallback& callback);
    void traverse_model(model::IAutosarPackages& model_unit, TraversalCallback& callback);
    void traverse_model(model::IAutosarPackage& element, TraversalCallback& callback);
    void traverse_model(model::IModelEntry& model_unit, TraversalCallback& callback);
    void traverse_model(model::IAutosarElements& model_unit, TraversalCallback& callback);
    void traverse_model(model::IAutosarElement& element, TraversalCallback& callback);

}
