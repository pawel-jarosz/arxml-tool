//
// Created by Paweł Jarosz on 29.12.2023.
//

#pragma once

#include <arxml/dfs/callbacks.hpp>

#include <vector>

namespace arxml::helpers {

    class ElementByTagFinder : public dfs::TraversalCallback {
    public:
        ElementByTagFinder(std::map<std::string, model::INamedAutosarElement&>& result, std::string expected_tag)
                : m_result{result}
                , m_expected_tag{std::move(expected_tag)}
        {

        }

        void visit(model::IAutosarElements& elements) override;
        void visit(model::IAutosarPackage& package) override;
        void close(model::IAutosarPackage& package) override;
    private:
        std::string m_expected_tag;
        std::map<std::string, model::INamedAutosarElement&>& m_result;
        std::vector<std::string> m_path;
    };

    class ElementByIdFinder : public dfs::TraversalCallback {
    public:
        ElementByIdFinder(std::vector<std::reference_wrapper<model::INamedAutosarElement>>& result, std::string expected_id)
                : m_result{result}
                , m_expected_id{std::move(expected_id)}
        {

        }

        void visit(model::IAutosarElement& element) override;
        void visit(model::IAutosarPackage& package) override;
        void close(model::IAutosarPackage& package) override;
        void close(model::IAutosarElement& package) override;
    private:
        std::string m_expected_id;
        std::vector<std::reference_wrapper<model::INamedAutosarElement>>& m_result;
        std::vector<std::string> m_path;
    };

    class ElementByReferenceFinder : public dfs::TraversalCallback {
    public:
        ElementByReferenceFinder(std::vector<std::string>& result, std::string expected_id)
                : m_result{result}
                , m_expected_id{std::move(expected_id)}
        {

        }

        void visit(model::IAutosarElement& element) override;
        void visit(model::IAutosarPackage& package) override;
        void close(model::IAutosarPackage& package) override;
        void close(model::IAutosarElement& package) override;
    private:
        std::string m_expected_id;
        std::vector<std::string>& m_result;
        std::vector<std::string> m_path;
    };

    class RootElementFinder : public dfs::TraversalCallback {
    public:
        RootElementFinder(std::optional<std::pair<std::string, model::INamedAutosarElement&>>& result,
                          const std::string& full_path);

        void visit(model::IAutosarElements& elements) override;
        void visit(model::IAutosarPackage& package) override;
        void close(model::IAutosarPackage& package) override;
    private:
        std::optional<std::pair<std::string, model::INamedAutosarElement&>>& m_result;
        std::vector<std::string> m_full_path;
        std::vector<std::string> m_path;
    };
}
