<#
.SYNOPSIS
A helper script to manage the Micromouse Docker development environment with automatic DISPLAY configuration.
#>
param (
[switch]$Build,
[switch]$Up,
[switch]$Down,
[switch]$Shell,
[switch]$Logs
)

# Set error preference

$ErrorActionPreference = 'Stop'

# Auto-detect Windows IP for DISPLAY inside container

$windowsIP = (Get-NetIPAddress -AddressFamily IPv4 |
Where-Object {$_.InterfaceAlias -notmatch 'Loopback'} |
Select-Object -First 1 -ExpandProperty IPAddress)
$env:DISPLAY = "$windowsIP:0.0"
Write-Host "DISPLAY automatically set to $env:DISPLAY" -ForegroundColor Cyan

if ($Build) {
Write-Host "Building Docker image 'micromouse_sim'..." -ForegroundColor Green
docker-compose build
}

if ($Up) {
Write-Host "Starting container in detached mode..." -ForegroundColor Green
docker-compose up -d
}

if ($Shell) {
Write-Host "Opening shell in 'ros-dev' container with DISPLAY set..." -ForegroundColor Cyan
docker-compose exec -e DISPLAY=$env:DISPLAY ros-dev /bin/bash
}

if ($Logs) {
Write-Host "Following container logs (Press Ctrl+C to stop)..." -ForegroundColor Yellow
docker-compose logs -f ros-dev
}

if ($Down) {
Write-Host "Stopping and removing container..." -ForegroundColor Red
docker-compose down
}

if (-not ($Build -or $Up -or $Down -or $Shell -or $Logs)) {
Write-Host @"
Usage: .\dev.ps1 [command]

Commands:
-Build      Build the Docker image.
-Up         Start the container in the background.
-Shell      Open a bash shell inside the running container with DISPLAY auto-set.
-Logs       Follow the container's logs.
-Down       Stop and remove the container.

Example:
.\dev.ps1 -Build -Up
"@
}
