#include "solver.h"
#include "error.h"
#include "scanner.h"
#include "parser.h"
#include "engine.h"
#include "utils.h"
#include <iostream>
#include <fstream>

void MathSolver::Run(int argc, const char** argv) {
    if (argc > 2) {
        std::cout << "Usage: math_solver [path]\n";
    } else if (argc == 2) {
        RunFile(argv[1]);
    } else {
        RunREPL();
    }
}

void MathSolver::RunREPL() {
    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line.empty()) {
            continue;
        }

        Solve(line);
        ErrorReporter::ClearError();
    }
}

void MathSolver::RunFile(std::string_view path) {
    std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);
    if (!file) {
        std::cout << "Failed to open file: " << path << "\n";
        return;
    }

    std::string buffer;
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer.resize(size);

    file.read(buffer.data(), size);
    file.close();

    Solve(buffer);
}

void MathSolver::Solve(std::string_view source) {
    Scanner scanner(source);
    auto& tokens = scanner.Scan();

    Parser parser(tokens, m_arena);
    auto ast = parser.Parse();

    if (ErrorReporter::HadError()) {
        return;
    }

    Engine engine(ast);
    std::cout << FormatDouble(engine.Evaluate()) << "\n";
}
