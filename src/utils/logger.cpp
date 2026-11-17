#include "logger.h"
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/FileSink.h"

namespace Logger {
quill::Logger *logger = nullptr;

void INIT_LOGGER() {
  quill::Backend::start();

  quill::FileSinkConfig cfg;
  cfg.set_open_mode('w');

  auto sink =
      quill::Frontend::create_or_get_sink<quill::FileSink>("log/log.txt", cfg);

  quill::PatternFormatterOptions pfo;
  pfo.format_pattern = "%(time) %(log_level:<5) %(message)";
  pfo.timestamp_pattern = "%H:%M:%S";

  logger = quill::Frontend::create_or_get_logger("root", std::move(sink), pfo);
}
} // namespace Logger
