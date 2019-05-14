#include "stdafx.h"
#include "static_Function.h"
#include <tlhelp32.h>
#include <Psapi.h>

//#pragma comment(lib,"")
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

DWORD Fun_FindIdent(void * szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout)
{

	DWORD dwret = 0;
	if (szSource==NULL|| dwSearchSize==0|| szIdent==NULL)
	{
		return dwret;
	}
	int ndStr = strlen(szIdent);
	if (ndStr%2!=0)
	{
		//
		DbgPrtA("wjj 特征码必须是可被2整除的");
		return dwret;
	}
	//BYTE* FindSouce = (BYTE*)szSource;
	DWORD dwStartaddr = (DWORD)szSource;
	int ndcodelen = strlen(szIdent);
	ConfigChar(szIdent);//替换通配符
	int requierlen = ndcodelen /2;
	BYTE * TraceBuff = new BYTE[requierlen];//申请内存存放转换后的特征码
	memset(TraceBuff, 0, requierlen);
	StrToHex(TraceBuff, szIdent, requierlen);//转换为16进制
	DWORD dwEndAddr = (DWORD)szSource + dwSearchSize; //结束地址
	DWORD dwTcout = 0; //匹配计次
	DWORD dwremainder = dwEndAddr - dwStartaddr;//剩余字节数
	BYTE bFirstcode = *(BYTE*)(TraceBuff);
	BYTE bEndcode = *(BYTE*)(TraceBuff+ (requierlen-1));
	while (dwremainder>=requierlen)
	{
		//TmpRecode = { 0 };
		if (dwTcout == dwCout)
		{
			break;
		}
		BYTE bFirstScode = *(BYTE*)(dwStartaddr);
		BYTE bEndScode = *(BYTE*)(dwStartaddr + (requierlen-1));
		if (bFirstScode==bFirstcode)
		{
			//如果匹配成功了
			if (bEndScode == bEndcode)
			{
				if (isEqueByte((BYTE*)dwStartaddr, TraceBuff, requierlen))
				{
					dwTcout++;
					if (dwTcout == dwCout)
					{
						dwret = dwStartaddr;
						break;
					}
					dwStartaddr += (requierlen-1);
				}
			}
		}
		dwStartaddr ++;
		dwremainder = dwEndAddr - dwStartaddr;//剩余字节数重新赋值
	}
	delete[]  TraceBuff;
	return dwret;
}

DWORD Fun_FindIdentByModule(char * szModule, char * szIdent, int ndOffset ,DWORD dwCout)
{
	DWORD dwret = 0;
	MODULEINFO minfo = { 0 };
	HMODULE hmodule = GetModuleHandle(szModule);
	if (hmodule==NULL)
	{
		return dwret;
	}
	if (GetModuleInformation(GetCurrentProcess(), hmodule, &minfo, sizeof(MODULEINFO)) == 0)
	{
		return dwret;
	}
	dwret=Fun_FindIdent((void*)minfo.lpBaseOfDll, minfo.SizeOfImage, szIdent, dwCout);
	dwret += ndOffset;
	return dwret;
}

DWORD Fun_FindIdentByModule2(DWORD dwProcessId, char * szModule, char * szIdent,int ndOffset, DWORD dwCout, DWORD SearChtype)
{
	DWORD dwRet = 0;
	MODULEENTRY32 minfo = GetModuleByName(dwProcessId,szModule);
	if (minfo.hModule!=NULL)
	{
		HANDLE hproccess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (hproccess!=INVALID_HANDLE_VALUE)
		{
			BYTE * szBase = new BYTE[minfo.modBaseSize];
			if (ReadProcessMemory(hproccess, minfo.modBaseAddr, szBase, minfo.modBaseSize, NULL))
			{
				dwRet = Fun_FindIdent(szBase, minfo.modBaseSize, szIdent, dwCout);
				if (dwRet>0)
				{
					dwRet = (dwRet - (DWORD)szBase)+ (DWORD)minfo.modBaseAddr;
					dwRet += ndOffset;
					DWORD dwTemp = 0;
					switch (SearChtype)
					{
					case 地址:
						break;
					case 基地址:
						ReadProcessMemory(hproccess, (LPCVOID)dwRet, &dwRet, 4, NULL);
						break;
					case 偏移:
						ReadProcessMemory(hproccess, (LPCVOID)dwRet, &dwRet, 4, NULL);
						break;
					case CALL:
						ReadProcessMemory(hproccess, (LPCVOID)dwRet, &dwTemp, 4, NULL);
						dwRet = (dwRet + dwTemp) + 5;
						break;
					}
					
				}
			}
			delete[] szBase;
			CloseHandle(hproccess);
		}
	}
	return dwRet;
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

MODULEENTRY32 GetModuleByName(DWORD dwProcessId, char * szModuleName)
{
		MODULEENTRY32 retEntry = { 0 };
		char strModule[MAX_PATH] = { 0 };
		BOOL    bFound = FALSE;
		HANDLE  hModuleSnap = NULL;
		MODULEENTRY32   me32 = { 0 };
		hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if (hModuleSnap == INVALID_HANDLE_VALUE)
		{
			return retEntry;
		}
		me32.dwSize = sizeof(MODULEENTRY32);
		if (::Module32First(hModuleSnap, &me32))
		{
			do
			{
				sprintf(strModule, "%s", me32.szModule);
				if (!lstrcmpi(szModuleName, strModule)) // 如果找到则返回true
				{
					retEntry = me32;
					break;
				}
			} while (::Module32Next(hModuleSnap, &me32));
		}
		::CloseHandle(hModuleSnap);
	return retEntry;
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
char* gateMy(WCHAR* str)
{
	char*     pElementText;
	int      iTextLen;
	// wide char to multi char	
	iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str, -1, pElementText, iTextLen, NULL, NULL);
	char strText[0x256] = { 0 };
	strcpy(strText, pElementText);
	delete[] pElementText;
	return strText;
}

std::string read_reg_sz(char * KeyPath, char * Key)
{
	HKEY hkey;
	std::string strRet("");

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER, KeyPath, 0, KEY_READ, &hkey))
	{
		char szBuffer[MAX_PATH] = { 0 };
		DWORD dwSzType = REG_SZ;
		DWORD dwSize = sizeof(szBuffer);
		if (::RegQueryValueEx(hkey, Key, 0, &dwSzType, (LPBYTE)&szBuffer, &dwSize) == ERROR_SUCCESS)
		{
			strRet = szBuffer;
		}
	}
	return strRet;
}

ProcessResult GetProcessByName(char *szProcessName)
{
	ProcessResult retRslt;
	//先清空容器
	retRslt.clear();
	int ndrequired = 0;
	int ndrequired2 = 0;
	NTSTATUS ntret = MyNtQuerySystemInfomation(5, NULL, 0, (PULONG)&ndrequired);
	if (ntret != 0)
	{

	}
	if (ntret == 0xc0000004)
	{
		char * szBufferd = new char[ndrequired * 2];
		memset(szBufferd, 0, ndrequired * 2);
		NTSTATUS ntret = MyNtQuerySystemInfomation(5, szBufferd, ndrequired, (PULONG)&ndrequired2);
		SYSTEM_PROCESSES* myprocesslist = (SYSTEM_PROCESSES*)(szBufferd);
		while (myprocesslist->NextEntryDelta != 0)
		{
			if (myprocesslist->ProcessName.Buffer!=NULL)
			{
				char * szProcess=UnicodeToGB2312(myprocesslist->ProcessName.Buffer);
				if (stricmp(szProcess, szProcessName)==NULL)
				{
					retRslt.push_back((DWORD)myprocesslist->ProcessId);
				}
			}
			//pProcIndex = (PSYSTEM_PROCESSES)((char*)pProcIndex + pProcIndex->NextEntryDelta)
			myprocesslist = (PSYSTEM_PROCESSES)((char*)myprocesslist + myprocesslist->NextEntryDelta);
			//printf("sdfs");
		}
		delete[] szBufferd;
	}
	return retRslt;
}

LONG __declspec(naked) NtCall(DWORD FunctionIndex, DWORD ClassIndex, ...) //快速调用NT函数
{
	__asm
	{
		push ebp
		mov ebp, esp
		mov eax, FunctionIndex
		mov ecx, ClassIndex
		lea edx, [ebp + 0x10]
		call fs : [0xC0]
		add esp, 0x4
		leave
		retn
	}
}
//2个Char转成Byte
BYTE HexCharToByte(char ch1, char ch2)
{
	int Srt;
	if ((ch1 >= 48) && (ch1 <= 57))				//大于0并且小于9
	{
		Srt = ch1 - 48;							//5=53  53-48=5
	}
	else if ((ch1 >= 65) && (ch1 <= 70))		//大于A并且小于F
	{
		Srt = ch1 - 65 + 10;					//D=68 68-65+10=13
	}

	if ((ch2 >= 48) && (ch2 <= 57))				//大于0并且小于9
	{
		return Srt * 16 + ch2 - 48;
	}
	else if ((ch2 >= 65) && (ch2 <= 70))
	{
		return Srt * 16 + ch2 - 65 + 10;
	}
	else
	{
		return -1;
	}
}

//byte数组转String
std::string byteToHexStr(unsigned char p[], int len)

{
	std::string hexstr = "";
	for (int i = 0; i < len; i++)
	{
		char hex1;
		char hex2;
		int value = p[i];
		int S = value / 16;
		int Y = value % 16;
		if (S >= 0 && S <= 9)
		{
			hex1 = (char)(48 + S);
		}
		else
		{
			hex1 = (char)(55 + S);
		}
		if (Y >= 0 && Y <= 9)
		{
			hex2 = (char)(48 + Y);
		}
		else
		{
			hex2 = (char)(55 + Y);
		}
		hexstr = hexstr + hex1 + hex2;
	}
	return hexstr;
}

//BIG5转换GBK
void BIG52GBK(char *szBuf)
{
	if (!strcmp(szBuf, ""))
		return;
	int nStrLen = strlen(szBuf);
	wchar_t *pws = new wchar_t[nStrLen + 1];
	__try
	{
		int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, pws, nStrLen + 1);
		BOOL bValue = false;
		nReturn = WideCharToMultiByte(936, 0, pws, nReturn, szBuf, nStrLen + 1, "?", &bValue);
		szBuf[nReturn] = 0;
	}
	__finally
	{
		delete[] pws;
	}
}

//BIG5 转换成 Unicode： 
wchar_t* BIG5ToUnicode(const char* szBIG5String)
{
	UINT nCodePage = 950; //BIG5 
	int nLength = MultiByteToWideChar(nCodePage, 0, szBIG5String, -1, NULL, 0);
	wchar_t* pBuffer = new wchar_t[nLength + 1];
	MultiByteToWideChar(nCodePage, 0, szBIG5String, -1, pBuffer, nLength);
	pBuffer[nLength] = 0;
	return pBuffer;
}

//Unicode 转换成 GB2312： 
char* UnicodeToGB2312(const wchar_t* szUnicodeString)
{
	UINT nCodePage = 936; //GB2312 
	int nLength = WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLength + 1];
	WideCharToMultiByte(nCodePage, 0, szUnicodeString, -1, pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	return pBuffer;
}


//繁体中文BIG5 转换成 简体中文 GB2312 
char* BIG5ToGB2312(const char* szBIG5String)
{
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	wchar_t* szUnicodeBuff = BIG5ToUnicode(szBIG5String);
	char* szGB2312Buff = UnicodeToGB2312(szUnicodeBuff);
	int nLength = LCMapStringA(lcid, LCMAP_SIMPLIFIED_CHINESE, (LPCSTR)szGB2312Buff, -1, NULL, 0);
	char* pBuffer = new char[nLength + 1];
	LCMapStringA(0x0804, LCMAP_SIMPLIFIED_CHINESE, (LPCSTR)szGB2312Buff, -1, (LPSTR)pBuffer, nLength);
	pBuffer[nLength] = 0;

	delete[] szUnicodeBuff;
	delete[] szGB2312Buff;
	return pBuffer;
}

BOOL ProcessInArray(ProcessResult ProcessArray, DWORD processID)
{
	BOOL retValue = FALSE;
	if (ProcessArray.size()>0)
	{
		for (size_t i = 0; i < ProcessArray.size(); i++)
		{
			if (ProcessArray[i] == processID)
			{
				retValue = TRUE;
				break;
			}
		}
	}
	return retValue;
}
