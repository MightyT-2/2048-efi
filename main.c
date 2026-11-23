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
    EFI_TIME time;

    InitializeLib(ImageHandle, SystemTable);

    uefi_call_wrapper(SystemTable->RuntimeServices->GetTime, 2, &time, 0);
    seed_rng(time.Second + time.Minute * 60 + time.Hour * 3600);

    add_number();
    add_number();   
    
    print_debug(L"Arrow keys = Movement");
    print_debug(L"Esc key    = Quit");
    print_debug(L"r          = Reset");

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
