//
// Created by Paweł Jarosz on 29.12.2023.
//

#include "structure_dump_subprogram.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <arxml/utilities/parser_facade.hpp>
#include <arxml/dfs/callbacks.hpp>
#include <arxml/dfs/traversal.hpp>
#include <arxml/project.hpp>

namespace arxml_tool {

    namespace {
        struct Entry {
            std::string tag;
            int counter;
            std::vector<std::string> referenced_types;
            std::vector<std::unique_ptr<Entry>> entries;
        };

        void traverse(arxml::model::INumberAutosarElement& element, Entry& entry);
        void traverse(arxml::model::IStringAutosarElement& element, Entry& entry);
        void traverse(arxml::model::ICompositeAutosarElement& element, Entry& entry);
        void traverse(arxml::model::INamedAutosarElement& element, Entry& entry);
        void traverse(arxml::model::IAutosarElement& element, Entry& entry);

        void traverse(arxml::model::INumberAutosarElement& element, Entry& entry) {
            entry.counter += 1;
        }

        void traverse(arxml::model::IStringAutosarElement& element, Entry& entry) {
            entry.counter += 1;
            auto destination = element.getAttribute("DEST");
            if (destination.has_value() and std::find(entry.referenced_types.begin(),
                                                      entry.referenced_types.end(),
                                                      destination.value()) == entry.referenced_types.end()) {
                entry.referenced_types.push_back(*destination);
            }
        }

        void traverse(arxml::model::ICompositeAutosarElement& element, Entry& entry) {
            entry.counter += 1;
            for (auto& it: element.getSubElements()) {
                bool found = false;
                for (auto& subentry: entry.entries) {
                    if (it->getTag() == subentry->tag) {
                        traverse(*it, *subentry);
                        found = true;
                    }
                }
                if (not found) {
                    auto e = std::make_unique<Entry>(it->getTag(), 0);
                    traverse(*it, *e);
                    entry.entries.emplace_back(std::move(e));
                }
            }
        }

        void traverse(arxml::model::INamedAutosarElement& element, Entry& entry) {
            entry.counter += 1;
            if (entry.entries.empty()) {
                entry.entries.emplace_back(std::make_unique<Entry>("SHORT-NAME", 1));
            }
            entry.entries[0]->counter += 1;

            for (auto& it: element.getSubElements()) {
                bool found = false;
                for (auto& subentry: entry.entries) {
                    if (it->getTag() == subentry->tag) {
                        traverse(*it, *subentry);
                        found = true;
                    }
                }
                if (not found) {
                    auto e = std::make_unique<Entry>(it->getTag(), 0);
                    traverse(*it, *e);
                    entry.entries.emplace_back(std::move(e));
                }
            }
        }

        void traverse(arxml::model::IAutosarElement& element, Entry& entry) {
            using arxml::model::EntryType;
            switch (element.getType()) {
                case EntryType::STRING_ELEMENT: {
                    traverse(dynamic_cast<arxml::model::IStringAutosarElement&>(element), entry);
                    break;
                }
                case EntryType::INTEGER_ELEMENT:
                case EntryType::FLOATING_ELEMENT: {
                    traverse(dynamic_cast<arxml::model::INumberAutosarElement&>(element), entry);
                    break;
                }
                case EntryType::NAMED_ELEMENT: {
                    traverse(dynamic_cast<arxml::model::INamedAutosarElement&>(element), entry);
                    break;
                }
                case EntryType::COMPOSITE_ELEMENT: {
                    traverse(dynamic_cast<arxml::model::ICompositeAutosarElement&>(element), entry);
                    break;
                }
                default: break;
            }
        }

        class ModelGraph {
        public:
            ModelGraph() = default;
            void register_element(arxml::model::IAutosarElement& element);
            void show_structure(std::string expected_tag);
        private:
            std::map<std::string, Entry> m_entries;
        };

        void present_structure(Entry& e, int indent = 0) {
            std::cout << std::setw(indent) << " " << "=> " << e.tag;
            if (not e.referenced_types.empty()) {
                std::cout << " (reference to " << e.referenced_types[0];
                std::for_each(e.referenced_types.begin() + 1, e.referenced_types.end(), [](const std::string& v) {
                    std::cout << ", " << v;
                });
                std::cout << ")";
            }
            std::cout << std::endl;
            std::for_each(e.entries.begin(), e.entries.end(), [indent](auto& it) {
                present_structure(*it, indent + 4);
            });
        }

        void ModelGraph::show_structure(std::string expected_tag = "") {
            if (not expected_tag.empty()) {
                present_structure(m_entries.at(expected_tag));
                return;
            }
            std::for_each(m_entries.begin(), m_entries.end(), [](auto& entry) {
                present_structure(entry.second);
            });
        }

        void ModelGraph::register_element(arxml::model::IAutosarElement& element) {
            if (not m_entries.contains(element.getTag())) {
                m_entries[element.getTag()] = Entry {
                        element.getTag(),
                        0
                };
            }
            traverse(element, m_entries[element.getTag()]);
        }

        class StructureAnalyserCallback : public arxml::dfs::TraversalCallback {
        public:
            StructureAnalyserCallback(ModelGraph& graph)
            : m_graph(graph) {

            }
            void visit(arxml::model::IAutosarElements& elements) override;
        private:
            ModelGraph& m_graph;
        };

        void StructureAnalyserCallback::visit(arxml::model::IAutosarElements& elements) {
            for (auto& it: elements.getElements()) {
                m_graph.register_element(*it);
            }
        }
    }

    void StructureDumpSubProgram::execute(const std::vector<std::string> &args) {
        if (args[0] != getName()) {
            return;
        }
        if (args.size() == 1 or (args.size() > 1 and args[1] == "help")) {
            std::cerr << help() << std::endl;
            return;
        }
        if (args.size() < 3) {
            throw std::logic_error("Invalid number of the arguments! Expected mode and path or ask for the help.");
        }
        std::string project_configuration = args[1];
        std::string path = args[2];
        arxml::utilities::DefaultParserFacade parser;
        arxml::project::ModelProject project;
        project.addDirectory(path);
        arxml::project::ModelProject::openModelFromProject(project, parser);
        auto model = parser.getModel();
        ModelGraph graph;
        StructureAnalyserCallback callback(graph);
        arxml::dfs::traverse_model(*model, callback);
        if (args.size() == 3 or (args.size() > 3 and args[3] == "--full")) {
            graph.show_structure();
        }
        if (args.size() >= 5 and args[3] == "--tag") {
            graph.show_structure(args[4]);
        }
    }

    std::string StructureDumpSubProgram::help() {
        std::stringstream ss;
        ss << "ARXML Structure Dump generates tree-like model description";
        ss << "Command need to be called on the following ways\n";
        ss  << "1 | help\n"
            << "2 | dir MODEL_DIR_NAME\n"
            << "3 | config CONFIGURATION_FILE_NAME\n";
        ss << "There are additional flags:\n"
            << "--full - default behaviour which dump complete model structure\n"
            << "--tag - requires tag name and dumps structure of it\n\n";
        ss << "Examples:\n./arxml_tool structure_dump dir data/\n";
        ss << "The same effect like ./arxml_tool structure_dump dir data --full\n";
        ss << "./arxml-tool structure_dump config config.yml --tag SERVICE-INTERFACE";
        return ss.str();
    }
}
