include "ntddk.h"

VOID OnUnload(){
	DbgPrint("[LotR] OnUnload Called! Cleaning up...");
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath){
	DbgPrint("[LotR] OnUnload Called! Cleaning up...");
	theDriverObject->DriverUnload = OnUnload;
	return STATUS_SUCCESS;
}