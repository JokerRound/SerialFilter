//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-01-26
// Description: 
//      The macroes was shared in this probject.
//
// Modify Log:
//      2019-01-26    Hoffman
//      Info: a. Add below mecroes
//              a.1. PAGEDCODE;
//              a.2. LOCKEDCODE;
//              a.2. INITCODE; 
//              a.2. PAGEDDATA; 
//              a.2. LOCKEDDATA; 
//              a.2. INITDATA; 
//              a.2. ARRAY_SIZE;
//              a.2. DEVICE_OBJECT_LIST_TAG;
//              a.2. SERIAL_PORT_NUMBER;
//******************************************************************************

#pragma once
#ifndef MACROSHARE_H_
#define MACROSHARE_H_

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define ARRAY_SIZE(p) (sizeof(p) / sizeof(p)[0])
#define DEVICE_OBJECT_LIST_TAG 'DOLT'

#define SERIAL_PORT_NUMBER 2
#endif // !MACROSHARE_H_