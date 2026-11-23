#include <efi.h>
#include <efilib.h>
#include <efiapi.h>
#include "2048.h"
#include "rng.h"

#define WIDTH  240
#define HEIGHT 63

EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_INPUT_KEY key;
	EFI_STATUS efi_status;
    EFI_TIME time;

    InitializeLib(ImageHandle, SystemTable);

    uefi_call_wrapper(SystemTable->ConOut->OutputString, 2, SystemTable->ConOut, L"Test");

    efi_status = uefi_call_wrapper(SystemTable->RuntimeServices->GetTime, 2, &time, 0);


    // EFI_GRAPHICS_OUTPUT_PROTOCOL;
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    efi_status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(efi_status))
        print_debug(L"Unable to locate GOP");
    else
        print_debug(L"Located GOP");

    seed_rng(time.Second + time.Minute * 60 + time.Hour * 3600);
    add_number();
    add_number();   
    
    while (key.ScanCode != 0x17)
    {
        print_game();
        
        if (is_lost())
        {
            print_debug(L"Lost Game");
        }

        WaitForSingleEvent(SystemTable->ConIn->WaitForKey, 0);
        uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &key);
        
        if (key.ScanCode == 1)
        {
            if (move_up())
            {
                add_number();
            }
        }
        else if (key.ScanCode == 2)
        {
            if (move_down())
            {
                add_number();
            }
        }
        else if (key.ScanCode == 3)
        {
            if (move_right())
            {
                add_number();
            }
        }
        else if (key.ScanCode == 4)
        {
            if (move_left())
            {
                add_number();
            }
        }
        else if (key.UnicodeChar == 'r')
        {
            uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
            clear_game();
            add_number();
            add_number();
        }
    }
    
    return EFI_SUCCESS;
}

void print_pixel(int x, int y, int pixel)
{

    *((uint32_t*)(gop->Mode->FrameBufferBase + 4 * gop->Mode->Info->PixelsPerScanLine * y + 4 * x)) = pixel;
}
