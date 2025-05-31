#pragma once
#include <string>
#include <Windows.h> // For MessageBox
#include <sstream>   // For wostringstream
#include <exception> // For std::exception

// Helper to convert char* to wchar_t* at compile time for string literals
#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

// Macros to get current file (as wide string) and line number
#define WFILE_MACRO WIDEN(__FILE__)
#define LINE_MACRO __LINE__

// Function declarations
void LogExceptionDetails(const std::wstring& errorMessage, const wchar_t* file, int line);
void LogStdException(const std::exception& ex, const wchar_t* file, int line);
void LogUnknownException(const wchar_t* file, int line);

// Function declaration for specific validation/configuration errors
void LogValidationError(const std::wstring& validationMessage, const wchar_t* file, int line);

// User-facing macros for logging
#define CATCH_AND_LOG_STD_EXCEPTION(ex) LogStdException(ex, WFILE_MACRO, LINE_MACRO)
#define CATCH_AND_LOG_UNKNOWN_EXCEPTION() LogUnknownException(WFILE_MACRO, LINE_MACRO)
#define LOG_ERROR_MESSAGE(wide_msg_str) LogExceptionDetails(wide_msg_str, WFILE_MACRO, LINE_MACRO)

// User-facing macro for logging validation errors
#define LOG_VALIDATION_ERROR(wide_msg_str) LogValidationError(wide_msg_str, WFILE_MACRO, LINE_MACRO)
