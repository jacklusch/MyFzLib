#pragma once
#include <Windows.h>
#include <string>
//�����ϳ�����strtohex������ԴΪĳCSDN����
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
//�����ϳ�����HexToStr������ԴΪĳCSDN����
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//�Լ�ʵ�ֵ��滻�������е�ͨ�����//���������е�??תΪ00
void ConfigChar(char* szIdecode);
//�Լ�ʵ�ֵ��ж�2��2���ƴ��Ƿ����,���ֽڱȽ�,ͳ����ͬ�����Ƿ���ڴ���ĳ���,�����������ĳ���ֽڵ���0 �Զ�Ĭ��Ϊ���
//��ǰ�ĵ�������
//				//ȡ����һ�κ������볤����ͬ���������������ֽڱȽ�,����??�����00Ĭ 5
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
