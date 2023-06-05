#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#ifndef __cplusplus
#error Error! :: Please use the "C++"
#endif

#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Error! :: Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. Thanks!
#endif

#include "targetver.h"
// ��������� ����� ������������ ���������� �� ���������� Windows
/*#define WIN32_LEAN_AND_MEAN*/
// ����� ���������� Windows
#include <windows.h>
#include <windowsx.h>
#include <psapi.h>
// ����� ���������� ����� ���������� C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <inttypes.h>
// ����� ���������� ����� ���������� C++
#include <iostream>

#include <string>
#include <vector>
#include <list>

#include <memory>
#include <functional>
#include <chrono>

#include <thread>
#include <atomic>
#include <condition_variable>

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME L".AudioEngineLog.txt"
#endif

#endif //_FRAMEWORK_H_
