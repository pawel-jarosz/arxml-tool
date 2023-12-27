//
// Created by Paweł Jarosz on 27.12.2023.
//

#include <arxml/printer.hpp>

#include <iomanip>
#include <cassert>

#include <arxml/dfs/traversal.hpp>

namespace arxml::printer {

    class TreePrinterCallback : public dfs::TraversalCallback {
    public:
        TreePrinterCallback(std::ostream& os, int tab_size)
        : m_os{os}
        , m_tab_size{tab_size}
        , m_indent_level{0}
        {}

        ~TreePrinterCallback() = default;

        void visit(model::IAutosarModel &root) override;
        void visit(model::IModelEntry &root) override;
        void visit(model::IAutosarPackages &packages) override;
        void visit(model::IAutosarPackage &package) override;
        void visit(model::IAutosarElements &elements) override;
        void visit(model::IAutosarElement &element) override;

        void close(model::IModelEntry &root) override;
        void close(model::IAutosarPackages &packages) override;
        void close(model::IAutosarPackage &package) override;
        void close(model::IAutosarElements &elements) override;
        void close(model::IAutosarElement &element) override;
    private:
        void visit(model::ISimpleAutosarElement& element);
        void visit(model::ICompositeAutosarElement& element);
        void visit(model::INamedAutosarElement& element);

        std::ostream& m_os;
        int m_tab_size;
        int m_indent_level;
    };

    void TreePrinterCallback::visit(model::IAutosarModel& root) {
        std::cout << "=> AUTOSAR\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::IModelEntry& model) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> AR-PACKAGES (from file: "
                  << model.getEntryName() << ")\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::IAutosarPackages &packages) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> AR-PACKAGES\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::IAutosarPackage &package) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> AR-PACKAGE: " << package.getName() << "\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::IAutosarElements &elements) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> ELEMENTS\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::IAutosarElement &element) {
        switch (element.getType()) {
            case model::EntryType::NAMED_ELEMENT:
                visit(dynamic_cast<model::INamedAutosarElement &>(element));
                break;
            case model::EntryType::COMPOSITE_ELEMENT:
                visit(dynamic_cast<model::ICompositeAutosarElement &>(element));
                break;
            case model::EntryType::STRING_ELEMENT:
            case model::EntryType::INTEGER_ELEMENT:
            case model::EntryType::FLOATING_ELEMENT:
                visit(dynamic_cast<model::ISimpleAutosarElement &>(element));
                break;
            default:
                break; // INVALID BRANCH
        }
    }

    void TreePrinterCallback::visit(model::ISimpleAutosarElement &element) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> " << element.getTag() << " ";
        const auto attributes = element.getAttributes();
        if (not attributes.empty()) {
            std::cout << "(" << attributes[0].first << ": " << attributes[0].second;
            for (int it = 1; it < attributes.size(); ++it) {
                std::cout << ", " << attributes[it].first << ": " << attributes[it].second;
            }
            std::cout << ")";
        }
        std::cout << ": ";
        switch (element.getType()) {
            case model::EntryType::INTEGER_ELEMENT: {
                std::cout << dynamic_cast<model::INumberAutosarElement &>(element).getInteger();
                break;
            }
            case model::EntryType::FLOATING_ELEMENT: {
                std::cout << dynamic_cast<model::INumberAutosarElement &>(element).getFloating();
                break;
            }
            case model::EntryType::STRING_ELEMENT: {
                std::cout << dynamic_cast<model::IStringAutosarElement &>(element).getText();
                break;
            }
            default:
                assert(1 == 0); // invalid branch
        }

        std::cout << "\n";
    }

    void TreePrinterCallback::visit(model::ICompositeAutosarElement &element) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> " << element.getTag() << "\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::visit(model::INamedAutosarElement &element) {
        std::cout << std::setw(m_indent_level * m_tab_size) << "" << "=> " << element.getTag() << ": "
                  << element.getName() << "\n";
        m_indent_level += 1;
    }

    void TreePrinterCallback::close(model::IModelEntry& root) {
        m_indent_level -= 1;
    }

    void TreePrinterCallback::close(model::IAutosarPackages &packages) {
        m_indent_level -= 1;
    }

    void TreePrinterCallback::close(model::IAutosarPackage &package) {
        m_indent_level -= 1;
    }

    void TreePrinterCallback::close(model::IAutosarElements &elements) {
        m_indent_level -= 1;
    }

    void TreePrinterCallback::close(model::IAutosarElement &element) {
        if (element.isComposite()) {
            m_indent_level -= 1;
        }
    }

    TreePrinter::TreePrinter(std::ostream& stream, int indent)
    : m_callback{new TreePrinterCallback{stream, indent}}
    {

    }

    template<class AutosarModelObject>
    void generic_print(dfs::TraversalCallback& callback, AutosarModelObject& object) {
        dfs::traverse_model(object, callback);
    }

    void TreePrinter::print(model::IAutosarModel& root) {
        generic_print(*m_callback, root);
    }

    void TreePrinter::print(model::IAutosarPackages& packages) {
        generic_print(*m_callback, packages);
    }

    void TreePrinter::print(model::IAutosarPackage& package) {
        generic_print(*m_callback, package);
    }

    void TreePrinter::print(model::IAutosarElements& elements) {
        generic_print(*m_callback, elements);
    }

    void TreePrinter::print(model::IAutosarElement& element) {
        generic_print(*m_callback, element);
    }

}
