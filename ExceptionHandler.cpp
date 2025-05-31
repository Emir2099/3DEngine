#include "ExceptionHandler.h"
#include <vector>
#include <cstdlib> // For mbstowcs_s
#include <string>  // For strlen
#include <sstream> // For wostringstream (though included via ExceptionHandler.h)

// Implementation for detailed logging
void LogExceptionDetails(const std::wstring& errorMessage, const wchar_t* file, int line) {
    std::wostringstream oss;
    oss << L"An error occurred:\n\n"
        << L"Message: " << errorMessage << L"\n"
        << L"File: " << file << L"\n"
        << L"Line: " << line;
    MessageBoxW(NULL, oss.str().c_str(), L"Exception Caught", MB_OK | MB_ICONERROR); // Explicitly use MessageBoxW
}

// Implementation for std::exception
void LogStdException(const std::exception& ex, const wchar_t* file, int line) {
    const char* what_char = ex.what();
    std::wstring what_wstring;
    if (what_char) {
        size_t convertedChars = 0;
        // Determine buffer size: strlen + 1 for null terminator
        size_t bufferSizeInWchars = strlen(what_char) + 1;
        std::vector<wchar_t> wcstring_buffer(bufferSizeInWchars);
        
        errno_t err = mbstowcs_s(&convertedChars, wcstring_buffer.data(), bufferSizeInWchars, what_char, _TRUNCATE);
        
        if (err == 0) {
            what_wstring = wcstring_buffer.data();
        } else {
            std::wostringstream oss_err_conv;
            oss_err_conv << L"Failed to convert exception message. Error code: " << err;
            what_wstring = oss_err_conv.str();
        }
    } else {
        what_wstring = L"Unknown std::exception (no message provided).";
    }
    LogExceptionDetails(what_wstring, file, line);
}

// Implementation for unknown exceptions
void LogUnknownException(const wchar_t* file, int line) {
    LogExceptionDetails(L"An unknown non-standard exception occurred.", file, line);
}

// Implementation for specific validation/configuration errors
void LogValidationError(const std::wstring& validationMessage, const wchar_t* file, int line) {
    std::wostringstream oss;
    oss << L"Application Validation Error:\n\n"
        << L"Message: " << validationMessage << L"\n"
        << L"File: " << file << L"\n"
        << L"Line: " << line;
    MessageBoxW(NULL, oss.str().c_str(), L"Validation Error", MB_OK | MB_ICONWARNING); // Use a different icon, e.g., MB_ICONWARNING
}
