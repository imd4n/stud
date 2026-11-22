param(
    [switch] $ConfigureOnly,
    [switch] $BuildOnly
)

$ErrorActionPreference = 'Continue'

# Project root is the parent of the .zed directory that contains this script.
$Root = Split-Path -Parent -Path $PSScriptRoot

# Make CMake, Ninja and MinGW discoverable even if Zed was started before installing them.
# Ninja is used as generator: unlike Makefiles it handles Cyrillic paths correctly.
$cmakeBin = 'C:\Program Files\CMake\bin'
$ninjaBin = Join-Path $env:LOCALAPPDATA 'Programs\ninja'
$mingwBin = Join-Path $env:USERPROFILE 'gcc\bin'
foreach ($dir in @($cmakeBin, $ninjaBin, $mingwBin)) {
    if (Test-Path -LiteralPath $dir) {
        $env:PATH = "$dir;$env:PATH"
    }
}

$cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($null -eq $cmakeCmd) {
    Write-Host 'cmake not found. Install CMake or add it to your PATH.' -ForegroundColor Red
    exit 1
}
$cmake = $cmakeCmd.Source
if ($null -eq (Get-Command ninja -ErrorAction SilentlyContinue)) {
    Write-Host 'ninja not found. Install Ninja or add it to your PATH.' -ForegroundColor Red
    exit 1
}

$srcDir = Join-Path $Root 'cmake'
$buildDir = Join-Path $Root 'build-win'
$toolchain = Join-Path $srcDir 'toolchain.cmake'
$exe = Join-Path $Root 'output\ExOOP.exe'

Write-Host "==> Configuring with $cmake" -ForegroundColor Cyan
& $cmake -S $srcDir -B $buildDir -G Ninja "-DCMAKE_TOOLCHAIN_FILE=$toolchain" -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) {
    Write-Host '==> Configure FAILED' -ForegroundColor Red
    exit $LASTEXITCODE
}
if ($ConfigureOnly) {
    exit 0
}

Write-Host '==> Building target ExOOP' -ForegroundColor Cyan
& $cmake --build $buildDir --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Host '==> Build FAILED' -ForegroundColor Red
    exit $LASTEXITCODE
}
Write-Host "==> Build OK -> $exe" -ForegroundColor Green

if ($BuildOnly) {
    exit 0
}

Write-Host '==> Running ExOOP.exe (press ESC in the game to quit)' -ForegroundColor Cyan
Write-Host '------------------------------------------------------------'
Push-Location -LiteralPath $Root
try {
    & $exe
    $runCode = $LASTEXITCODE
}
finally {
    Pop-Location
}
Write-Host '------------------------------------------------------------'
if ($runCode -ne 0) {
    Write-Host "==> Program exited with code $runCode" -ForegroundColor Yellow
}
else {
    Write-Host '==> Program finished (exit code 0)' -ForegroundColor Green
}
exit $runCode
