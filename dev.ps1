<#
.SYNOPSIS
A helper script to manage the micromouse Docker development environment.
#>
param (
    # Re-build the docker image
    [switch]$Build,

    # Start the container in the background
    [switch]$Up,

    # Stop and remove the container
    [switch]$Down,

    # Open a bash shell inside the running container
    [switch]$Shell,

    # Follow the logs of the container
    [switch]$Logs
)

# Set common options
$ErrorActionPreference = "Stop"

if ($Build) {
    Write-Host "Building Docker image 'micromouse_sim'..." -ForegroundColor Green
    docker-compose build
}

if ($Up) {
    Write-Host "Starting container in detached mode..." -ForegroundColor Green
    docker-compose up -d
}

if ($Shell) {
    Write-Host "Opening shell in 'ros-dev' container..." -ForegroundColor Cyan
    docker-compose exec ros-dev /bin/bash
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
    Write-Host "
Usage: .\dev.ps1 [command]

Commands:
  -Build      Build the Docker image.
  -Up         Start the container in the background.
  -Shell      Open a bash shell inside the running container.
  -Logs       Follow the container's logs.
  -Down       Stop and remove the container.

Example:
  .\dev.ps1 -Build -Up
"
}
