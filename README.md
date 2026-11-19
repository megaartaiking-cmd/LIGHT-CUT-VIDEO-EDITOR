# LiteCut Pro — Lightweight Offline Video Editor for Windows 10

> **LiteCut Pro 1.0 — a lightweight, offline video editor for Windows 10 that runs on low-end PCs, including 512 MB RAM and single-core processors. Trim, split, add titles, apply filters, and export to WebM / MP4. No internet, no account, files never leave your PC.**

![LiteCut Pro screenshot](https://github.com/user-attachments/assets/06ba3227-7838-4062-b7d8-3b940e0eccea)

LiteCut Pro is a Premiere Pro–style video editor built to work where heavy editors cannot: old Windows 10 PCs with very little RAM. The Windows installer is a 122 KB, 32-bit `.exe` — it installs locally, opens the editor in the browser already on your PC, and needs no internet connection.

## Why LiteCut Pro?

- **Runs on low-end PCs** — designed for Windows 10 PCs with 512 MB RAM and a single-core processor older than 2010. Includes a **512 MB mode** with quarter-resolution preview and fast 480p exports.
- **Completely offline** — no internet, no account, no extra runtime to install. Your video files never leave your PC.
- **Tiny installer** — `LiteCutPro.exe` is **122 KB (0.12 MB)**, 32-bit, and runs on 32-bit and 64-bit Windows 10. It uses only Windows built-in DLLs.
- **Familiar editing** — 3-track timeline, trim, split, titles, colour correction, transitions, and export, in a Premiere-style layout.

## Features

- **3-track timeline:** V2 titles, V1 video, A1 audio — drag clips to move them
- **Editing:** trim, split, ripple, duplicate, delete, undo / redo
- **Colour:** colour-correction sliders plus 10 filter presets
- **Transitions:** 7 transitions, plus blend-mode overlays
- **Titles & subtitles:** add titles and subtitles to any clip
- **Audio & speed:** speed control, volume and fades
- **Projects:** save and open projects as a file, plus a one-click demo project
- **Export:** render in-browser to WebM / MP4 up to 1080p (canvas + MediaRecorder). Use 480p first on very low-memory PCs.

## Download & Install (Windows 10, offline)

> Note: this repo currently contains only a README and a license. Add `LiteCutPro.exe` to a GitHub Release, or add the editor files here, so visitors can actually download it — that is the single biggest SEO and conversion fix for this repo.

1. Copy `LiteCutPro.exe` to the Windows 10 PC (a USB stick is fine).
2. Double-click it.
   - First run copies the app to `%LOCALAPPDATA%\LiteCutPro\LiteCutPro.exe` and the editor to `%LOCALAPPDATA%\LiteCutPro\LiteCutPro.html`
   - It adds shortcuts to the Desktop and Start Menu > LiteCut Pro
   - It opens the editor in your default browser (Edge / Chrome / Firefox) as a local file. No server, no internet.
3. After that, just use the Desktop shortcut.

### Why it opens in the browser

A full built-in Chromium would be 150 MB+ and need far more than 512 MB RAM. This installer is 122 KB because it uses the browser already on Windows 10.

## Usage

1. Open LiteCut Pro from the Desktop shortcut.
2. Import a video, photo, or audio file — or click the demo project to try it first.
3. Drag clips on the timeline, trim or split at the playhead, add a title, adjust colour or speed.
4. Export to WebM / MP4. On a 512 MB PC, export at 480p first if playback stutters.

## System Requirements

- Windows 10, 32-bit or 64-bit
- 512 MB RAM (512 MB mode recommended) — Windows 10 itself needs 1–2 GB, so 512 MB is the design target, not a guarantee
- Single-core processor (pre-2010 PCs supported)
- A modern browser already installed on the PC (Edge, Chrome, or Firefox)
- No internet connection required

## Uninstall

Run `LiteCutPro.exe /uninstall`, or delete `%LOCALAPPDATA%\LiteCutPro` and the Desktop / Start Menu shortcuts.

## FAQ

**Is LiteCut Pro a Premiere Pro replacement?**
It is a lightweight alternative for basic editing on low-end PCs — not a full Premiere Pro equivalent. Premiere Pro needs 8 GB+ RAM; LiteCut Pro is built for machines that cannot run it.

**Does it work offline?**
Yes — completely offline and local after install.

**Can it run in any web browser?**
Yes — the editor runs in the browser already on your PC. No install of a separate browser is needed.

**What can I export?**
WebM / MP4, up to 1080p. Use 480p on very low-memory PCs.

## License

See [LICENSE](LICENSE).

---

**Keywords:** lightweight video editor, offline video editor, Windows 10 video editor, low-end PC video editor, 512 MB RAM video editor, Premiere Pro alternative, free video editor for old PC, WebM MP4 video editor
