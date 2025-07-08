#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <string>

uintptr_t GetModuleBaseAddress(DWORD procId) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32FirstW(hSnap, &modEntry)) {
            // Das erste Modul ist i.d.R. das Main-Module
            modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
        }
        CloseHandle(hSnap);
    }
    return modBaseAddr;
}

int main() {
    DWORD pid;
    std::cout << "Enter PID: ";
    std::cin >> pid;
    std::cin.ignore(); // Leert Eingabepuffer

    uintptr_t base = GetModuleBaseAddress(pid);
    if (base == 0) {
        std::cout << "Base address not found!\n";
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    std::ofstream out("base_address.txt");
    out << std::hex << base;
    out.close();
    std::cout << "Base address written to base_address.txt: 0x" << std::hex << base << std::endl;

    std::cout << "Done. Press Enter to exit...";
    std::cin.get();
    return 0;
}
