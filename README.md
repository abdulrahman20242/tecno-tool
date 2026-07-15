
<div align="center">

# 🔧 Techno Tool — All-in-One Steam Utility

**A modern, dark-themed PyQt5 GUI toolkit for Steam game management, fixes, and utilities.**

[![Python](https://img.shields.io/badge/Python-3.8%2B-blue?style=flat-square&logo=python)](https://python.org)
[![PyQt5](https://img.shields.io/badge/PyQt5-5.15%2B-green?style=flat-square&logo=qt)](https://riverbankcomputing.com/software/pyqt)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square&logo=windows)](https://microsoft.com/windows)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
  - [📥 Add Tab](#-add-tab)
  - [📚 Library Tab](#-library-tab)
  - [🔧 Generic Fix Tab](#-generic-fix-tab)
  - [🔨 Crack Tab](#-crack-tab)
  - [🌐 Online Fix Tab](#-online-fix-tab)
  - [🔄 Bypass Tab](#-bypass-tab)
  - [🏆 Achievement Manager Tab](#-achievement-manager-tab)
- [Top Bar Controls](#-top-bar-controls)
- [Installation](#-installation)
- [Usage](#-usage)
- [System Requirements](#-system-requirements)
- [Project Structure](#-project-structure)
- [Third-Party Dependencies & Credits](#-third-party-dependencies--credits)
- [Disclaimer](#-disclaimer)
- [License](#-license)

---

## 🎯 Overview

**Techno Tool** is a comprehensive desktop application built with **PyQt5** that provides a unified interface for managing Steam-related utilities. It features a sleek dark UI, drag-and-drop support, multi-threaded downloads, and seamless integration with multiple third-party tools and GitHub repositories.

The tool automatically detects your Steam installation, manages Lua scripts, downloads game fixes, handles cloud save backups, and provides one-click access to popular Steam utilities.

---

## ✨ Features

### 📥 Add Tab

| Feature | Description |
|---------|-------------|
| **Drag & Drop** | Drop `.lua` files directly into the application |
| **Auto-Detection** | Automatically detects Steam path via Windows Registry |
| **Smart Folder Management** | Migrates legacy `stplug-in` folders to `lua` automatically |
| **Overwrite Protection** | Prompts before overwriting existing files |
| **Fallback Path** | Saves to current directory if Steam is not found |

**How it works:**
1. Drag any `.lua` file into the drop zone
2. The tool copies it to `Steam/config/lua/`
3. If a file with the same name exists, you'll be asked to overwrite

---

### 📚 Library Tab

| Feature | Description |
|---------|-------------|
| **Auto-Scan** | Scans `Steam/config/lua/` for all `.lua` scripts |
| **Live Search** | Filter games by name or AppID in real-time |
| **Steam API Integration** | Fetches game icons and names from Steam Store API |
| **Parallel Loading** | Multi-threaded icon/name fetching (up to CPU count threads) |
| **One-Click Delete** | Remove scripts with confirmation dialog |
| **Manifest Cleanup** | Automatically cleans `.manifest` files on deletion |

**Visual Features:**
- Game cards with fetched Steam icons
- Live counter showing total/visible games
- Loading progress indicator during refresh

---

### 🔧 Generic Fix Tab

Downloads **AfandiLauncher.exe** — a tool for enabling online play in Steam games with generic fixes.

| Feature | Description |
|---------|-------------|
| **One-Click Download** | Direct download to chosen folder |
| **Progress Tracking** | Real-time download progress bar |
| **Auto-Open Folder** | Opens destination folder after completion |

**Source:** [857seif/online-fix-for-steam](https://github.com/857seif/online-fix-for-steam)

---

### 🔨 Crack Tab

Downloads and extracts the **Steam Fox DRM Unlocker** package for bypassing DRM protection.

| Feature | Description |
|---------|-------------|
| **Download & Extract** | Automatic ZIP download and extraction |
| **Progress Feedback** | Shows download and extraction progress |
| **Auto-Open** | Opens extracted folder on completion |

**Source:** [857seif/steam-fox-drm-unloacker](https://github.com/857seif/steam-fox-drm-unloacker)

---

### 🌐 Online Fix Tab

Downloads the **online-fix-downloader.exe** tool for downloading online multiplayer fixes.

| Feature | Description |
|---------|-------------|
| **Direct Download** | Fetches the latest version |
| **Progress Bar** | Visual download progress |
| **Auto-Open** | Opens save folder after download |

**Source:** [857seif/tecno-tool](https://github.com/857seif/tecno-tool)

---

### 🔄 Bypass Tab

A **Game Fixes Browser** that connects to a curated JSON database of game bypasses and fixes.

| Feature | Description |
|---------|-------------|
| **Live Database** | Fetches fixes from GitHub-hosted JSON |
| **Search** | Search by game name or AppID |
| **Grid Layout** | Responsive 4-column card grid |
| **Cancellable Downloads** | Download fixes with cancel support |
| **ZIP Extraction** | Auto-extracts downloaded fixes |
| **Size Info** | Shows fix size before downloading |

**Database Source:** [857seif/games-bypass](https://github.com/857seif/games-bypass)

**Download Features:**
- Frameless progress dialog
- Cancel anytime during download
- Auto-cleanup of ZIP files after extraction
- Success/cancelled/failed states with icons

---

### 🏆 Achievement Manager Tab

Downloads and extracts **Steam Achievement Manager (SAM)** for managing game achievements.

| Feature | Description |
|---------|-------------|
| **Download & Extract** | Fetches SAM 7.0.41 and extracts automatically |
| **Official Release** | Uses official Gibbed release |
| **Auto-Open** | Opens extracted folder on completion |

**Source:** [gibbed/SteamAchievementManager](https://github.com/gibbed/SteamAchievementManager)

---

## 🎛️ Top Bar Controls

The top bar provides quick-access toggle buttons and action buttons:

| Button | Function |
|--------|----------|
| **Steam Family Shear** | Toggle ON/OFF (visual toggle) |
| **Last Games Update** | Toggle ON/OFF (visual toggle) |
| **DLC Unlocker** | Toggle ON/OFF (visual toggle) |
| **☁️ Fix Cloud Save** | Backs up `userdata` folder and restarts Steam |
| **🚀 Launch Steam** | Starts Steam.exe from detected installation |

### Cloud Save Fix Process:
1. Detects Steam installation
2. Checks if `userdata` exists
3. Prompts to close Steam
4. Creates `afndi backup` folder in Steam directory
5. Moves all `userdata` contents to backup
6. Removes old `userdata` folder
7. Restarts Steam automatically

---

## 📦 Installation

### Prerequisites

- **Windows 10/11**
- **Python 3.8+**
- **Steam** installed (for full functionality)

### Step 1: Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/techno-tool.git
cd techno-tool
```

### Step 2: Install Dependencies

```bash
pip install PyQt5 requests
```

### Step 3: Run

```bash
python techno-tool.py
```

---

## 🚀 Usage

### First Launch
1. The tool automatically detects your Steam installation
2. Background DLLs are downloaded silently (`OpenSteamTool.dll`, `dwmapi.dll`, `xinput1_4.dll`)
3. The `lua` folder is created in `Steam/config/` if it doesn't exist

### Adding Lua Scripts
1. Go to the **📥 Add** tab
2. Drag and drop any `.lua` file into the drop zone
3. The file is copied to `Steam/config/lua/`

### Managing Library
1. Go to the **📚 Library** tab
2. View all installed Lua scripts with game info
3. Use the search bar to filter
4. Click 🗑 to delete unwanted scripts

### Downloading Fixes
1. Go to **🔄 Bypass** tab
2. Search for your game
3. Click **↓ Download** on the desired fix
4. Choose destination folder
5. Wait for download and extraction

---

## 💻 System Requirements

| Requirement | Minimum |
|-------------|---------|
| OS | Windows 10/11 |
| Python | 3.8+ |
| RAM | 512 MB |
| Disk Space | 43 MB |
| Internet | Required for downloads |
| Steam | Optional (for library features) |

---

## 📁 Project Structure

```
techno-tool/
│
├── techno_tool.py          # Main application file
├── README.md               # This file
├── LICENSE                 # MIT License
│
└── (Runtime files)
    ├── Steam/config/lua/   # Lua scripts folder
    └── afndi backup/       # Cloud save backups
```

---

## 🔗 Third-Party Dependencies & Credits

| Tool / Resource | Repository | Purpose |
|-----------------|------------|---------|
| **Game Fixes Database** | [857seif/games-bypass](https://github.com/857seif/games-bypass) | Curated fixes JSON |
| **Afandi Launcher** | [857seif/online-fix-for-steam](https://github.com/857seif/online-fix-for-steam) | Generic online fix |
| **Steam Fox DRM Unlocker** | [857seif/steam-fox-drm-unloacker](https://github.com/857seif/steam-fox-drm-unloacker) | DRM bypass tool |
| **Online Fix Downloader** | [857seif/tecno-tool](https://github.com/857seif/tecno-tool) | Online fix downloader |
| **Steam Achievement Manager** | [gibbed/SteamAchievementManager](https://github.com/gibbed/SteamAchievementManager) | Achievement manager |

### Python Libraries
- **PyQt5** — GUI framework
- **requests** — HTTP downloads and API calls
- **urllib3** — HTTP client (warnings disabled for self-signed certs)

---

## ⚠️ Disclaimer

> **This tool is provided for educational and research purposes only.**

- The authors do not condone piracy or circumvention of copy protection
- Use this tool at your own risk
- Always respect Steam's Terms of Service
- The tool modifies Steam configuration files — create backups before use
- Downloaded third-party tools are property of their respective authors
- This project is not affiliated with Valve Corporation or Steam

---

## 📄 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2026 Techno Tool Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

<div align="center">


[⬆ Back to Top](#-techno-tool--all-in-one-steam-utility)

</div>
