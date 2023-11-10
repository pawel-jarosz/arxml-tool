//
// Created by Paweł Jarosz on 10.11.2023.
//

#pragma once

#include <fstream>
#include <string>

namespace arxml::io {

    class IModelSource {
    public:
        virtual std::string getContent() = 0;
        virtual bool open(std::string_view input) = 0;
        virtual bool isOpened() = 0;
    };

    class FileSource : public IModelSource {
    public:
        explicit FileSource(std::string filename)
        : m_input(std::move(filename))
        , m_opened(true)
        {

        }

        FileSource()
        : m_input()
        , m_opened(false)
        {

        }

        std::string getContent() override;
        bool isOpened() override { return m_opened; };
        bool open(std::string_view filename) override;
    private:
        std::ifstream m_input;
        bool m_opened;
    };

    class StringSource : public IModelSource {
    public:
        explicit StringSource(std::string content)
        : m_content{std::move(content)}
        {

        }

        StringSource()
        : m_content{}
        {

        }

        bool isOpened() override { return true; }
        bool open(std::string_view input) override;
        std::string getContent() override { return m_content; };
    private:
        std::string m_content;
    };
}