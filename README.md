# 3DEngine

This is a minimal Win32 C++ application that creates a window using the Windows API.

## Files
- `WinMain.cpp`: Main source file containing the WinMain entry point and window creation code.
- `WinMain.exe`: Compiled executable (generated after building).

## How to Build

### Using MinGW (g++)
1. Open a terminal in this directory.
2. Run:
   ```powershell
   g++ WinMain.cpp -o WinMain.exe -mwindows
   ```

### Using MSVC (cl)
1. Open a Developer Command Prompt for Visual Studio.
2. Run:
   ```powershell
   cl WinMain.cpp /link user32.lib
   ```

## How to Run

After building, run the executable:
```powershell
./WinMain.exe
```

A window titled "Happy Window" should appear.

## Notes
- The code uses Unicode Windows API. Ensure `UNICODE` and `_UNICODE` are defined before including `<Windows.h>`.
- The message loop is required for the window to remain open and responsive.
