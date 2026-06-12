#include "pch.h"
#include "error.h"

void ErrorReporter::CompileError(int line, std::string_view msg) {
    std::cerr << "[line " << line << "] Compile error: " << msg << "\n";
    s_hadError = true;
}

void ErrorReporter::CompileError(int line, std::string_view where, std::string_view msg) {
    std::cerr << "[line " << line << "] Compile error" << where << ": " << msg << "\n";
    s_hadError = true;
}

bool ErrorReporter::HadError() { return s_hadError; }

void ErrorReporter::ClearError() { s_hadError = false; }
