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

//自己实现的替换特征码中的通配符号
void ConfigChar(char* szIdecode)//把特征码中的??转为00
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

FindArray Fun_FindIdent(void * szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout)
{
	//以前的调用例程
	//				//取出和一段和特征码长度相同的区域来进行逐字节比较,遇到??翻译成00默 5
	//	char idetecode[] = "55660a72??eb";
	//	byte  szsouce[] = { 0x55,0x66,0x0a,0x72,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x75,0x88,0x90,0x55,0x66,0x0a,0x72,0x75,0xeb,0x08 };
	//	ConfigChar(idetecode);
	//	int ndsouce = sizeof(szsouce);
	//	int ndcodelen = strlen(idetecode);
	//	int requierlen = ndcodelen /2;
	//	byte * TraceBuff = new byte(requierlen);
	//	memset(TraceBuff, 0, requierlen);
	//	StrToHex(TraceBuff, idetecode, requierlen);
	//	DWORD dwAddr = 0;
	//	dwAddr = SearchChinStream((byte*)szsouce, TraceBuff, requierlen, ndsouce);
	
	FindArray tmpArray;
	MyFindRecod TmpRecode;
	tmpArray.clear();
	//必要的参数检查
	if (szSource==NULL|| dwSearchSize==0|| szIdent==NULL)
	{
		return tmpArray;
	}
	int ndStr = strlen(szIdent);
	if (ndStr%2!=0)
	{
		//
		DbgPrtA("wjj 特征码必须是可被2整除的");
		return tmpArray;
	}
	//BYTE* FindSouce = (BYTE*)szSource;
	DWORD dwStartaddr = (DWORD)szSource;
	int ndcodelen = strlen(szIdent);
	ConfigChar(szIdent);//替换通配符
	int requierlen = ndcodelen /2;
	BYTE * TraceBuff = new BYTE(requierlen);//申请内存存放转换后的特征码
	memset(TraceBuff, 0, requierlen);
	StrToHex(TraceBuff, szIdent, requierlen);//转换为16进制
	DWORD dwEndAddr = (DWORD)szSource + dwSearchSize; //结束地址
	DWORD dwTcout = 0; //匹配计次
	DWORD dwremainder = dwEndAddr - dwStartaddr;//剩余字节数
	while (dwStartaddr>=dwremainder)
	{
		dwremainder = dwEndAddr - dwStartaddr;//剩余字节数重新赋值
		TmpRecode = { 0 };
		if (dwTcout== dwCout)
		{
			break;
		}
		if (isEqueByte((BYTE*)dwStartaddr,TraceBuff,requierlen))
		{
			//如果匹配成功了
			TmpRecode.dwAddr = dwStartaddr;
			dwTcout++;
			TmpRecode.ndNum = dwTcout;//先累加保证下标从1开始
			tmpArray.push_back(TmpRecode);
			dwStartaddr += ndcodelen; 
			continue;
		}
		dwStartaddr++;
	}
	delete[] TraceBuff;
	return tmpArray;
}

//自己实现的判断2段2进制串是否相等,逐字节比较,统计相同个数是否等于传入的长度,如果特征码中某个字节等于0 自动默认为相等
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
//子类化窗口过程函数
WNDPROC subclass_game_wndproc(HWND hWnd, LONG our_wndproc)
{
	WNDPROC funWndProc = NULL;
	if (hWnd)
	{
		funWndProc = (WNDPROC)::SetWindowLongA(hWnd, GWL_WNDPROC, (LONG)our_wndproc);
	}
	return funWndProc;
}
//卸载窗口子类化
void un_subclass_game_wndproc(HWND hWnd, LONG old_wndproc)//卸载子类化窗口过程
{
	if (hWnd && old_wndproc)
	{
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)old_wndproc);
	}
}
