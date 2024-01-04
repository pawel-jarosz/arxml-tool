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

    template<class AutosarModelObject>
    void generic_print(dfs::TraversalCallback& callback, AutosarModelObject& object) {
        dfs::traverse_model(object, callback);
    }

    TreePrinter::TreePrinter(std::ostream& stream, int indent)
    : m_callback{new TreePrinterCallback{stream, indent}}
    {

    }

    void TreePrinter::print(model::IAutosarModel& root) {
        generic_print(*m_callback, root);
    }

    void TreePrinter::print(model::IModelEntry& root) {
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

    class ArxmlPrinterCallback : public dfs::TraversalCallback {
    public:
        ArxmlPrinterCallback(std::ostream& os, int tab_size)
                : m_os{os}
                , m_tab_size{tab_size}
                , m_indent_level{0}
        {}

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

    void ArxmlPrinterCallback::visit(model::IAutosarModel& root) {

    }

    void ArxmlPrinterCallback::visit(model::IModelEntry& model) {
        m_os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
             << "<AUTOSAR xmlns=\"" << model.getXmlns() << "\" xmlns:xsi=\"" << model.getXmlnsXsi()
             << "\" xsi:schemaLocation=\"" << model.getSchemaLocation() << "\">\n";
        m_indent_level += 1;
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<AR-PACKAGES>\n";
        m_indent_level += 1;
    }

    void ArxmlPrinterCallback::visit(model::IAutosarPackages &packages) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<AR-PACKAGES>\n";
        m_indent_level += 1;
    }

    void ArxmlPrinterCallback::visit(model::IAutosarPackage &package) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<AR-PACKAGE>\n";
        m_indent_level += 1;
    }

    void ArxmlPrinterCallback::visit(model::IAutosarElements &elements) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<ELEMENTS>\n";
        m_indent_level += 1;
    }

    void ArxmlPrinterCallback::visit(model::IAutosarElement &element) {
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

    void ArxmlPrinterCallback::visit(model::ISimpleAutosarElement &element) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<" << element.getTag();
        const auto attributes = element.getAttributes();
        if (not attributes.empty()) {
            for (const auto& [name, value]: attributes) {
                m_os << " " << name << "=\"" << value;
            }
            m_os << "\"";
        }
        m_os << ">" << std::setw(0);
        switch (element.getType()) {
            case model::EntryType::INTEGER_ELEMENT: {
                m_os << dynamic_cast<model::INumberAutosarElement &>(element).getInteger();
                break;
            }
            case model::EntryType::FLOATING_ELEMENT: {
                m_os << dynamic_cast<model::INumberAutosarElement &>(element).getFloating();
                break;
            }
            case model::EntryType::STRING_ELEMENT: {
                m_os << dynamic_cast<model::IStringAutosarElement &>(element).getText();
                break;
            }
            default:
                assert(1 == 0); // invalid branch
        }
    }

    void ArxmlPrinterCallback::visit(model::ICompositeAutosarElement &element) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<" << element.getTag() << ">\n";
        m_indent_level += 1;
    }

    void ArxmlPrinterCallback::visit(model::INamedAutosarElement &element) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<" << element.getTag() << ">\n";
        m_indent_level += 1;
        m_os << std::setw(m_indent_level * m_tab_size) << " " <<  "<SHORT-NAME>" << element.getName() << "</SHORT-NAME>\n";
    }

    void ArxmlPrinterCallback::close(model::IModelEntry& root) {
        m_indent_level -= 1;
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "</AR-PACKAGES>\n";
        m_indent_level -= 1;
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "</AUTOSAR>\n";
    }

    void ArxmlPrinterCallback::close(model::IAutosarPackages &packages) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "</AR-PACKAGES>\n";
        m_indent_level -= 1;
    }

    void ArxmlPrinterCallback::close(model::IAutosarPackage &package) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<AR-PACKAGE>\n";
        m_indent_level -= 1;
    }

    void ArxmlPrinterCallback::close(model::IAutosarElements &elements) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "<ELEMENTS>\n";
        m_indent_level -= 1;
    }

    void ArxmlPrinterCallback::close(model::IAutosarElement &element) {
        m_os << std::setw(m_indent_level * m_tab_size) << " " << "</" << element.getTag() << ">\n";
        if (element.isComposite()) {
            m_indent_level -= 1;
        }
    }

    ArxmlPrinter::ArxmlPrinter(std::ostream& stream, int indent)
            : m_callback{new ArxmlPrinterCallback{stream, indent}}
    {

    }

    void ArxmlPrinter::print(model::IAutosarModel& root) {
        for (auto& [name, entry]: root.getModelUnits()) {
            std::cout << "entry: " << name << std::endl;
            generic_print(*m_callback, *entry);
        }
    }

    void ArxmlPrinter::print(model::IModelEntry& root) {
        generic_print(*m_callback, root);
    }

    void ArxmlPrinter::print(model::IAutosarPackages& packages) {
        generic_print(*m_callback, packages);
    }

    void ArxmlPrinter::print(model::IAutosarPackage& package) {
        generic_print(*m_callback, package);
    }

    void ArxmlPrinter::print(model::IAutosarElements& elements) {
        generic_print(*m_callback, elements);
    }

    void ArxmlPrinter::print(model::IAutosarElement& element) {
        generic_print(*m_callback, element);
    }

}
