#pragma once
#include <Windows.h>
#include <string>
//网络上抄来的strtohex函数来源为某CSDN博客
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
//网络上抄来的HexToStr函数来源为某CSDN博客
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//自己实现的替换特征码中的通配符号//把特征码中的??转为00
void ConfigChar(char* szIdecode);
//自己实现的判断2段2进制串是否相等,逐字节比较,统计相同个数是否等于传入的长度,如果特征码中某个字节等于0 自动默认为相等
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
bool isEqueByte(BYTE* szSource, BYTE* szIdecode, DWORD dwlen);
//进程名取ID
DWORD FindProcess(const std::string& Name);
//debug输出的A版本和W版本
void __cdecl DbgPrtW(PCWSTR format, ...);
void __cdecl DbgPrtA(PCSTR format, ...);
//内存读写的操作,进程内直接指针操作,直接使用#define进行重定义操作
#define dd *(DWORD*)
#define dw *(WORD*)
#define db *(BYTE*)
//主线程操作
//子类化窗口函数,需要自己实现回调函数
WNDPROC subclass_game_wndproc(HWND hWnd, LONG our_wndproc);
//卸载子类化
void	un_subclass_game_wndproc(HWND hWnd, LONG old_wndproc);
