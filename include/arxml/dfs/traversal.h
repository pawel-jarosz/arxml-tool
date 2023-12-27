//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml/dfs/callbacks.hpp>
#include <arxml/elements.hpp>

namespace arxml::dfs {

    void traverse_model(model::IAutosarModel& root, TraverseModelCallback& callback);
    void traverse_model(model::IAutosarPackages& model_unit, TraverseModelCallback& callback);
    void traverse_model(model::IAutosarPackage& element, TraverseModelCallback& callback);
    void traverse_model(model::IModelEntry& model_unit, TraverseModelCallback& callback);
    void traverse_model(model::IAutosarElements& model_unit, TraverseModelCallback& callback);
    void traverse_model(model::IAutosarElement& element, TraverseModelCallback& callback);

}
