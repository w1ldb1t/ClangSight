#ifndef ASSERT_MACROS
#define ASSERT_MACROS

#include "platform.h"

#include <iostream>
#include <string>

#if defined(WINDOWS)
#include <Windows.h>
#elif defined(POSIX)
#include <cstdlib>
#else
#error Unsupported platform. Only Windows and Linux are supported.
#endif

#if defined(NDEBUG)
#define DCHECK(condition)                                                      \
  do {                                                                         \
    static_cast<void>(sizeof(condition));                                      \
  } while (false)
#define DCHECK_EQ(lhs, rhs)                                                    \
  do {                                                                         \
    static_cast<void>(sizeof(lhs));                                            \
    static_cast<void>(sizeof(rhs));                                            \
  } while (false)
#define DCHECK_NE(lhs, rhs)                                                    \
  do {                                                                         \
    static_cast<void>(sizeof(lhs));                                            \
    static_cast<void>(sizeof(rhs));                                            \
  } while (false)
#define DCHECK_GT(lhs, rhs)                                                    \
  do {                                                                         \
    static_cast<void>(sizeof(lhs));                                            \
    static_cast<void>(sizeof(rhs));                                            \
  } while (false)
#define DCHECK_LT(lhs, rhs)                                                    \
  do {                                                                         \
    static_cast<void>(sizeof(lhs));                                            \
    static_cast<void>(sizeof(rhs));                                            \
  } while (false)
#else
#define DCHECK(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      Crash(#condition, __FILE__, __LINE__);                                   \
    }                                                                          \
  } while (false)

#define DCHECK_EQ(lhs, rhs)                                                    \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value == rhs_value)) {                                           \
      Crash(#lhs " == " #rhs, __FILE__, __LINE__);                             \
    }                                                                          \
  } while (false)

#define DCHECK_NE(lhs, rhs)                                                    \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value != rhs_value)) {                                           \
      Crash(#lhs " != " #rhs, __FILE__, __LINE__);                             \
    }                                                                          \
  } while (false)

#define DCHECK_GT(lhs, rhs)                                                    \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value > rhs_value)) {                                            \
      Crash(#lhs " > " #rhs, __FILE__, __LINE__);                              \
    }                                                                          \
  } while (false)

#define DCHECK_LT(lhs, rhs)                                                    \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value < rhs_value)) {                                            \
      Crash(#lhs " < " #rhs, __FILE__, __LINE__);                              \
    }                                                                          \
  } while (false)
#endif

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      Crash(#condition, __FILE__, __LINE__);                                   \
    }                                                                          \
  } while (false)

#define CHECK_EQ(lhs, rhs)                                                     \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value == rhs_value)) {                                           \
      Crash(#lhs " == " #rhs, __FILE__, __LINE__);                             \
    }                                                                          \
  } while (false)

#define CHECK_NE(lhs, rhs)                                                     \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value != rhs_value)) {                                           \
      Crash(#lhs " != " #rhs, __FILE__, __LINE__);                             \
    }                                                                          \
  } while (false)

#define CHECK_GT(lhs, rhs)                                                     \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value > rhs_value)) {                                            \
      Crash(#lhs " > " #rhs, __FILE__, __LINE__);                              \
    }                                                                          \
  } while (false)

#define CHECK_LT(lhs, rhs)                                                     \
  do {                                                                         \
    auto lhs_value = (lhs);                                                    \
    auto rhs_value = (rhs);                                                    \
    if (!(lhs_value < rhs_value)) {                                            \
      Crash(#lhs " < " #rhs, __FILE__, __LINE__);                              \
    }                                                                          \
  } while (false)
#define NOT_REACHED()                                                          \
  do {                                                                         \
    Crash("NOT_REACHED()", __FILE__, __LINE__);                                \
  } while (false)

inline void Crash(const char *message, const char *file, int line) {
  std::string file_path = std::string(file);
  std::string file_name = file_path.substr(file_path.find_last_of("/\\") + 1);
  std::cout << "[FATAL:" << file_name << "(" << line << ")] "
            << "Check failed: " << message << std::endl;
#if defined(WINDOWS)
  DebugBreak();
#elif defined(POSIX)
  std::abort();
#endif
}

#endif // ASSERT_MACROS