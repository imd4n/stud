param(
    [switch] $BuildOnly
)

$ErrorActionPreference = 'Continue'

# Корень проекта — родитель папки .zed, в которой лежит этот скрипт.
$Root = Split-Path -Parent -Path $PSScriptRoot

# g++ может быть не в PATH, если Zed запущен до установки компилятора.
$mingwBin = Join-Path $env:USERPROFILE 'gcc\bin'
if (Test-Path -LiteralPath $mingwBin) {
    $env:PATH = "$mingwBin;$env:PATH"
}

$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if ($null -eq $gpp) {
    Write-Host 'g++ not found. Add C:\Users\<user>\gcc\bin to PATH.' -ForegroundColor Red
    exit 1
}

$srcDir = Join-Path $Root 'Tgraph'
$buildDir = Join-Path $Root 'build'
$exe = Join-Path $buildDir 'Tgraph.exe'
if (-not (Test-Path -LiteralPath $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Write-Host '==> Compiling Tgraph' -ForegroundColor Cyan
& $gpp.Source -std=c++17 -O2 -g `
    -o $exe `
    (Join-Path $srcDir 'Tgraph.cpp') (Join-Path $srcDir 'graph.cpp')
if ($LASTEXITCODE -ne 0) {
    Write-Host '==> Build FAILED' -ForegroundColor Red
    exit $LASTEXITCODE
}
Write-Host "==> Build OK -> $exe" -ForegroundColor Green

if ($BuildOnly) {
    exit 0
}

# Если открыт один из тестовых файлов — запускаем его, иначе берём bpg_001.
$inputFile = Join-Path $srcDir 'resources\base_part\matrix\bpg_001.txt'
if ($env:ZED_FILE) {
    $candidate = $env:ZED_FILE
    if ($candidate -like '*.txt' -and $candidate.Contains('resources')) {
        $inputFile = $candidate
    }
}

Write-Host "==> Running with input: $inputFile" -ForegroundColor Cyan
Write-Host '------------------------------------------------------------'
Push-Location -LiteralPath $Root
try {
    & $exe $inputFile
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
