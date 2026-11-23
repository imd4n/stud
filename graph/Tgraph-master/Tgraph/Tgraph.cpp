#include <fstream>
#include <iostream>
#include <string>

#include "graph.h"

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string input_file;
    std::string output_file;
    bool have_output = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[++i];
                have_output = true;
            }
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << '\n';
            return 1;
        } else if (input_file.empty()) {
            input_file = arg;
        }
    }

    if (input_file.empty()) {
        if (argc > 1) {
            std::cerr << "Usage: Tgraph <input_file> [-o output_file]\n";
            return 1;
        }
        std::cout << "Input file: ";
        if (!(std::cin >> input_file)) return 1;
    }

    // Тип графа берётся из файла ("U"/"D"); для старого файла без типа
    // используется неориентированный граф по умолчанию.
    Graph g(input_file.c_str(), false);

    if (have_output) {
        // Бинарный режим: эталонные файлы используют LF, а не CRLF.
        std::ofstream fout(output_file, std::ios::binary);
        if (!fout) {
            std::cerr << "Cannot create output file: " << output_file << '\n';
            return 1;
        }
        g.print_report(fout);
    } else {
        g.print_report(std::cout);
    }
    return 0;
}
