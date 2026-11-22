param(
    [Parameter(Mandatory = $true)]
    [string] $Source,

    [switch] $CompileOnly
)

$ErrorActionPreference = 'Continue'

if (-not (Test-Path -LiteralPath $Source)) {
    Write-Host "Source file not found: $Source" -ForegroundColor Red
    exit 1
}

# Find gcc on PATH, with a few common Windows/MinGW fallbacks.
$gccCmd = Get-Command gcc -ErrorAction SilentlyContinue
if ($null -eq $gccCmd) {
    $candidates = @(
        "$env:USERPROFILE\gcc\bin\gcc.exe",
        'C:\MinGW\bin\gcc.exe',
        'C:\msys64\mingw64\bin\gcc.exe',
        'C:\Program Files\mingw-w64\x86_64-posix-seh\bin\gcc.exe'
    )
    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate) {
            $gccCmd = Get-Command $candidate
            break
        }
    }
}
if ($null -eq $gccCmd) {
    Write-Host 'gcc not found. Install MinGW-w64 or add gcc to your PATH.' -ForegroundColor Red
    exit 1
}
$gcc = $gccCmd.Source

$exe = [System.IO.Path]::ChangeExtension($Source, '.exe')
$sourceDir = Split-Path -Parent -Path $Source
$fileName = Split-Path -Leaf -Path $Source

Write-Host "==> Compiling $fileName" -ForegroundColor Cyan
& $gcc -std=c11 -O2 -Wall -Wextra -g $Source -o $exe
$buildCode = $LASTEXITCODE
if ($buildCode -ne 0) {
    Write-Host "==> Build FAILED (exit code $buildCode)" -ForegroundColor Red
    exit $buildCode
}
Write-Host "==> Build OK -> $exe" -ForegroundColor Green

if ($CompileOnly) {
    exit 0
}

Write-Host "==> Running $fileName" -ForegroundColor Cyan
Write-Host '------------------------------------------------------------'
Push-Location -LiteralPath $sourceDir
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
