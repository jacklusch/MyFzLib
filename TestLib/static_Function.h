#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <winnt.h>
#include <winternl.h>
#include <string>
#include <vector>

//用到的结构的定义
typedef std::vector<DWORD> ProcessResult;
typedef enum _KWAIT_REASON {
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVirtualMemory,
	WrPageOut,
	WrRendezvous,
	WrKeyedEvent,
	WrTerminated,
	WrProcessInSwap,
	WrCpuRateControl,
	WrCalloutStack,
	WrKernel,
	WrResource,
	WrPushLock,
	WrMutex,
	WrQuantumEnd,
	WrDispatchInt,
	WrPreempted,
	WrYieldExecution,
	WrFastMutex,
	WrGuardedMutex,
	WrRundown,
	MaximumWaitReason
} KWAIT_REASON;
typedef struct _VM_COUNTERS
{
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG  WaitTime;
	PVOID  StartAddress;
	CLIENT_ID  ClientID;
	KPRIORITY  Priority;
	KPRIORITY  BasePriority;
	ULONG  ContextSwitchCount;
	ULONG  ThreadState;
	KWAIT_REASON WaitReason;
#ifdef _WIN64
	ULONG  Reserved; //Add
#else
#endif
}SYSTEM_THREADS, *PSYSTEM_THREADS;
typedef struct _SYSTEM_PROCESSES
{
	ULONG  NextEntryDelta;
	ULONG  ThreadCount;
	ULONG  Reserved[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY  BasePriority;
	HANDLE  ProcessId;  //Modify
	HANDLE  InheritedFromProcessId;//Modify
	ULONG  HandleCount;
	ULONG  SessionId;
	ULONG_PTR PageDirectoryBase;
	VM_COUNTERS VmCounters;
	SIZE_T  PrivatePageCount;//Add
	IO_COUNTERS IoCounters; //windows 2000 only
	struct _SYSTEM_THREADS Threads[1];
}SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;





typedef struct _myfindrecod
{
	int ndNum;//序号
	DWORD dwAddr; //地址
}MyFindRecod,*PMyFindRecod;
enum 搜索类型
{
	地址=0,
	偏移,
	基地址,
	CALL
};
LONG  NtCall(DWORD FunctionIndex, DWORD ClassIndex, ...); //快速调用NT函数;
/*定义函数指针*/
//typedef NTSTATUS(WINAPI*  MyZwQuerySystemInformation)(int, PVOID, ULONG, PULONG);
#define MyNtKillTimer(SectionHandle,dwTimeId)  NtCall(0x101B, 0x0,SectionHandle,dwTimeId);
#define MyNtQuerySystemInfomation(ndclass , SystemBasicInfo, infozie, presult) NtCall(0x33,0,ndclass,SystemBasicInfo,infozie,presult);

/*定义函数指针结束*/
/*重定义操作符*/
typedef std::vector<MyFindRecod> FindArray;

/*//网络上抄来的strtohex函数来源为某CSDN博客*/
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
 /*网络上抄来的HexToStr函数来源为某CSDN博客*/
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//自己实现的替换特征码中的通配符号//把特征码中的??转为00
void ConfigChar(char* szIdecode);
//特征码搜索,参数1=起始地址,参数2=搜索大小,参数3=特征码字符串,支持??通配符,参数4=搜索次数
DWORD Fun_FindIdent(void* szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout=1);
//本进程内搜索特征码,参数1=模块名,参数2=特征码字符串,支持??通配符,参数3=搜索次数
DWORD Fun_FindIdentByModule(char* szModule,char * szIdent, int ndOffset = 0, DWORD dwCout = 1);
//其他进程内搜索特征码
DWORD Fun_FindIdentByModule2(DWORD dwProcessId, char* szModule, char * szIdent, int ndOffset=0, DWORD dwCout = 1,DWORD SearChtype=0);
//自己实现的判断2段2进制串是否相等,逐字节比较,统计相同个数是否等于传入的长度,如果特征码中某个字节等于0 自动默认为相等
bool isEqueByte(BYTE* szSource, BYTE* szIdecode, DWORD dwlen);
//进程名取ID
DWORD FindProcess(const std::string& Name);
//名取模块信息,返回模块结构
MODULEENTRY32 GetModuleByName(DWORD dwProcessId, char* szModuleName);
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
char* gateMy(WCHAR* str);
std::string read_reg_sz( char* KeyPath,char * Key);
//根据进程名返回所有这个名字的进程ID
ProcessResult GetProcessByName(char * szProcessName);
//2个Char转成Byte
BYTE HexCharToByte(char ch1, char ch2);
//byte数组转String
std::string byteToHexStr(unsigned char p[], int len);
//BIG5转换GBK
void BIG52GBK(char *szBuf);
//BIG5 转换成 Unicode： 
wchar_t* BIG5ToUnicode(const char* szBIG5String);
//Unicode 转换成 GB2312： 
char* UnicodeToGB2312(const wchar_t* szUnicodeString);
//繁体中文BIG5 转换成 简体中文 GB2312 
char* BIG5ToGB2312(const char* szBIG5String);
//判断一个进程ID是否在一个进程数组里
BOOL ProcessInArray(ProcessResult ProcessArray,DWORD processID);