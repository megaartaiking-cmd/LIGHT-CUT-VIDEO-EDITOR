# Building LiteCutPro.exe (Windows, offline)

LiteCut Pro 1.0 is the editor in `LiteCutPro.html` plus a tiny 32-bit
Windows launcher in `src/main.c`.

The launcher embeds `LiteCutPro.html`, installs it to
`%LOCALAPPDATA%\LiteCutPro`, creates Desktop / Start Menu shortcuts,
and opens the editor as a local file in the default browser.
No internet, no server, no extra runtime. The `.exe` is 122 KB.

## Files

- `LiteCutPro.html` — the full editor (3-track timeline, trim/split,
  titles, filters, transitions, export to WebM/MP4)
- `src/main.c` — Windows launcher / installer (32-bit, GUI)
- `src/version.rc` — version info for the exe
- `README-INSTALL.txt` — install / uninstall instructions

## Build on Linux (cross-compile for Windows)

```sh
# 1. Generate the embedded HTML header from LiteCutPro.html
python3 -c "
data = open('LiteCutPro.html','rb').read()
with open('litecut_html.h','w') as f:
    f.write('static const unsigned char litecut_html[] = {\n')
    for i in range(0, len(data), 12):
        f.write('  ' + ','.join(str(b) for b in data[i:i+12]) + ',\n')
    f.write('};\n')
    f.write(f'static const unsigned int litecut_html_len = {len(data)};\n')
"

# 2. Install the MinGW cross-compiler (Debian/Ubuntu)
sudo apt-get install -y gcc-mingw-w64-i686

# 3. Compile the 32-bit exe
i686-w64-mingw32-windres src/version.rc -o version.o
i686-w64-mingw32-gcc -Os -s -mwindows -municode src/main.c version.o \
  -o LiteCutPro.exe -lole32 -lshell32 -luuid
```

Result: `LiteCutPro.exe`, ~122 KB, PE32 GUI, Intel 80386.
It runs on 32-bit and 64-bit Windows 10.

## Or just use the editor directly

Open `LiteCutPro.html` in Edge, Chrome, or Firefox. No install needed.
Use the 512 MB mode for very low-memory PCs.
