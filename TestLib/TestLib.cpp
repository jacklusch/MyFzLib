// TestLib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include "../MyFzLib/static_Function.h"
//#include "../MyFzLib/mhook-lib/mhook.h"
//#include <Windows.h>
//HOOK����Գɹ�
//
typedef int (WINAPI *MymessageBox)(HWND,LPCSTR,LPCSTR,UINT);
int WINAPI NewMeesageBox(HWND, LPCSTR, LPCSTR, UINT);

MymessageBox oldMessage;
//int main()
//{
//	//MessageBoxA();
//	HMODULE hm=LoadLibraryA("user32.dll");
//	if (hm==NULL)
//	{
//		DbgPrtA("LoadLibraryAʧ��");
//		return 0;
//	}
//	oldMessage = (MymessageBox)GetProcAddress(hm, "MessageBoxA");
//	if (oldMessage == NULL)
//	{
//		DbgPrtA("oldMessageʧ��");
//		return 0;
//	}
//	Mhook_SetHook((PVOID*)&oldMessage,(PVOID) NewMeesageBox);
//	int ad=MessageBoxA(0, "sssss", "1111", 1);
//	int * bb = &ad;
//	int c = dd(bb);
//	SendMessageA(0,0,0,0);
//    return 0;
//}
//int WINAPI NewMeesageBox(HWND hwnd, LPCSTR text, LPCSTR caption, UINT id)
//{
//	DbgPrtA("NewMeesageBox %08x...%s....%s...%d", hwnd,text,caption,id);
//	int retad = oldMessage(hwnd, text, caption, id);
//	DbgPrtA("oldMessage������%d", retad);
//	return retad += 1;
//}
typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWCH   Buffer;
} UNICODE_STRING,*PUNICODE_STRING;
typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES,*POBJECT_ATTRIBUTES;
#include   <windows.h>   
//_Out_ PHANDLE            ThreadHandle,
//_In_  ACCESS_MASK        DesiredAccess,
//_In_  POBJECT_ATTRIBUTES ObjectAttributes,
//_In_  PCLIENT_ID         ClientId
typedef int (WINAPI *ntopenthread)(PHANDLE ThreadHandle, DWORD DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);
ntopenthread ntOpenThread;
int main()
{
	HMODULE hm=LoadLibraryA("c:\\windows\\system32\\ntkrnlpa.exe");
	ntOpenThread = (ntopenthread)GetProcAddress(hm, "NtOpenThread");
	HANDLE h;
	CLIENT_ID cid = { 0 };
	cid.UniqueThread = (PVOID)GetCurrentThreadId();
	ntOpenThread(&h, 0, NULL, &cid);
	return 0;
}