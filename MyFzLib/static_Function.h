#pragma once
#include <Windows.h>
#include <string>
#include <vector>
//�õ��Ľṹ�Ķ���
typedef struct _myfindrecod
{
	int ndNum;//���
	DWORD dwAddr; //��ַ
}MyFindRecod,*PMyFindRecod;
typedef std::vector<MyFindRecod> FindArray;


//�����ϳ�����strtohex������ԴΪĳCSDN����
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
//�����ϳ�����HexToStr������ԴΪĳCSDN����
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//�Լ�ʵ�ֵ��滻�������е�ͨ�����//���������е�??תΪ00
void ConfigChar(char* szIdecode);
//�Լ�ʵ�ֵ��ж�2��2���ƴ��Ƿ����,���ֽڱȽ�,ͳ����ͬ�����Ƿ���ڴ���ĳ���,�����������ĳ���ֽڵ���0 �Զ�Ĭ��Ϊ���
FindArray Fun_FindIdent(void* szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout=1);
bool isEqueByte(BYTE* szSource, BYTE* szIdecode, DWORD dwlen);
//������ȡID
DWORD FindProcess(const std::string& Name);
//debug�����A�汾��W�汾
void __cdecl DbgPrtW(PCWSTR format, ...);
void __cdecl DbgPrtA(PCSTR format, ...);
//�ڴ��д�Ĳ���,������ֱ��ָ�����,ֱ��ʹ��#define�����ض������
#define dd *(DWORD*)
#define dw *(WORD*)
#define db *(BYTE*)
//���̲߳���
//���໯���ں���,��Ҫ�Լ�ʵ�ֻص�����
WNDPROC subclass_game_wndproc(HWND hWnd, LONG our_wndproc);
//ж�����໯
void	un_subclass_game_wndproc(HWND hWnd, LONG old_wndproc);
