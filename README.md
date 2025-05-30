# 3DEngine

This is a minimal Win32 C++ application that creates a window using the Windows API. It serves as a foundational example for understanding core Win32 programming concepts.

## Files
- `WinMain.cpp`: Main source file containing the `WinMain` entry point, window class registration, window creation, and message loop.
- `WinMain.exe`: Compiled executable (generated after building).
- `README.md`: This file.
- `notes.txt`: Developer notes and learnings.

## How to Build

### Using MinGW (g++)
1. Open a PowerShell terminal in this directory.
2. Run:
   ```powershell
   g++ WinMain.cpp -o WinMain.exe -mwindows
   ```
   The `-mwindows` flag is crucial as it links against the GUI subsystem. This affects how the application is launched and how its exit code is reported (see "Important Note on Exit Codes" below).

### Using MSVC (cl)
1. Open a Developer Command Prompt for Visual Studio.
2. Run:
   ```powershell
   cl WinMain.cpp /link user32.lib
   ```

## How to Run

After building, run the executable from PowerShell:
```powershell
.\\WinMain.exe
```
A window titled "Happy Window" should appear. To close the application, click the close button (X) on the window.

## Important Note on Exit Codes (PowerShell & -mwindows)

When you compile with `-mwindows` (to create a GUI application) and run the executable from PowerShell, PowerShell launches the GUI application asynchronously. This means PowerShell doesn't wait for the GUI application to terminate before returning control to the user. 

As a result, `echo $LASTEXITCODE` in PowerShell immediately after launching `.\\WinMain.exe` will typically show `0` (PowerShell's own exit code for successfully launching the process), **not** the exit code returned by `WinMain.exe` (e.g., the `22` from `PostQuitMessage(22)`).

To reliably get the exit code of a GUI application launched this way, you would need to use a different method, such as starting the process with `Start-Process -Wait`:

```powershell
Start-Process .\\WinMain.exe -Wait
echo $LASTEXITCODE 
```
This will make PowerShell wait for `WinMain.exe` to close, and then `$LASTEXITCODE` will reflect the actual exit code of your application.

## Key Win32 Concepts Demonstrated

This project illustrates several fundamental Win32 API concepts:

-   **Entry Point (`WinMain`)**: The `WinMain` function serves as the primary entry point for Windows GUI applications, analogous to `main` in console applications. It's where the application initializes and begins execution.
-   **Window Class Registration (`WNDCLASSEX`, `RegisterClassEx`)**: Before a window can be created, its "class" must be registered with the system. This involves defining properties like the window procedure (`lpfnWndProc`), style (`style`), icon, cursor, and class name using the `WNDCLASSEX` structure and `RegisterClassEx` function.
-   **Window Creation (`CreateWindowEx`)**: Once the class is registered, an instance of the window is created using `CreateWindowEx`. This function specifies the window's class name, title, style, size, position, parent window, and menu.
-   **Window Styles (`WS_OVERLAPPEDWINDOW`, etc.)**: Various styles (e.g., `WS_CAPTION`, `WS_SYSMENU`, `WS_MINIMIZEBOX`) determine the window's appearance and behavior (e.g., whether it has a title bar, system menu, minimize button).
-   **Message Loop (`GetMessage`, `TranslateMessage`, `DispatchMessage`)**: The heart of a GUI application. The message loop retrieves messages from the application's message queue, translates virtual-key messages into character messages, and dispatches messages to the appropriate window procedure for processing.
-   **Window Procedure (`WndProc`)**: A callback function that processes messages sent to a window. It handles events like `WM_CLOSE` (when the user attempts to close the window), `WM_PAINT` (when the window needs to be redrawn), and user input messages.
-   **Message Handling (e.g., `WM_CLOSE`, `PostQuitMessage`)**: Specific messages trigger actions. For example, `WM_CLOSE` is typically handled by calling `PostQuitMessage`, which posts a `WM_QUIT` message to the application's message queue, signaling the message loop to terminate.
-   **Error Handling**: It's crucial to check the return values of Win32 API functions (e.g., `RegisterClassEx`, `CreateWindowEx`) and handle potential failures gracefully, often by displaying an error message and exiting cleanly.

## General Notes
- The code uses Unicode Windows API functions and strings. Ensure `UNICODE` and `_UNICODE` are defined before including `<Windows.h>` for this to work correctly.
- The message loop is essential for the window to remain open, responsive to user input, and process system events.
