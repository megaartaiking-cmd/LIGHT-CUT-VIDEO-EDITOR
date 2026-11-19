#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <stdio.h>
#include "litecut_html.h"

/* LiteCut Pro - offline Windows launcher / installer.
   Single 32-bit exe (runs on 32-bit and 64-bit Windows 10).
   No internet, no server, no installer framework.
   On first run it copies itself + the embedded editor HTML to
   %LOCALAPPDATA%\LiteCutPro, creates Desktop + Start Menu shortcuts,
   then opens the editor (a local file) in the default browser.
   All video files stay on the PC; the editor is plain HTML/JS. */

static int dir_exists(const wchar_t *p) {
    DWORD a = GetFileAttributesW(p);
    return (a != INVALID_FILE_ATTRIBUTES) && (a & FILE_ATTRIBUTE_DIRECTORY);
}

static int make_dirs(const wchar_t *p) {
    /* SHCreateDirectoryEx creates parents as needed. */
    int r = SHCreateDirectoryExW(NULL, p, NULL);
    return (r == ERROR_SUCCESS || r == ERROR_ALREADY_EXISTS || dir_exists(p));
}

static int write_file_if_changed(const wchar_t *path, const unsigned char *data, unsigned int len) {
    HANDLE h;
    DWORD written = 0;
    WIN32_FILE_ATTRIBUTE_DATA fad;
    /* Skip rewrite if an identical-size file is already there (same build). */
    if (GetFileAttributesExW(path, GetFileExInfoStandard, &fad)) {
        LARGE_INTEGER sz;
        sz.LowPart = fad.nFileSizeLow; sz.HighPart = fad.nFileSizeHigh;
        if (sz.QuadPart == (LONGLONG)len) return 1; /* already installed */
    }
    h = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    if (len > 0) WriteFile(h, data, len, &written, NULL);
    CloseHandle(h);
    return (written == len);
}

static int write_text_file(const wchar_t *path, const char *text) {
    HANDLE h = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return 0;
    WriteFile(h, text, (DWORD)strlen(text), &w, NULL);
    CloseHandle(h);
    return 1;
}

static int create_shortcut(const wchar_t *target, const wchar_t *link_path, const wchar_t *desc) {
    IShellLinkW *psl = NULL;
    IPersistFile *ppf = NULL;
    HRESULT hr;
    wchar_t workdir[MAX_PATH];
    const wchar_t *slash;
    hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLinkW, (void **)&psl);
    if (FAILED(hr) || !psl) return 0;
    psl->lpVtbl->SetPath(psl, target);
    psl->lpVtbl->SetDescription(psl, desc);
    wcscpy_s(workdir, MAX_PATH, target);
    slash = wcsrchr(workdir, L'\\');
    if (slash) workdir[slash - workdir] = L'\0';
    psl->lpVtbl->SetWorkingDirectory(psl, workdir);
    hr = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, (void **)&ppf);
    if (SUCCEEDED(hr) && ppf) {
        hr = ppf->lpVtbl->Save(ppf, link_path, TRUE);
        ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
    return SUCCEEDED(hr);
}

static int get_local_appdata(wchar_t *out, DWORD n) {
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, out))) return 1;
    if (GetEnvironmentVariableW(L"LOCALAPPDATA", out, n) > 0) return 1;
    if (GetEnvironmentVariableW(L"TEMP", out, n) > 0) return 1;
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, PWSTR pCmdLine, int nShow) {
    wchar_t appdata[MAX_PATH], install_dir[MAX_PATH], html_path[MAX_PATH];
    wchar_t exe_path[MAX_PATH], dest_exe[MAX_PATH], readme_path[MAX_PATH];
    wchar_t desktop[MAX_PATH], programs[MAX_PATH], link_path[MAX_PATH];
    wchar_t msg[1024];
    int first_install = 0;
    LPWSTR cmd = GetCommandLineW();
    (void)hInst; (void)hPrev; (void)pCmdLine; (void)nShow;

    CoInitialize(NULL);

    /* ---- Uninstall: LiteCutPro.exe /uninstall ---- */
    if (cmd && (wcsstr(cmd, L"/uninstall") || wcsstr(cmd, L"--uninstall"))) {
        if (get_local_appdata(appdata, MAX_PATH)) {
            wsprintfW(install_dir, L"%s\\LiteCutPro", appdata);
            wsprintfW(html_path, L"%s\\LiteCutPro.html", install_dir);
            DeleteFileW(html_path);
            wsprintfW(readme_path, L"%s\\README.txt", install_dir);
            DeleteFileW(readme_path);
        }
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, desktop))) {
            wsprintfW(link_path, L"%s\\LiteCut Pro.lnk", desktop);
            DeleteFileW(link_path);
        }
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAMS, NULL, SHGFP_TYPE_CURRENT, programs))) {
            wsprintfW(link_path, L"%s\\LiteCut Pro\\LiteCut Pro.lnk", programs);
            DeleteFileW(link_path);
            wsprintfW(link_path, L"%s\\LiteCut Pro", programs);
            RemoveDirectoryW(link_path);
        }
        MessageBoxW(NULL, L"LiteCut Pro files and shortcuts were removed.\nIf this exe is in the install folder, delete it manually.", L"LiteCut Pro uninstalled", MB_OK | MB_ICONINFORMATION);
        CoUninitialize();
        return 0;
    }

    if (!get_local_appdata(appdata, MAX_PATH)) {
        MessageBoxW(NULL, L"Could not find a local folder to install to.", L"LiteCut Pro", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    }
    wsprintfW(install_dir, L"%s\\LiteCutPro", appdata);
    if (!make_dirs(install_dir)) {
        MessageBoxW(NULL, L"Could not create the install folder.", L"LiteCut Pro", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    }

    wsprintfW(html_path, L"%s\\LiteCutPro.html", install_dir);
    wsprintfW(dest_exe, L"%s\\LiteCutPro.exe", install_dir);
    wsprintfW(readme_path, L"%s\\README.txt", install_dir);

    GetModuleFileNameW(NULL, exe_path, MAX_PATH);
    if (_wcsicmp(exe_path, dest_exe) != 0) {
        if (CopyFileW(exe_path, dest_exe, FALSE)) first_install = 1;
    }
    if (!write_file_if_changed(html_path, litecut_html, litecut_html_len)) {
        /* Fall back to the HTML next to the exe if the embedded write failed. */
        MessageBoxW(NULL, L"Could not write the editor file to the install folder.", L"LiteCut Pro", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    } else {
        /* write_file_if_changed returns 1 both when skipped and written;
           treat a fresh exe copy as first install for the welcome note. */
        if (first_install) { /* already set */ }
    }
    write_text_file(readme_path,
        "LiteCut Pro - offline video editor\r\n"
        "==================================\r\n"
        "No internet needed. No account. Files never leave this PC.\r\n"
        "\r\n"
        "Run: double-click LiteCutPro.exe, or the Desktop / Start Menu shortcut.\r\n"
        "It opens LiteCutPro.html in your default browser (Edge / Chrome / Firefox).\r\n"
        "Use the 512 MB mode in the editor for very low-memory PCs.\r\n"
        "\r\n"
        "Uninstall: run LiteCutPro.exe /uninstall, or delete this folder\r\n"
        "and the Desktop / Start Menu shortcuts.\r\n");

    /* Shortcuts point at the installed exe, so they survive moving the download. */
    {
        const wchar_t *target = dir_exists(install_dir) ? dest_exe : exe_path;
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, desktop))) {
            wsprintfW(link_path, L"%s\\LiteCut Pro.lnk", desktop);
            create_shortcut(target, link_path, L"LiteCut Pro - offline video editor");
        }
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAMS, NULL, SHGFP_TYPE_CURRENT, programs))) {
            wchar_t subdir[MAX_PATH];
            wsprintfW(subdir, L"%s\\LiteCut Pro", programs);
            make_dirs(subdir);
            wsprintfW(link_path, L"%s\\LiteCut Pro.lnk", subdir);
            create_shortcut(target, link_path, L"LiteCut Pro - offline video editor");
        }
    }

    /* Open the local editor file in the default browser. */
    {
        HINSTANCE r = ShellExecuteW(NULL, L"open", html_path, NULL, NULL, SW_SHOWNORMAL);
        if ((INT_PTR)r <= 32) {
            wsprintfW(msg, L"Installed to:\n%s\n\nCould not open the editor in your browser. Open LiteCutPro.html manually.", install_dir);
            MessageBoxW(NULL, msg, L"LiteCut Pro", MB_OK | MB_ICONERROR);
            CoUninitialize();
            return 1;
        }
    }

    if (first_install) {
        wsprintfW(msg, L"LiteCut Pro is installed and ready, completely offline.\n\nInstalled to:\n%s\n\nShortcuts were added to your Desktop and Start Menu.\nRun LiteCutPro.exe /uninstall to remove it.", install_dir);
        MessageBoxW(NULL, msg, L"LiteCut Pro installed", MB_OK | MB_ICONINFORMATION);
    }

    CoUninitialize();
    return 0;
}
