#include "ds/Logger.h"

#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace ds {

bool Logger::Init(std::string logger_name, std::string file_name, size_t max_file_size, size_t max_files)
{
  try
  {
    std::vector<spdlog::sink_ptr> sinks;

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%H:%M:%S][%5t][%^%-5!l%$] %-60v [%s:%#]");

    console_sink->set_level(spdlog::level::trace);
    sinks.push_back(console_sink);

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file_name, max_file_size, max_files);
    file_sink->set_pattern("[%C/%m/%d][%H:%M:%S.%e][%5P][%5t][%-5!l] %-60v [%s:%#]");
    sinks.push_back(file_sink);

    auto logger = std::make_shared<spdlog::logger>(logger_name, begin(sinks), end(sinks));

#ifdef _DEBUG
    logger->set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::info);
#else
    logger->flush_on(spdlog::level::info);
#endif

    spdlog::set_default_logger(logger);
  }
  catch (const spdlog::spdlog_ex& ex)
  {
    fprintf(stderr, "Log initialization failed : %s\n", ex.what());
    return false;
  }

  return true;
}

} // namespace ds