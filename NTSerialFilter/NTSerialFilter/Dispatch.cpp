//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The dispatch routine for irp.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below functions.
//              a.1. DefaultDispatchRoutine();
//******************************************************************************

#include "Launch.h"

#pragma PAGEDCODE
NTSTATUS DefaultDispatchRoutine(IN PDEVICE_OBJECT pstDeviceObject,
                                IN PIRP pstIrp)
{
    PAGED_CODE();
    KdPrint(("Enter DefaultDispatchRoutine\n"));

    do
    {
        PIO_STACK_LOCATION pstStackLocation = 
            IoGetCurrentIrpStackLocation(pstIrp);
        if (NULL == pstStackLocation)
        {
            KdPrint(("The current stack location is NULL.\r\n"));
            break;
        }
        PDEVICE_EXTENSION pstDeviceExtension = 
            (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;

        // Power irp.
        if (IRP_MJ_POWER == pstStackLocation->MajorFunction)
        {
            PoStartNextPowerIrp(pstIrp);
            IoSkipCurrentIrpStackLocation(pstIrp);
            return PoCallDriver(pstDeviceExtension->pstNextDeviceObject_, 
                                pstIrp);
        }

        // Write irp.
        if (IRP_MJ_WRITE == pstStackLocation->MajorFunction)
        {
            ULONG ulLen = pstStackLocation->Parameters.Write.Length;
            PUCHAR pucBuffer = NULL;
            if (NULL != pstIrp->MdlAddress)
            {
                pucBuffer =
                    (PUCHAR)MmGetSystemAddressForMdlSafe(pstIrp->MdlAddress,
                                                         NormalPagePriority);
            }
            else if (NULL != pstIrp->UserBuffer)
            {
                pucBuffer = (PUCHAR)pstIrp->UserBuffer;
            }
            else
            {
                pucBuffer = (PUCHAR)pstIrp->AssociatedIrp.SystemBuffer;
            }

            if (NULL == pucBuffer)
            {
                KdPrint(("All buffer are NULL.\r\n"));
                break;
            }

            for (ULONG cntI = 0; cntI < ulLen; cntI++)
            {
                KdPrint(("COM Send Data: %c\r\n", pucBuffer[cntI]));
            }
        } //! if 'Write irp' END

        IoSkipCurrentIrpStackLocation(pstIrp);
        return IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
    } while (FALSE);

    KdPrint(("Leave DefaultDispatchRoutine\n"));
    pstIrp->IoStatus.Status = STATUS_INVALID_PARAMETER;
    pstIrp->IoStatus.Information = 0;
    IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
} //! DefaultDispatchRoutine() END
