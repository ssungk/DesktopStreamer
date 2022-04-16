#ifndef _DS_CORE_LOGGER_H_
#define _DS_CORE_LOGGER_H_

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/fmt/ostr.h>

namespace ds {

class Logger
{
public:
  static bool Init(std::string logger_name, std::string file_name, size_t max_file_size, size_t max_files);
};

// SPDLOG style log
#define DSLOG_TRACE(...)         SPDLOG_TRACE(##__VA_ARGS__)
#define DSLOG_DEBUG(...)         SPDLOG_DEBUG(##__VA_ARGS__)
#define DSLOG_INFO(...)          SPDLOG_INFO(##__VA_ARGS__)
#define DSLOG_WARN(...)          SPDLOG_WARN(##__VA_ARGS__)
#define DSLOG_ERROR(...)         SPDLOG_ERROR(##__VA_ARGS__)
#define DSLOG_CRITICAL(...)      SPDLOG_CRITICAL(##__VA_ARGS__)

} // namespace ds

#endif