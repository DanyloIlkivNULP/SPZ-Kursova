#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "framework.h"

#define HPRINT(_data_) std::wcout << _data_ << std::endl;

class Logger {

public:
	Logger(void); ~Logger(void);

	static Logger& GetInstance(void);

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static void LoadLogLevel(DWORD dwLogLevel);

	static BOOL LogMessage(DWORD dwLogLevel,
		const std::wstring& wsMessage);
	static DWORD LogLastError(DWORD dwLogLevel);
	static DWORD LogFormatError(DWORD dwLogLevel,
		DWORD dwError);

	static DWORD ClearLog(void);



	static DWORD ShowLastError(const wchar_t* wcTitle);
	static DWORD ShowFormatError(const wchar_t* wcTitle,
		DWORD dwError);
	static void ShowMessage(const wchar_t* wcMessage,
		const wchar_t* wcTitle);

	static void LogAndShowMessage(DWORD dwLogLevel,
		const std::wstring& wsMessage);

	enum LogLevel {
		LOG_LVL_NULL = 0x0,
		LOG_LVL_INFO = 0x1,
		LOG_LVL_WARNING = 0x2,
		LOG_LVL_ERROR = 0x3,
		LOG_LVL_DEBUG = 0x4
	};

private:
	HWND m_hwnd = NULL;
	DWORD m_dwLogLevel = LogLevel::LOG_LVL_NULL;

	std::mutex m_mLogMessage;
};

#endif //_LOGGER_H_
