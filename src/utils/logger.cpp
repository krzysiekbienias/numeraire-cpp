#include "utils/logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace {

    std::string makeTimestampedLogFile(const std::string& dir) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        std::tm tm{};
        localtime_r(&t, &tm); // macOS / Linux

        std::ostringstream oss;
        oss << dir << "/numeraire_"
            << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S")
            << ".log";

        return oss.str();
    }

} // namespace

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger>& Logger::get() {
    if (!logger) {

#ifndef NUMERAIRE_LOG_DIR
        const std::string log_dir = "logs";
#else
        const std::string log_dir = NUMERAIRE_LOG_DIR;
#endif
        std::filesystem::create_directories(log_dir);

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);

        const std::string logfile = makeTimestampedLogFile(log_dir);
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            logfile, /*truncate=*/false
        );
        file_sink->set_level(spdlog::level::debug);

        logger = std::make_shared<spdlog::logger>(
            "logger_numeraire",
            spdlog::sinks_init_list{file_sink, console_sink}
        );

        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        spdlog::register_logger(logger);
    }
    return logger;
}