#include <windows.h>
#include <commctrl.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")

#define IDC_BTN_DOWNLOAD    1001
#define IDC_BTN_BROWSE      1003
#define IDC_PROGRESS        1002
#define IDC_EDIT_PATH       1004
#define IDC_BTN_OPENFOLDER  1005

HWND hWndMain;
HWND hBtnDownload;
HWND hBtnBrowse;
HWND hBtnOpenFolder;
HWND hProgress;
HWND hStatusText;
HWND hEditPath;
char downloadLink[1024] = "";
char fileName[256] = "tecno.tool.exe";
char savePath[MAX_PATH] = "";
char installationFolder[MAX_PATH] = "";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void FetchDownloadInfo(HWND hWnd);
void DownloadFile(HWND hWnd);
void RunFile(char* path);
void CreateShortcut(char* targetPath);
DWORD WINAPI DownloadThread(LPVOID lpParam);
void BrowseFolder(HWND hWnd);
void OpenFolderInExplorer(HWND hWnd);

typedef struct {
    HWND hWnd;
    char url[1024];
    char savePath[MAX_PATH];
} DownloadParams;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "TecnoToolDawnloader";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, "Failed to register window!", "Error", MB_ICONERROR);
        return 1;
    }

    hWndMain = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "TecnoToolDawnloader",
        "Tecno Tool Dawnloader",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 320,
        NULL, NULL, hInstance, NULL
    );

    if (!hWndMain) {
        MessageBox(NULL, "Failed to create window!", "Error", MB_ICONERROR);
        return 1;
    }

    CreateWindow(
        "STATIC",
        "Tecno Tool Installer",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
        0, 10, 600, 35,
        hWndMain, NULL, hInstance, NULL
    );

    hStatusText = CreateWindow(
        "STATIC",
        "Fetching download information...",
        WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
        20, 55, 560, 30,
        hWndMain, NULL, hInstance, NULL
    );

    CreateWindow(
        "STATIC",
        "Installation Folder:",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        20, 100, 120, 20,
        hWndMain, NULL, hInstance, NULL
    );

    hEditPath = CreateWindow(
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
        20, 125, 400, 28,
        hWndMain, (HMENU)IDC_EDIT_PATH, hInstance, NULL
    );

    hBtnBrowse = CreateWindow(
        "BUTTON",
        "Browse...",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        430, 125, 70, 28,
        hWndMain, (HMENU)IDC_BTN_BROWSE, hInstance, NULL
    );

    hBtnOpenFolder = CreateWindow(
        "BUTTON",
        "Open",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        505, 125, 70, 28,
        hWndMain, (HMENU)IDC_BTN_OPENFOLDER, hInstance, NULL
    );

    hBtnDownload = CreateWindow(
        "BUTTON",
        "Download & Install",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        200, 170, 200, 35,
        hWndMain, (HMENU)IDC_BTN_DOWNLOAD, hInstance, NULL
    );
    EnableWindow(hBtnDownload, FALSE);

    hProgress = CreateWindow(
        PROGRESS_CLASS,
        NULL,
        WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
        20, 220, 560, 25,
        hWndMain, (HMENU)IDC_PROGRESS, hInstance, NULL
    );
    SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

    if (SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, installationFolder) == S_OK) {
        SetWindowText(hEditPath, installationFolder);
        strcpy_s(savePath, sizeof(savePath), installationFolder);
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    FetchDownloadInfo(hWndMain);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_BTN_BROWSE:
                    BrowseFolder(hWnd);
                    break;
                case IDC_BTN_OPENFOLDER:
                    OpenFolderInExplorer(hWnd);
                    break;
                case IDC_BTN_DOWNLOAD:
                    DownloadFile(hWnd);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void BrowseFolder(HWND hWnd) {
    BROWSEINFO bi = {0};
    bi.hwndOwner = hWnd;
    bi.lpszTitle = "Select Installation Folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
    bi.lpfn = NULL;
    bi.lParam = 0;

    if (strlen(installationFolder) > 0) {
        bi.pszDisplayName = installationFolder;
    }

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL) {
        char selectedPath[MAX_PATH];
        if (SHGetPathFromIDList(pidl, selectedPath)) {
            SetWindowText(hEditPath, selectedPath);
            strcpy_s(installationFolder, sizeof(installationFolder), selectedPath);
            strcpy_s(savePath, sizeof(savePath), selectedPath);
        }
        CoTaskMemFree(pidl);
    }
}

void OpenFolderInExplorer(HWND hWnd) {
    char folderPath[MAX_PATH];
    GetWindowText(hEditPath, folderPath, sizeof(folderPath));
    
    if (strlen(folderPath) > 0) {
        ShellExecute(NULL, "open", folderPath, NULL, NULL, SW_SHOWNORMAL);
    } else {
        MessageBox(hWnd, "No folder selected!", "Information", MB_ICONINFORMATION);
    }
}

void FetchDownloadInfo(HWND hWnd) {
    HINTERNET hInternet = InternetOpen("TecnoToolDawnloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        SetWindowText(hStatusText, "Internet connection failed!");
        return;
    }

    HINTERNET hConnect = InternetOpenUrl(
        hInternet,
        "https://raw.githubusercontent.com/857seif/tecno-tool/main/data.json",
        NULL, 0, INTERNET_FLAG_RELOAD, 0
    );

    if (!hConnect) {
        InternetCloseHandle(hInternet);
        SetWindowText(hStatusText, "Failed to fetch data!");
        return;
    }

    char buffer[4096] = {0};
    DWORD bytesRead = 0;
    char jsonData[4096] = {0};
    int totalBytes = 0;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        strcat_s(jsonData, sizeof(jsonData), buffer);
        totalBytes += bytesRead;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    if (totalBytes == 0) {
        SetWindowText(hStatusText, "Empty data!");
        return;
    }

    char* linkStart = strstr(jsonData, "\"link\"");
    if (linkStart) {
        linkStart = strstr(linkStart, ":");
        if (linkStart) {
            linkStart++;
            while (*linkStart == ' ' || *linkStart == '\t' || *linkStart == '\"') {
                linkStart++;
            }
            
            char* linkEnd = strstr(linkStart, "\"");
            if (linkEnd) {
                int linkLen = linkEnd - linkStart;
                if (linkLen < sizeof(downloadLink)) {
                    strncpy_s(downloadLink, sizeof(downloadLink), linkStart, linkLen);
                }
                
                char* fileNameStart = strrchr(downloadLink, '/');
                if (fileNameStart) {
                    strcpy_s(fileName, sizeof(fileName), fileNameStart + 1);
                } else {
                    strcpy_s(fileName, sizeof(fileName), "tecno.tool.exe");
                }

                char statusMsg[512];
                sprintf_s(statusMsg, sizeof(statusMsg), "Ready to download: %s", fileName);
                SetWindowText(hStatusText, statusMsg);
                EnableWindow(hBtnDownload, TRUE);
            }
        }
    } else {
        SetWindowText(hStatusText, "Download link not found!");
    }
}

void DownloadFile(HWND hWnd) {
    if (strlen(downloadLink) == 0) {
        MessageBox(hWnd, "Download link not available!", "Error", MB_ICONERROR);
        return;
    }

    char folderPath[MAX_PATH];
    GetWindowText(hEditPath, folderPath, sizeof(folderPath));
    
    if (strlen(folderPath) == 0) {
        MessageBox(hWnd, "Please select an installation folder!", "Error", MB_ICONERROR);
        return;
    }

    strcpy_s(savePath, sizeof(savePath), folderPath);
    strcat_s(savePath, sizeof(savePath), "\\");
    strcat_s(savePath, sizeof(savePath), fileName);

    EnableWindow(hBtnDownload, FALSE);
    EnableWindow(hBtnBrowse, FALSE);
    EnableWindow(hBtnOpenFolder, FALSE);
    SetWindowText(hStatusText, "Downloading...");

    DownloadParams* params = (DownloadParams*)malloc(sizeof(DownloadParams));
    if (params) {
        params->hWnd = hWnd;
        strcpy_s(params->url, sizeof(params->url), downloadLink);
        strcpy_s(params->savePath, sizeof(params->savePath), savePath);
        CreateThread(NULL, 0, DownloadThread, params, 0, NULL);
    }
}

DWORD WINAPI DownloadThread(LPVOID lpParam) {
    DownloadParams* params = (DownloadParams*)lpParam;
    if (!params) return 1;
    
    HWND hWnd = params->hWnd;
    char* url = params->url;
    char* path = params->savePath;

    HINTERNET hInternet = InternetOpen("TecnoToolDawnloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        MessageBox(hWnd, "Internet connection failed!", "Error", MB_ICONERROR);
        free(params);
        return 1;
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        MessageBox(hWnd, "Failed to open download link!", "Error", MB_ICONERROR);
        free(params);
        return 1;
    }

    DWORD contentLength = 0;
    DWORD contentLengthSize = sizeof(contentLength);
    HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
                  &contentLength, &contentLengthSize, NULL);

    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        MessageBox(hWnd, "Failed to create file!", "Error", MB_ICONERROR);
        free(params);
        return 1;
    }

    char buffer[8192];
    DWORD bytesRead = 0;
    DWORD totalBytes = 0;
    int lastProgress = -1;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        DWORD bytesWritten;
        WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
        totalBytes += bytesRead;

        if (contentLength > 0) {
            int progress = (int)((totalBytes * 100) / contentLength);
            if (progress != lastProgress) {
                lastProgress = progress;
                SendMessage(hProgress, PBM_SETPOS, progress, 0);
                
                char statusMsg[256];
                sprintf_s(statusMsg, sizeof(statusMsg), "Downloading... %d%%", progress);
                SetWindowText(hStatusText, statusMsg);
            }
        }
    }

    CloseHandle(hFile);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    SendMessage(hProgress, PBM_SETPOS, 100, 0);
    SetWindowText(hStatusText, "Download complete!");

    int result = MessageBox(hWnd,
        "File downloaded successfully!\n\nDo you want to run it now?",
        "Download Complete",
        MB_YESNO | MB_ICONQUESTION);

    if (result == IDYES) {
        RunFile(path);
    }

    CreateShortcut(path);
    EnableWindow(hBtnDownload, TRUE);
    EnableWindow(hBtnBrowse, TRUE);
    EnableWindow(hBtnOpenFolder, TRUE);
    free(params);
    return 0;
}

void RunFile(char* path) {
    ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}

void CreateShortcut(char* targetPath) {
    char desktopPath[MAX_PATH];
    char shortcutPath[MAX_PATH];
    
    if (SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, desktopPath) != S_OK) {
        MessageBox(NULL, "Failed to get desktop path!", "Warning", MB_ICONWARNING);
        return;
    }

    sprintf_s(shortcutPath, sizeof(shortcutPath), "%s\\Tecno Tool.lnk", desktopPath);

    char command[2048];
    sprintf_s(command, sizeof(command), 
        "powershell -command \"$WshShell = New-Object -comObject WScript.Shell; "
        "$Shortcut = $WshShell.CreateShortcut('%s'); "
        "$Shortcut.TargetPath = '%s'; "
        "$Shortcut.WorkingDirectory = '%s'; "
        "$Shortcut.Save()\"",
        shortcutPath, targetPath, targetPath);
    
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    if (CreateProcess(NULL, command, NULL, NULL, FALSE, 
                     CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        MessageBox(NULL, "Shortcut created on desktop!", "Success", MB_ICONINFORMATION);
    }
}
