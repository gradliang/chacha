#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#endif

#include "stdafx.h"
#include "chacha2.h"
#include "logger.h"

#define LOG_DEFAULT_PORT				(19970)

//CRITICAL_SECTION  logDataCs;
std::list<std::string> logdata;

#ifdef WIN32
SOCKET sclient = INVALID_SOCKET;
#endif

void LogWriterInit()
{
#ifdef WIN32
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return;
	}
#endif

	logdata.clear();
}

int sync_all_logs()
{
#if 0
	SOCKET sclient = INVALID_SOCKET;
	FILE * fp = NULL;
	bool useFp = false;

	if (logdata.size() == 0)
		return 0;

	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (sclient == INVALID_SOCKET)
	{
		//printf("invalid socket !");
		fp = fopen("C:\\Temp\\mccchacha_fastcgi_log.txt", "ab");
		if (fp == NULL) 
		{
			logdata.clear();
			return 0;
		}
		useFp = true;
	}
	else
	{
		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(LOG_DEFAULT_PORT);
		serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("connect error !");
			closesocket(sclient);
			//////
			fp = fopen("C:\\Temp\\mccchacha_fastcgi_log.txt", "ab");
			if (fp == NULL)
			{
				logdata.clear();
				return 0;
			}
			useFp = true;
		}
	}

	std::list<std::string>::iterator it;

	for (it = logdata.begin(); it != logdata.end(); it++)
	{
		const char * str;
		int len, len2, n, cnt = 0;

		str = it->c_str();
		len = (int) it->length();
		len2 = len + 4;

		if (useFp)
		{
			fwrite(str, 1, len, fp);
		}
		else
		{
			n = send(sclient, (char*)&len2, 4, 0);
			while (len > 0)
			{
				n = send(sclient, &str[cnt], len, 0);
				if (n < 0)
					break;
				len -= n;
				cnt += n;
			}
		}

		
	}

	if (useFp)
	{
		fclose(fp);
		fp = NULL;
	}
	else
	{ 
		shutdown(sclient, SD_BOTH);
		closesocket(sclient);
		sclient = INVALID_SOCKET;
	}

	logdata.clear();
#endif
	return 0;
}


void putlogstr(const char * str)
{
#if 0
	std::string str1(str);
	//EnterCriticalSection(&logDataCs);
	//if (sclient != INVALID_SOCKET)
		logdata.push_back(str1);
	//LeaveCriticalSection(&logDataCs);
#endif
}

void putlogstr(const char * str, int size)
{
#if 0
	std::string str1(str, size);
	//EnterCriticalSection(&logDataCs);
	//if (sclient != INVALID_SOCKET)
		logdata.push_back(str1);
	//LeaveCriticalSection(&logDataCs);
#endif
}

void putlogstr(const char * str, unsigned int size)
{
#if 0
	std::string str1(str, size);
	//EnterCriticalSection(&logDataCs);
	//if (sclient != INVALID_SOCKET)
		logdata.push_back(str1);
	//LeaveCriticalSection(&logDataCs);
#endif
}

void putlogstr(std::string & str)
{
#if 0
	//EnterCriticalSection(&logDataCs);
	//if (sclient != INVALID_SOCKET)
		logdata.push_back(str);
	//LeaveCriticalSection(&logDataCs);
#endif
}

int logprint(const char *format, ...)
{
#if 0
	char buffer[3072];
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, ap);
	buffer[sizeof(buffer) - 1] = 0;
	va_end(ap);
	putlogstr(buffer);
	return ret;
#else
	return 0;
#endif
}

int logPrintWithSystemID(const char *format, ...)
{
#if 0
	char buffer[3072];
	va_list ap;
	int ret1, ret2, len;
	char * start;

	tm tm1;
	time_t curtime = time(NULL);
	localtime_s(&tm1, &curtime);

	ret1 = sprintf_s(buffer, sizeof(buffer) - 1, "%4d-%d-%d %02d:%02d:%02d, pid=%05d, tid=%05d, ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec,
		GetCurrentProcessId(), GetCurrentThreadId());
	len = (int) strlen(buffer);
	start = &buffer[len];

	va_start(ap, format);
	ret2 = vsnprintf_s(start, sizeof(buffer) - len, sizeof(buffer) - 1 - len, format, ap);
	buffer[sizeof(buffer) - 1] = 0;
	va_end(ap);
	putlogstr(buffer);
	return ret1 + ret2;
#else
	return 0;
#endif
}







