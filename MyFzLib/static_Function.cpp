#include "stdafx.h"
#include "static_Function.h"
#include <tlhelp32.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "user32.lib")
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen)
{
	char h1, h2;
	BYTE s1, s2;
	for (int i = 0; i < nLen; i++)
	{
		h1 = pszSrc[2 * i];
		h2 = pszSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}

void HexToStr(char *pszDest, BYTE *pbSrc, int nLen)
{
	char	ddl, ddh;
	for (int i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pszDest[i * 2] = ddh;
		pszDest[i * 2 + 1] = ddl;
	}

	pszDest[nLen * 2] = '\0';

}

//�Լ�ʵ�ֵ��滻�������е�ͨ�����
void ConfigChar(char* szIdecode)//���������е�??תΪ00
{
	//char * szIdecode = *szIdecode1;
	int ndLen = strlen(szIdecode);
	for (size_t i = 0; i < ndLen; i++)
	{
		if (szIdecode[i] == '?')
		{

			szIdecode[i] = '0';
		}
	}

}

//�Լ�ʵ�ֵ��ж�2��2���ƴ��Ƿ����,���ֽڱȽ�,ͳ����ͬ�����Ƿ���ڴ���ĳ���,�����������ĳ���ֽڵ���0 �Զ�Ĭ��Ϊ���
bool isEqueByte(BYTE* szSource, BYTE* szIdecode, DWORD dwlen)
{
	DWORD ndlen = 0;
	for (DWORD i = 0; i < dwlen; i++)
	{
		BYTE ch1 = *(BYTE*)(szSource + i);
		BYTE ch2 = *(BYTE*)(szIdecode + i);
		if (ch1 == ch2 || ch2 == 0x00)
		{
			ndlen++;
		}
	}
	return ndlen == dwlen;
}

DWORD FindProcess(const std::string& Name)
{
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE ProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(ProcessSnapshot, &ProcessEntry))
	{
		do
		{
			if (!strcmp(ProcessEntry.szExeFile, Name.data()))
			{
				CloseHandle(ProcessSnapshot);
				return ProcessEntry.th32ProcessID;
			}
		} while (Process32Next(ProcessSnapshot, &ProcessEntry));
	}
	CloseHandle(ProcessSnapshot);
	return 0;
}

void __cdecl DbgPrtA(PCSTR format, ...) {
	va_list	args;
	va_start(args, format);
	int len = _vscprintf(format, args);
	if (len > 0) {
		len += (1 + 2);
		PSTR buf = (PSTR)malloc(len);
		if (buf) {
			len = vsprintf_s(buf, len, format, args);
			if (len > 0) {
				while (len && isspace(buf[len - 1])) len--;
				buf[len++] = '\r';
				buf[len++] = '\n';
				buf[len] = 0;
				OutputDebugStringA(buf);
			}
			free(buf);
		}
		va_end(args);
	}
}

void __cdecl DbgPrtW(PCWSTR format, ...) {
	va_list	args;
	va_start(args, format);
	int len = _vscwprintf(format, args);
	if (len > 0) {
		len += (1 + 2);
		PWSTR buf = (PWSTR)malloc(sizeof(WCHAR)*len);
		if (buf) {
			len = vswprintf_s(buf, len, format, args);
			if (len > 0) {
				while (len && iswspace(buf[len - 1])) len--;
				buf[len++] = L'\r';
				buf[len++] = L'\n';
				buf[len] = 0;
				OutputDebugStringW(buf);
			}
			free(buf);
		}
		va_end(args);
	}
}
//���໯���ڹ��̺���
WNDPROC subclass_game_wndproc(HWND hWnd, LONG our_wndproc)
{
	WNDPROC funWndProc = NULL;
	if (hWnd)
	{
		funWndProc = (WNDPROC)::SetWindowLongA(hWnd, GWL_WNDPROC, (LONG)our_wndproc);
	}
	return funWndProc;
}
//ж�ش������໯
void un_subclass_game_wndproc(HWND hWnd, LONG old_wndproc)//ж�����໯���ڹ���
{
	if (hWnd && old_wndproc)
	{
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)old_wndproc);
	}
}