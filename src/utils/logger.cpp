#include "logger.hpp"
#include <spdlog/sinks/daily_file_sink.h>
#include <filesystem>

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger>& Logger::get() {
    if (!logger) {
        std::string log_dir = "/Users/krzysztofbienias/Loggs";
        std::filesystem::create_directories(log_dir);

        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            log_dir + "/numeraire", 0, 0); // rotates daily at midnight

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        file_sink->set_level(spdlog::level::debug);
        console_sink->set_level(spdlog::level::info);

        logger = std::make_shared<spdlog::logger>("logger_numeraire",
                                                  spdlog::sinks_init_list{file_sink, console_sink});

        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        spdlog::register_logger(logger);
    }
    return logger;
}

