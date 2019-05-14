#pragma once
LONG NtCall(DWORD FunctionIndex, DWORD ClassIndex, ...);
#define NtProtectVirtualMemory(ProcessHandle,BaseAddress,RegionSize,NewProtectWin32,OldProtect) NtCall(0x4d, 0x0, ProcessHandle,BaseAddress,RegionSize,NewProtectWin32,OldProtect);
#define NtCreateThreadEx(ThreadHandle,DesiredAccess,ObjectAttributes,ProcessHandle,lpStartAddress,lpParameter,CreateSuspended,dwStackSize,Unknown1,Unknown2,Unknown3) NtCall(0xA5, 0x0, ThreadHandle,DesiredAccess,ObjectAttributes,ProcessHandle,lpStartAddress,lpParameter,CreateSuspended,dwStackSize,Unknown1,Unknown2,Unknown3);
#define NtOpenSection(SectionHandle,DesiredAccess,ObjectAttributes) NtCall(0x34, 0x0,SectionHandle,DesiredAccess,ObjectAttributes);
#define NtMapViewOfSection(SectionHandle,ProcessHandle,BaseAddress,ZeroBits, CommitSize,SectionOffset,ViewSize,InheritDisposition,AllocationType,Protect)NtCall(0x25, 0x0,SectionHandle,ProcessHandle,BaseAddress,ZeroBits, CommitSize,SectionOffset,ViewSize,InheritDisposition,AllocationType,Protect)
#define NtSetTimer(SectionHandle,dwTimeId,dwValue,TimeProc)  NtCall(0x1018, 0x0,SectionHandle,dwTimeId,dwValue,TimeProc);
#define NtKillTimer(SectionHandle,dwTimeId)  NtCall(0x101B, 0x0,SectionHandle,dwTimeId);