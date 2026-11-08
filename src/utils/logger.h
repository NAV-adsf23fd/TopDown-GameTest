#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

namespace Logger {
enum class Level : uint8_t {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  MSG,
};

inline std::string levelToString(Level level) {
  switch (level) {
  case Level::DEBUG:
    return "DEBUG";
  case Level::INFO:
    return "INFO";
  case Level::WARNING:
    return "WARN";
  case Level::ERROR:
    return "ERROR";
  case Level::MSG:
    return "";
  default:
    return "UNKNOWN";
  }
}

[[maybe_unused]] inline static Level min_level = Level::INFO;

const std::string RESET = "\033[0m";
const std::string LOG_RED = "\033[31m";
const std::string LOG_GREEN = "\033[32m";
const std::string LOG_YELLOW = "\033[33m";
const std::string LOG_BLUE = "\033[34m";
const std::string LOG_CYAN = "\033[36m";

inline std::string getColor(Level level) {
  switch (level) {
  case Level::DEBUG:
    return LOG_BLUE;
  case Level::INFO:
    return LOG_GREEN;
  case Level::WARNING:
    return LOG_YELLOW;
  case Level::ERROR:
    return LOG_RED;
  default:
    return RESET;
  }
}

[[maybe_unused]] inline static std::ofstream log_file;
[[maybe_unused]] inline static bool write_to_file = false;

void init(bool to_file = true, const std::string &path = "log/log.txt",
          bool verbose_flag = false);
void log(const std::string &msg, Level level = Level::INFO);
void close();

// Hàm cơ sở để dừng đệ quy
inline void build_string(std::stringstream &) {}

// Hàm variadic template để nhồi mọi loại biến vào stringstream
template <typename T, typename... Args>
void build_string(std::stringstream &ss, T &&first, Args &&...args) {
  ss << std::forward<T>(first);
  build_string(ss, std::forward<Args>(args)...);
}

// Hàm Wrapper chính cho Logger
template <typename... Args> void qlog(Logger::Level level, Args &&...args) {
  std::stringstream ss;
  build_string(ss, std::forward<Args>(args)...);
  Logger::log(ss.str(), level);
}

} // namespace Logger
