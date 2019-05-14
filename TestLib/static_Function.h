#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <winnt.h>
#include <winternl.h>
#include <string>
#include <vector>

//�õ��Ľṹ�Ķ���
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
	int ndNum;//���
	DWORD dwAddr; //��ַ
}MyFindRecod,*PMyFindRecod;
enum ��������
{
	��ַ=0,
	ƫ��,
	����ַ,
	CALL
};
LONG  NtCall(DWORD FunctionIndex, DWORD ClassIndex, ...); //���ٵ���NT����;
/*���庯��ָ��*/
//typedef NTSTATUS(WINAPI*  MyZwQuerySystemInformation)(int, PVOID, ULONG, PULONG);
#define MyNtKillTimer(SectionHandle,dwTimeId)  NtCall(0x101B, 0x0,SectionHandle,dwTimeId);
#define MyNtQuerySystemInfomation(ndclass , SystemBasicInfo, infozie, presult) NtCall(0x33,0,ndclass,SystemBasicInfo,infozie,presult);

/*���庯��ָ�����*/
/*�ض��������*/
typedef std::vector<MyFindRecod> FindArray;

/*//�����ϳ�����strtohex������ԴΪĳCSDN����*/
void StrToHex(BYTE *pbDest, char *pszSrc, int nLen);
 /*�����ϳ�����HexToStr������ԴΪĳCSDN����*/
void HexToStr(char *pszDest, BYTE *pbSrc, int nLen);
//�Լ�ʵ�ֵ��滻�������е�ͨ�����//���������е�??תΪ00
void ConfigChar(char* szIdecode);
//����������,����1=��ʼ��ַ,����2=������С,����3=�������ַ���,֧��??ͨ���,����4=��������
DWORD Fun_FindIdent(void* szSource, DWORD dwSearchSize, char * szIdent, DWORD dwCout=1);
//������������������,����1=ģ����,����2=�������ַ���,֧��??ͨ���,����3=��������
DWORD Fun_FindIdentByModule(char* szModule,char * szIdent, int ndOffset = 0, DWORD dwCout = 1);
//��������������������
DWORD Fun_FindIdentByModule2(DWORD dwProcessId, char* szModule, char * szIdent, int ndOffset=0, DWORD dwCout = 1,DWORD SearChtype=0);
//�Լ�ʵ�ֵ��ж�2��2���ƴ��Ƿ����,���ֽڱȽ�,ͳ����ͬ�����Ƿ���ڴ���ĳ���,�����������ĳ���ֽڵ���0 �Զ�Ĭ��Ϊ���
bool isEqueByte(BYTE* szSource, BYTE* szIdecode, DWORD dwlen);
//������ȡID
DWORD FindProcess(const std::string& Name);
//��ȡģ����Ϣ,����ģ��ṹ
MODULEENTRY32 GetModuleByName(DWORD dwProcessId, char* szModuleName);
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
char* gateMy(WCHAR* str);
std::string read_reg_sz( char* KeyPath,char * Key);
//���ݽ�������������������ֵĽ���ID
ProcessResult GetProcessByName(char * szProcessName);
//2��Charת��Byte
BYTE HexCharToByte(char ch1, char ch2);
//byte����תString
std::string byteToHexStr(unsigned char p[], int len);
//BIG5ת��GBK
void BIG52GBK(char *szBuf);
//BIG5 ת���� Unicode�� 
wchar_t* BIG5ToUnicode(const char* szBIG5String);
//Unicode ת���� GB2312�� 
char* UnicodeToGB2312(const wchar_t* szUnicodeString);
//��������BIG5 ת���� �������� GB2312 
char* BIG5ToGB2312(const char* szBIG5String);
//�ж�һ������ID�Ƿ���һ������������
BOOL ProcessInArray(ProcessResult ProcessArray,DWORD processID);