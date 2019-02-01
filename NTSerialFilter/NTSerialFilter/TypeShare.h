//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The types was shared in this project.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below type.
//              a.1. struct tagDeviceExtention;
//******************************************************************************

#pragma once
#ifndef TYPESHARE_H_
#define TYPESHARE_H_

typedef struct tagDeviceExtention
{
    PDEVICE_OBJECT pstDeviceObject_;
    PDEVICE_OBJECT pstNextDeviceObject_;
    UNICODE_STRING ustrDeviceName_;
    UNICODE_STRING ustrSymLineName_;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#endif // !TYPESHARE_H_
