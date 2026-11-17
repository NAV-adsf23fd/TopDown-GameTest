#pragma once

// Flag for turn off log
#ifdef NDEBUG
#define QUILL_COMPILE_ACTIVE_LOG_LEVEL QUILL_COMPILE_ACTIVE_LOG_LEVEL_INFO
#endif

#include "quill/LogMacros.h"
#include "quill/Logger.h"

namespace Logger {
extern quill::Logger *logger;
void INIT_LOGGER();
} // namespace Logger

#define Log(...) LOG_INFO(Logger::logger, __VA_ARGS__)
#define LogW(...) LOG_WARNING(Logger::logger, __VA_ARGS__)
#define LogE(...) LOG_ERROR(Logger::logger, __VA_ARGS__)
