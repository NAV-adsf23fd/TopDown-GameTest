#pragma once
#include <ostream>
#include <string>

inline const std::string VERSION = "0.1";

struct RunConfig {
  std::string log_file = "log/log.txt";

  bool to_file = true;
  bool verbose_flag = false;

  friend std::ostream &operator<<(std::ostream &os, const RunConfig &config);
  std::string toString() const;
};

RunConfig parseArgs(int argc, char *argv[]);
