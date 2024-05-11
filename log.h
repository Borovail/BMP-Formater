#ifndef LOG_H
#define LOG_H

// Define the LOG_INFO, LOG_ERROR, and LOG_WARNING macros
// They will only log if DEBUG is defined; otherwise, they perform no action or return NULL where needed.

#ifdef DEBUG
// Define ANSI color codes for colored logging
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define LOG_INFO(format, ...) printf(ANSI_COLOR_BLUE "INFO: [%s:%d] %s: " format ANSI_COLOR_RESET "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) printf(ANSI_COLOR_YELLOW "WARNING: [%s:%d] %s: " format ANSI_COLOR_RESET "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) printf(ANSI_COLOR_RED "ERROR: [%s:%d] %s: " format ANSI_COLOR_RESET "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#else
// Do nothing if not debugging
#define LOG_INFO(msg, ...)
#define LOG_ERROR(msg, ...)
#define LOG_WARNING(msg, ...)
#endif

#endif // LOG_H
