//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <iostream>
#include <ostream>
#include <memory>

#include <arxml/elements.hpp>
#include <arxml/dfs/callbacks.hpp>

namespace arxml::printer {

    class TreePrinter {
    public:
        explicit TreePrinter(std::ostream& stream, int indent = 4);
        void print(model::IAutosarModel& root);
        void print(model::IAutosarPackages& packages);
        void print(model::IAutosarPackage& package);
        void print(model::IAutosarElements& elements);
        void print(model::IAutosarElement& element);

        template<class AutosarModelObject>
        static void stdout_dump(AutosarModelObject& object, int tab_size = 4);
    private:
        std::unique_ptr<dfs::TraversalCallback> m_callback;
    };


    template<class AutosarModelObject>
    void TreePrinter::stdout_dump(AutosarModelObject& object, int tab_size) {
        TreePrinter printer(std::cout, tab_size);
        printer.print(object);
    }
}
