#pragma once
#include <Windows.h>
#include <string>
#include <vector>
//用到的结构的定义
typedef struct _myfindrecod
{
	int ndNum;//序号
	DWORD dwAddr; //地址
}MyFindRecod,*PMyFindRecod;
typedef std::vector<MyFindRecod> FindArray;


//网络上抄来的strtohex函数来源为某CSDN博客
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
//网络上抄来的HexToStr函数来源为某CSDN博客
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//自己实现的替换特征码中的通配符号//把特征码中的??转为00
void ConfigChar(char* szIdecode);
//自己实现的判断2段2进制串是否相等,逐字节比较,统计相同个数是否等于传入的长度,如果特征码中某个字节等于0 自动默认为相等
FindArray Fun_FindIdent(void* szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout=1);
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
