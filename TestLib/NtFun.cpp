#include "stdafx.h"
#include "NtFun.h"
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
