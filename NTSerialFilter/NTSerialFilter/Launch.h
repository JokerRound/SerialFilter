//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The header file of init.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below funtions's declares.
//              a.1. DriverEntry();
//              a.2. DriverUnload();
//              a.3. CreateDevice();
//******************************************************************************

#pragma once
#ifndef LAUCH_H_
#define LAUCH_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <ntifs.h>
#include <ntstrsafe.h>
#ifdef __cplusplus
}
#endif // __cplusplus
#include "TypeShare.h"
#include "MacroShare.h"
#include "Dispatch.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
DRIVER_INITIALIZE DriverEntry;
#ifdef __cplusplus
}
#endif // __cplusplus

VOID DriverUnload(IN PDRIVER_OBJECT pstDriverObject);

NTSTATUS CreateDevice(IN PDRIVER_OBJECT pstDriverObject);

#endif // !LAUCH_H_
