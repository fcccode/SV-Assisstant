//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                          Copyright 2007 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#define OLS_DRIVER_INSTALL			1
#define OLS_DRIVER_REMOVE			2
#define OLS_DRIVER_SYSTEM_INSTALL	3
#define	OLS_DRIVER_SYSTEM_UNINSTALL	4

BOOL ManageDriver(LPCTSTR DriverId, LPCTSTR DriverPath, USHORT Function);

BOOLEAN
SetupDriverName(
	_Inout_updates_all_(BufferLength) TCHAR* DriverLocation,
	_In_ ULONG BufferLength
);

#define DRIVER_NAME					_T("ZhaoxinRing0.sys")
#define DRIVER_ID					_T("ZhaoxinRing0")
#define DRIVER_INF_NAME				L"ZhaoxinRing0.inf"

BOOL Initialize();