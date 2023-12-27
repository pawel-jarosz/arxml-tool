//
// Created by Paweł Jarosz on 27.12.2023.
//

#include "arxml/dfs/traversal.h"

namespace arxml::dfs {
    class ElementTraversalStrategy {
    public:
        explicit ElementTraversalStrategy(TraverseModelCallback& callback)
                : m_callback{callback}
        {

        }

        void traverse_model(model::IAutosarModel& root);
        void traverse_model(model::IModelEntry& model);
        void traverse_model(model::IAutosarPackages& packages);
        void traverse_model(model::IAutosarPackage& packages);
        void traverse_model(model::IAutosarElements& elements);
        void traverse_model(model::IAutosarElement& element);
    private:
        TraverseModelCallback& m_callback;
    };

    void ElementTraversalStrategy::traverse_model(model::IAutosarModel& root) {
        m_callback.visit(root);
        for (auto& [unit_name, unit]: root.getModelUnits()) {
            traverse_model(*unit);
        }
        m_callback.close(root);

    }

    void ElementTraversalStrategy::traverse_model(model::IModelEntry& model) {
        m_callback.visit(model);
        for (auto& packages: model.getPackages()) {
            traverse_model(*packages);
        }
        m_callback.close(model);
    }

    void ElementTraversalStrategy::traverse_model(model::IAutosarPackages& packages) {
        m_callback.visit(packages);
        for (auto& subpackages: packages.getPackages()) {
            traverse_model(*subpackages);
        }
        m_callback.close(packages);
    }

    void ElementTraversalStrategy::traverse_model(model::IAutosarPackage& package) {
        m_callback.visit(package);
        switch(package.getCollectionType()) {
            case arxml::model::CollectionType::ELEMENTS_COLLECTION: {
                traverse_model(package.getElements());
                break;
            }
            case arxml::model::CollectionType::PACKAGES_COLLECTION: {
                traverse_model(package.getPackages());
                break;
            }
            default:
                //invalid branch
                break;
        }
        m_callback.close(package);
    }

    void ElementTraversalStrategy::traverse_model(model::IAutosarElements& elements) {
        m_callback.visit(elements);
        for (auto& it: elements.getElements()) {
            traverse_model(*it);
        }
        m_callback.close(elements);
    }

    void ElementTraversalStrategy::traverse_model(model::IAutosarElement& element) {
        m_callback.visit(element);
        bool is_named_element = element.getType() == model::EntryType::NAMED_ELEMENT;
        bool is_composite_element = element.getType() == model::EntryType::COMPOSITE_ELEMENT;
        if (is_composite_element or is_named_element) {
            auto& composite = dynamic_cast<model::ICompositeAutosarElement&>(element);
            for (auto& subelements:  composite.getSubElements()) {
                traverse_model(*subelements);
            }
        }
        m_callback.close(element);
    }


    template<class ModelElement>
    void traverse_model_tree(ModelElement& element, TraverseModelCallback& callback) {
        ElementTraversalStrategy traversal_strategy{callback};
        traversal_strategy.traverse_model(element);
    }

    void traverse_model(model::IAutosarModel& root, TraverseModelCallback& callback) {
        traverse_model_tree(root, callback);
    }

    void traverse_model(model::IModelEntry& model_unit, TraverseModelCallback& callback) {
        traverse_model_tree(model_unit, callback);
    }

    void traverse_model(model::IAutosarPackages& packages, TraverseModelCallback& callback) {
        traverse_model_tree(packages, callback);
    }

    void traverse_model(model::IAutosarPackage& package, TraverseModelCallback& callback) {
        traverse_model_tree(package, callback);
    }

    void traverse_model(model::IAutosarElements& elements, TraverseModelCallback& callback) {
        traverse_model_tree(elements, callback);
    }

    void traverse_model(model::IAutosarElement& element, TraverseModelCallback& callback) {
        traverse_model_tree(element, callback);
    }
}

