#pragma once

#include "scanner.h"

class Solver {
  public:
    void Run(int argc, const char** argv);

  private:
    void RunREPL();
    void RunFile(std::string_view path);
    void Solve(std::string_view source);

  private:
    Scanner m_scanner;
};
