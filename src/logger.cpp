#include "logger.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "config.h"

void Logger::init(bool to_file, const std::string &path, bool verbose_flag) {
  write_to_file = to_file;

  if (write_to_file) {
    log_file.open(path, std::ios::out | std::ios::trunc);

    if (!log_file.is_open()) {
      std::cerr << "[Logger] Cannot open log file: " << path
                << ". Falling back to console only." << std::endl;
      write_to_file = false;
    }
  }

  std::string header = "Topdown Test (v." + std::string(VERSION) + ")";
  header += std::string(48, '-');

  if (write_to_file) {
    log_file << header << std::endl;
  }

  min_level = Level::INFO;

  if (verbose_flag) {
    min_level = Level::DEBUG;
  }
}

void Logger::log(const std::string &msg, Level level) {
  if (static_cast<int>(level) < static_cast<int>(min_level)) {
    return;
  }

  auto now = std::time(nullptr);
  auto tm = *std::localtime(&now);

  std::stringstream ss_lv;
  std::string formatted_lv;
  if (level != Level::MSG) {
    ss_lv << "[" << levelToString(level) << "]";
    formatted_lv = ss_lv.str();
  }

  // format message + timestamp
  std::stringstream ss_msg;
  ss_msg << (formatted_lv.empty() ? "" : " - ") << "["
         << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
  ss_msg << msg;

  std::string formatted_msg = ss_msg.str();

  // output ra file hoặc console
  if (write_to_file) {
    log_file << formatted_lv << formatted_msg << std::endl;
    log_file.flush(); // đảm bảo ghi ngay lập tức
  }
}

void Logger::close() {
  if (log_file.is_open()) {
    log_file.close();
  }
}
