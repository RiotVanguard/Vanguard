//
// *OFFICIAL* Riot Vanguard source code.
//

#include <ntddk.h>

//
// Because we don't know how PE files work.
//

NTKERNELAPI
PVOID
NTAPI
RtlFindExportedRoutineByName(
    _In_ PVOID ImageBase,
    _In_ PCCH RoutineNam
);

_IRQL_requires_same_ _IRQL_requires_(PASSIVE_LEVEL)
void 
DriverUnload(
    _In_ PDRIVER_OBJECT DriverObject
    ) 
/*++

Routine Description:

    This is the unload routine for the Vanguard kernel driver.

Arguments:

    DriverObject - Pointer to driver object created by the system.

Return Value:

    None

--*/
{
    UNREFERENCED_PARAMETER(DriverObject);

    //
    // Bye~
    //
}

_IRQL_requires_same_ _IRQL_requires_(PASSIVE_LEVEL)
NTSTATUS 
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject, 
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    This is the entry routine for the Vanguard kernel driver.

Arguments:

    DriverObject - Pointer to driver object created by the system.

    RegistryPath - Receives the full registry path to the SERVICES 
            node of the current control set.

Return Value:

    An NTSTATUS code.

--*/
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    DriverObject->DriverUnload = DriverUnload;

    UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\vgk_PLZNOHACK");
    PFILE_OBJECT FileObject = NULL;
    PDEVICE_OBJECT DeviceObject = NULL;
    if (NT_SUCCESS(IoGetDeviceObjectPointer(&DeviceName, FILE_READ_DATA, &FileObject, &DeviceObject)))
    {
        const PDRIVER_OBJECT VgkDriverObject = DeviceObject->DriverObject;
        if (VgkDriverObject && VgkDriverObject->DriverStart)
        {
            //
            // Executing Vanguard kernel.
            //
            ((void(*)())(RtlFindExportedRoutineByName(VgkDriverObject->DriverStart, "Egg")))();

            Status = STATUS_SUCCESS;
        }
        
        ObDereferenceObject(FileObject);
    }

    return Status;
}