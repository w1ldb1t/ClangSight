#ifndef _LOGGING_
#define _LOGGING_

#include <iostream>
#include <sstream>

enum class LogLevel { INFO, ERR, DEBUG };

class Logger {
public:
  explicit Logger(LogLevel level, const char *file, int line)
      : level_(level), file_(file), line_(line) {}

  ~Logger() {
    std::string log_message = ss_.str();
    switch (level_) {
    case LogLevel::ERR:
      std::cout << "[ERROR:" << GetFileName(file_) << "(" << line_ << ")] "
                << log_message << std::endl;
      break;
    case LogLevel::DEBUG:
#ifdef IS_DEBUG
      std::cout << "[DEBUG:" << GetFileName(file_) << "(" << line_ << ")] "
                << log_message << std::endl;
#endif
      break;
    case LogLevel::INFO:
      std::cout << "[INFO:" << GetFileName(file_) << "(" << line_ << ")] "
                << log_message << std::endl;
      break;
    }
  }

  template <typename T> auto operator<<(const T &value) -> Logger & {
    ss_ << value;
    return *this;
  }

private:
  static auto GetFileName(const char *file) -> std::string {
    std::string file_path = std::string(file);
    return file_path.substr(file_path.find_last_of("/\\") + 1);
  }

  std::ostringstream ss_;
  LogLevel level_;
  const char *file_;
  int line_;
};

#define LOG(level) Logger(LogLevel::level, __FILE__, __LINE__)
#endif // _LOGGING_