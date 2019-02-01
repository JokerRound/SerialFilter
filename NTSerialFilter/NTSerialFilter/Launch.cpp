//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The init active of driver.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below functions.
//              a.1. DriverEntry();
//              a.2. DriverUnload();
//              a.3. OpenCom();
//              a.4. CreateDevice();
//******************************************************************************

#include "Launch.h"

#pragma INITCODE
extern "C"
_Use_decl_annotations_
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pstDriverObject,
                     IN PUNICODE_STRING pstRegistryPath)
{
    PAGED_CODE();
    NTSTATUS status;
    KdPrint(("Enter DriverEntry\n"));

    pstDriverObject->DriverUnload = DriverUnload;
    for (ULONG cntI = 0; cntI < IRP_MJ_MAXIMUM_FUNCTION; cntI++)
    {
        pstDriverObject->MajorFunction[cntI] = DefaultDispatchRoutine;
    }

    status = CreateDevice(pstDriverObject);
    
    KdPrint(("DriverEntry end\n"));
    return status;
} //! DriverEntry() END

#pragma PAGEDCODE
VOID DriverUnload(IN PDRIVER_OBJECT pstDriverObject)
{
    PAGED_CODE();

    PDEVICE_OBJECT pstNextDeviceObject = NULL;
    PDEVICE_EXTENSION pstDeviceExtension = NULL;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    ULONG ulDeviceObjectNumber = 0;
    PDEVICE_OBJECT *apstDeviceObejctList = NULL;
    ULONG ulDeiveObjectListSize = 0;

    KdPrint(("Enter DriverUnload\n"));

    // Delete device and symbol.
    do
    {
        // Get number of device object.
        ntStatus = IoEnumerateDeviceObjectList(pstDriverObject,
                                               NULL,
                                               0,
                                               &ulDeviceObjectNumber);
        ulDeiveObjectListSize = sizeof(PDEVICE_OBJECT) * ulDeviceObjectNumber;

        // Allocate memory.
        apstDeviceObejctList = 
            (PDEVICE_OBJECT *)
            ExAllocatePoolWithTag(PagedPool,
                                  ulDeiveObjectListSize,
                                  DEVICE_OBJECT_LIST_TAG);
        if (NULL == apstDeviceObejctList)
        {
            KdPrint(("ExAllocatePoolWithTag failed.\r\n"));
            break;
        }

        // Get device object list.
        ntStatus = IoEnumerateDeviceObjectList(pstDriverObject,
                                               apstDeviceObejctList,
                                               ulDeiveObjectListSize,
                                               &ulDeviceObjectNumber);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("IoEnumerateDeviceObjectList failed.\r\n"));
            break;
        }

        for (ULONG cntI = 0; cntI < ulDeviceObjectNumber; cntI++)
        {
            // Normal check.
            if (NULL == apstDeviceObejctList[cntI])
            {
                continue;
            }

            ObDereferenceObject(apstDeviceObejctList[cntI]);
            pstDeviceExtension = 
                (PDEVICE_EXTENSION)apstDeviceObejctList[cntI]->DeviceExtension;

            if (NULL != pstDeviceExtension &&
                NULL != pstDeviceExtension->pstNextDeviceObject_)
            {
                IoDetachDevice(pstDeviceExtension->pstNextDeviceObject_);
            }

            IoDeleteDevice(apstDeviceObejctList[cntI]);
        }
    } while (false); //! do 'Delete device and symbol' while END 

    // Free memory.
    if (NULL != apstDeviceObejctList)
    {
        ExFreePoolWithTag(apstDeviceObejctList, DEVICE_OBJECT_LIST_TAG);
    }
} //! DriverUnload() END

#pragma PAGEDCODE
PDEVICE_OBJECT OpenCom(ULONG ulID, NTSTATUS *pNtStatus)
{
    PAGED_CODE();
    UNICODE_STRING ustrObjectName;

    UNICODE_STRING ustrDriverObjectName;
    static WCHAR wszObjectName[32] = { 0 };
    PFILE_OBJECT pstFileObject = NULL;
    PDEVICE_OBJECT pstDeviceObject = NULL;
    // Get serial port name.
    RtlStringCchPrintfW(wszObjectName, 32, L"\\Device\\Serial%d", ulID);
    RtlInitUnicodeString(&ustrObjectName, wszObjectName);

    // Open device object.
    *pNtStatus = IoGetDeviceObjectPointer(&ustrObjectName,
                                          FILE_ALL_ACCESS,
                                          &pstFileObject,
                                          &pstDeviceObject);
    if (STATUS_SUCCESS == *pNtStatus)
    {
        ObDereferenceObject(pstFileObject);
    }

    return pstDeviceObject;
} //! OpenCom() END

#pragma PAGEDCODE
NTSTATUS CreateDevice(IN PDRIVER_OBJECT pstDriverObject)
{
    PAGED_CODE();
    NTSTATUS ntStatus;
    PDEVICE_OBJECT pstDeviceObject = NULL;
    PDEVICE_EXTENSION pstDeviceExtension = NULL;
    PDEVICE_OBJECT pstSerialPortDeviceObject = NULL;

    for (size_t cntI = 0; cntI < SERIAL_PORT_NUMBER; cntI++)
    {
        pstSerialPortDeviceObject = OpenCom(cntI, &ntStatus);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("Open COM failed.\r\n"));
            continue;
        }

        ntStatus = IoCreateDevice(pstDriverObject,
                                  sizeof(*pstDeviceExtension),
                                  NULL,
                                  FILE_DEVICE_UNKNOWN,
                                  pstSerialPortDeviceObject->Characteristics,
                                  TRUE,
                                  &pstDeviceObject);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("Create Device faile.\r\n"));
            return ntStatus;
        }

        if (pstSerialPortDeviceObject->Flags & DO_BUFFERED_IO)
        {
            pstDeviceObject->Flags |= DO_BUFFERED_IO;
        }

        if (pstSerialPortDeviceObject->Flags & DO_DIRECT_IO)
        {
            pstDeviceObject->Flags |= DO_DIRECT_IO;
        }

        PDEVICE_OBJECT pstNextDeviceObject = NULL;
        ntStatus = 
            IoAttachDeviceToDeviceStackSafe(pstDeviceObject,
                                            pstSerialPortDeviceObject,
                                            &pstNextDeviceObject);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("Create Device faile.\r\n"));
            IoDeleteDevice(pstDeviceObject);
            return ntStatus;
        }

        pstDeviceExtension = 
            (PDEVICE_EXTENSION)(pstDeviceObject->DeviceExtension);
        pstDeviceExtension->pstDeviceObject_ = pstDeviceObject;
        pstDeviceExtension->pstNextDeviceObject_ = pstNextDeviceObject;

        // Set device has started.
        pstSerialPortDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    }

    return STATUS_SUCCESS;
} //! CreateDevice() END