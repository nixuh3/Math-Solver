#pragma once

#include <string_view>

class ErrorReporter {
  public:
    static void CompileError(int line, std::string_view msg);
    static void CompileError(int line, std::string_view where, std::string_view msg);
    static bool HadError();
    static void ClearError();

  private:
    inline static bool s_hadError = false;
};
