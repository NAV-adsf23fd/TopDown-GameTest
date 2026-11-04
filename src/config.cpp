#include "config.h"

#include <sstream>

RunConfig parseArgs(int argc, char **argv) {
  RunConfig config;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
      if (i + 1 < argc) {
        config.log_file = argv[++i];
      } else {
        config.log_file = "";
      }
      if (config.log_file == "") {
        config.to_file = false;
      }
    } else if (arg == "-v" || arg == "--verbose") {
      config.verbose_flag = true;
    }
  }
  return config;
}

std::ostream &operator<<(std::ostream &os, const RunConfig &config) {
  os << std::boolalpha; // in bool dưới dạng true/false
  os << "RunConfig {\n"
     << "  to_file:      " << config.to_file << "\n"
     << "  log_file:     \"" << config.log_file << "\"\n"
     << "  verbose_flag: " << config.verbose_flag << "\n"
     << "}";
  return os;
}

std::string RunConfig::toString() const {
  std::stringstream ss;
  ss << std::boolalpha; // in bool dưới dạng true/false
  ss << "RunConfig {\n"
     << "  to_file:      " << to_file << "\n"
     << "  log_file:     \"" << log_file << "\"\n"
     << "  verbose_flag: " << verbose_flag << "\n"
     << "}";
  return ss.str();
}
