#include"logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
std::shared_ptr<spdlog::logger> logger = spdlog::rotating_logger_mt("main", "logs/log.txt", 1024 * 1024, 5, true);