#include "logger.h"

Logger::Logger(void)
{ /*Code...*/ }
Logger::~Logger(void)
{ /*Code...*/ }

Logger& Logger::GetInstance(void)
{ static Logger _h; return(_h); }

void Logger::LoadLogLevel(DWORD dwLogLevel) {
	std::lock_guard<std::mutex>
		lgLogMessage(GetInstance().m_mLogMessage);

	GetInstance().m_dwLogLevel = dwLogLevel;
}

BOOL Logger::LogMessage(DWORD dwLogLevel, const std::wstring& wsMessage) {
	if (dwLogLevel == LogLevel::LOG_LVL_NULL)
		{ return(FALSE); }
	std::lock_guard<std::mutex>
		lgLogMessage(GetInstance().m_mLogMessage);

	size_t sizeMessageLength = wsMessage.size();

	SYSTEMTIME sysTime = { 0x0 };

	HANDLE hLogFileHandle = INVALID_HANDLE_VALUE;
	DWORD dwEndOfFile = 0x0, dwNumberOfBytesWritten = 0x0;

	DWORD dwError = ERROR_SUCCESS;

	if (GetInstance().m_dwLogLevel < dwLogLevel) { return(FALSE); }
	if (sizeMessageLength <= NULL ||
		sizeMessageLength >= 1024) { return(FALSE); }

	//*wstring wsDateTime, wsSeverityTag, wsFormattedMessage;

	wchar_t wcDateTime[64]{};
	wchar_t wcSeverityTag[16]{};
	wchar_t wcFormattedMessage[1024];

	GetLocalTime(&sysTime);

	swprintf_s(wcDateTime, L"[%i/%i/%i %i:%i:%i.%i]",
		sysTime.wMonth, sysTime.wDay, sysTime.wYear,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
		sysTime.wMilliseconds);

	switch (dwLogLevel) {
	case LogLevel::LOG_LVL_INFO:
		wcscpy_s(wcSeverityTag, L"[INFO] : ");
		break;
	case LogLevel::LOG_LVL_WARNING:
		wcscpy_s(wcSeverityTag, L"[WARNING] : ");
		break;
	case LogLevel::LOG_LVL_ERROR:
		wcscpy_s(wcSeverityTag, L"[ERROR] : ");
		break;
	case LogLevel::LOG_LVL_DEBUG:
		wcscpy_s(wcSeverityTag, L"[DEBUG] : ");
		break;

	default:
		break;
	}

	swprintf_s(wcFormattedMessage, L"%s\n", wsMessage.data());

	if ((hLogFileHandle = CreateFile(LOG_FILE_NAME, FILE_APPEND_DATA,
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{ dwError = GetLastError(); goto linkError; }

	dwEndOfFile = SetFilePointer(hLogFileHandle, 0x0, NULL, FILE_END);

	if(!WriteFile(hLogFileHandle, wcDateTime, wcslen(wcDateTime) * sizeof(wchar_t),
		&dwNumberOfBytesWritten, NULL))
	{ dwError = GetLastError(); goto linkError; }

	if(!WriteFile(hLogFileHandle, wcSeverityTag, wcslen(wcSeverityTag) * sizeof(wchar_t),
		&dwNumberOfBytesWritten, NULL))
	{ dwError = GetLastError(); goto linkError; }

	if(!WriteFile(hLogFileHandle, wcFormattedMessage, wcslen(wcFormattedMessage) * sizeof(wchar_t),
		&dwNumberOfBytesWritten, NULL))
	{ dwError = GetLastError(); goto linkError; }

	if (hLogFileHandle != INVALID_HANDLE_VALUE)
		if(!CloseHandle(hLogFileHandle))
		{ dwError = GetLastError(); goto linkExit; }

linkExit:
	return(TRUE);
linkError:
	(void)ShowFormatError
		(L"LogMessage - Error!", dwError); { return(FALSE); }
}

DWORD Logger::LogLastError(DWORD dwLogLevel) {
	DWORD dwError = ERROR_SUCCESS;
	(void)LogFormatError(dwLogLevel,
		dwError = GetLastError());
	return(dwError);
}

DWORD Logger::LogFormatError(DWORD dwLogLevel, DWORD dwError) {
	WCHAR wcError[256]{};
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), wcError, 256, NULL);
	(void)LogMessage(dwLogLevel,
		L"[" + std::to_wstring(dwError) + L"]" + wcError);
	return(dwError);
}

DWORD Logger::ClearLog(void) {
	DWORD dwError = ERROR_SUCCESS;

	if(!DeleteFile(LOG_FILE_NAME))
	{ dwError = GetLastError(); }

	return(dwError);
}



DWORD Logger::ShowLastError(const wchar_t* wcTitle) {
	DWORD dwError = ERROR_SUCCESS;
	ShowFormatError(wcTitle, dwError = GetLastError());
	return(dwError);
}
DWORD Logger::ShowFormatError(const wchar_t* wcTitle, DWORD dwError) {
	WCHAR wcError[256]{};
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), wcError, 256, NULL);

	wchar_t wcMessage[256]{};
	swprintf_s(wcMessage, L"[%d]%s", dwError, wcError);

	ShowMessage(wcMessage, wcTitle);
	return(dwError);
}
void Logger::ShowMessage(const wchar_t* wcMessage, const wchar_t* wcTitle) {
	(void)MessageBeep(MB_ICONERROR), (void)MessageBox(NULL, wcMessage, wcTitle,
		MB_OK | MB_DEFBUTTON1 | MB_APPLMODAL | MB_SETFOREGROUND);
}



void Logger::LogAndShowMessage(DWORD dwLogLevel, const std::wstring& wsMessage) {
	if(LogMessage(dwLogLevel, wsMessage))
	{ ShowMessage(wsMessage.data(),
		L"Log-Message");
	}
}
