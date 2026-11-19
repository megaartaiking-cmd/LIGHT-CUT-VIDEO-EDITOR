# LIGHT-CUT-VIDEO-EDITOR
LiteCut Pro 1.0 - offline Windows video editor | CAN RUN ON ANY WEB BREWSER
================================================
File: LiteCutPro.exe - 122 KB (0.12 MB, limit was 80 MB)
32-bit Windows GUI exe - runs on 32-bit and 64-bit Windows 10.
No internet, no account, no extra runtime to install.
Uses only Windows built-in DLLs: KERNEL32, SHELL32, ole32, USER32, msvcrt.

How to install (offline)
-------------------------
1. Copy LiteCutPro.exe to the Windows 10 PC (USB stick is fine).
2. Double-click it.
   - First run copies itself and the editor to:
     %LOCALAPPDATA%\LiteCutPro\LiteCutPro.exe
     %LOCALAPPDATA%\LiteCutPro\LiteCutPro.html
   - It adds shortcuts to Desktop and Start Menu > LiteCut Pro.
   - It opens the editor in your default browser (Edge / Chrome / Firefox)
     as a local file. No server, no internet.
3. After that, just use the Desktop shortcut.

Why it opens in the browser:
A full built-in Chromium would be 150 MB+ and need far more than
512 MB RAM. This exe is 122 KB because it uses the browser already
on Windows 10. Your video files never leave the PC.

Use the 512 MB mode in the editor for very low-memory PCs,
and export at 480p first if playback stutters.

Uninstall
----------
Run: LiteCutPro.exe /uninstall
Or delete %LOCALAPPDATA%\LiteCutPro and the shortcuts.

Note: this exe was built and verified on Linux (PE header, imports,<img width="908" height="612" alt="kight cut pro" src="https://github.com/user-attachments/assets/06ba3227-7838-4062-b7d8-3b940e0eccea" />

embedded editor, size). It was not double-click tested on Windows
here, so please try the demo project first on your PC and tell
Alya if anything fails to open.
