//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The header file of dispatch routine.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below function's declare.
//              a.1. DefaultDispatchRoutine();
//******************************************************************************

#pragma once
#ifndef DISPATCH_H_
#define DISPATCH_H_

_Dispatch_type_(IRP_MJ_CLOSE)
_Dispatch_type_(IRP_MJ_WRITE)
_Dispatch_type_(IRP_MJ_READ)
_Dispatch_type_(IRP_MJ_CREATE)
_Dispatch_type_(IRP_MJ_CREATE_NAMED_PIPE)
NTSTATUS DefaultDispatchRoutine(IN PDEVICE_OBJECT pstDeviceObject,
                                IN PIRP pstIrp);

#endif // !DISPATCH_H_
