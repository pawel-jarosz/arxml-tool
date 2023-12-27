#include <iostream>
#include <arxml_tool/program_selector.hpp>

int main(int argc, const char* argv[]) {
    arxml_tool::ProgramSelector selector(argc, argv);

    try {
        selector.execute();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
