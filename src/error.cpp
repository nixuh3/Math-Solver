#include "error.h"
#include <iostream>

void ErrorReporter::CompileError(int line, std::string_view msg) {
    std::cerr << "[line " << line << "] Compile error: " << msg << "\n";
    s_hadError = true;
}

bool ErrorReporter::HadError() { return false; }

void ErrorReporter::ClearError() { s_hadError = false; }
