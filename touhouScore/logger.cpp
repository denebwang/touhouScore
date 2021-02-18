#include"logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>
std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("main", "logs/log.txt");