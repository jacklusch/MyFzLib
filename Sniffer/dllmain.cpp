// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "../MyFzLib/mhook-lib/mhook.h"
#include "../MyFzLib/static_Function.h"
#include <WinSock2.h>

//#in
int ndFlag = 0;
typedef int (WINAPI *Myrecv)(SOCKET s, char FAR * buf, int len, int flags);
Myrecv G_recv = NULL;



int WSAAPI newrecv
(
	_In_ SOCKET s,
	_Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR * buf,
	_In_ int len,
	_In_ int flags
)
{
	DbgPrtA("wjj aaaaaaaaaa");
	int ndret=G_recv(s, buf, len, flags);
	if (ndret>0)
	{
		int mylen = ndret*2+1;
		char * szDest = new char[mylen];
		memset(szDest, 0, mylen);
		HexToStr(szDest, (BYTE*)buf, ndret);
		DbgPrtA("wjj �յ���,����=%d,������=%s", ndret, szDest);
		delete [] szDest;
		//szDest = NULL;
	}
		
	return ndret;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (ndFlag==0)
		{
			//OutputDebugStringA("111111111");
			ndFlag = 1;
			HMODULE hm=GetModuleHandleA("ws2_32.dll");
			if (hm== NULL)
			{
				DbgPrtA("wjj ��ȡģ��ʧ��!!");
			}
			DbgPrtA("wjj ��ȡģ��ɹ�!!");
			G_recv=(Myrecv)GetProcAddress(hm, "recv");
			if (G_recv==NULL)
			{
				DbgPrtA("wjj ��ȡ����ʧ��!!");
			}
			DbgPrtA("wjj ��ȡ�����ɹ�!!");
			Mhook_SetHook((PVOID*)&G_recv, (PVOID)newrecv);
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

