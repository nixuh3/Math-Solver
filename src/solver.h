#pragma once

#include "arena.h"

class MathSolver {
  public:
    void Run(int argc, const char** argv);

  private:
    void RunREPL();
    void RunFile(std::string_view path);
    void Solve(std::string_view source);

  private:
    Arena m_arena{ 2 * 1024 * 1024 };
};
