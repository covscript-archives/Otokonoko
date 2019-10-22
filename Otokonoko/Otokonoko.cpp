/*
 * PROJECT:   Otokonoko no Sekai
 * FILE:      Otokonoko.cpp
 * PURPOSE:   Implementation for Otokonoko no Sekai
 *
 * LICENSE:   See LICENSE in the top level directory
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifdef _In_
#define _In_
#endif 

#ifdef _In_opt_
#define _In_opt_
#endif 

#ifdef _Out_
#define _Out_
#endif 

#ifdef _Out_opt_
#define _Out_opt_
#endif 

typedef unsigned short wchar_t;

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif



/*
 * Otokonoko Platform (OP) Status
 */
typedef uint32_t OPSTATUS;

#define OPSTATUS_SUCCESS ((OPSTATUS)0x00000000)


EFI_HANDLE g_ImageHandle;
EFI_SYSTEM_TABLE* g_SystemTable;
EFI_BOOT_SERVICES* g_BootServices;

EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol;




EFI_STATUS UefiOutputString(
	_In_ EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* Instance,
	const wchar_t* String)
{
	return Instance->OutputString(Instance, const_cast<CHAR16*>(String));
}


[[noreturn]] void BugCheck()
{
	UefiOutputString(
		g_SystemTable->ConOut,
		L"\r\n"
		L"\r\n"
		L"BugCheck: Not implemented. [OPStatus = 0x000000BC]\r\n");


	for (;;) {}
}

/**
 * Entry point to UEFI Application.
 *
 * @param ImageHandle The image handle of the UEFI Application.
 * @param SystemTable A pointer to the EFI System Table.
 * @return EFI_STATUS. If the function succeeds, the return value is 
 *         EFI_SUCCESS.
 */
EFI_STATUS EFIAPI UefiMain(
	_In_ EFI_HANDLE        ImageHandle,
	_In_ EFI_SYSTEM_TABLE* SystemTable)
{
	g_ImageHandle = ImageHandle;
	g_SystemTable = SystemTable;
	g_BootServices = SystemTable->BootServices;

	// Disable UEFI watchdog timer
	g_BootServices->SetWatchdogTimer(0, 0, 0, nullptr);

	EFI_STATUS Status = EFI_SUCCESS;

	EFI_GUID GraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	Status = g_BootServices->LocateProtocol(
		&GraphicsOutputProtocolGuid,
		nullptr,
		reinterpret_cast<void**>(&g_GraphicsOutputProtocol));
	if (!EFI_ERROR(Status))
	{
		uint32_t* FrameBufferBase = reinterpret_cast<uint32_t*>(
			g_GraphicsOutputProtocol->Mode->FrameBufferBase);
		size_t FrameBufferSize = static_cast<size_t>(
			g_GraphicsOutputProtocol->Mode->FrameBufferSize);		

		for (size_t i = 0; i < FrameBufferSize; ++i)
		{
			FrameBufferBase[i] = 0xFF000000;
		}
		
	}
	else
	{
		UefiOutputString(
			g_SystemTable->ConOut,
			L"Failed to locate the Graphics Output Protocol.\r\n");
	}

	UefiOutputString(
		g_SystemTable->ConOut, 
		L"Otokonoko no Sekai Version 0.1 [Build 1]\r\n"
		L"Copyright (C) 2019 Covariant Institute. All rights reserved.\r\n"
		L"\r\n");

	UefiOutputString(
		g_SystemTable->ConOut,
		L"Hello World\r\n");

	BugCheck();

	return EFI_SUCCESS;
}

#pragma comment(linker, "/entry:UefiMain")
