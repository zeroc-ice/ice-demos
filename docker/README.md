
# Ice 3.7 Nightly Package Testing with Docker

This directory contains Docker configurations for testing Ice 3.7 nightly packages on various Linux distributions.

- [Ice 3.7 Nightly Package Testing with Docker](#ice-37-nightly-package-testing-with-docker)
  - [Available Images](#available-images)
  - [Common Tools](#common-tools)
  - [Quick Start](#quick-start)
  - [Testing Demos](#testing-demos)
  - [Container Management](#container-management)
  - [Notes](#notes)

## Available Images

| Directory                   | Distribution         | Ice Packages              |
|-----------------------------|----------------------|---------------------------|
| [ubuntu22.04](ubuntu22.04/) | Ubuntu 22.04 (Jammy) | Ubuntu nightly            |
| [ubuntu24.04](ubuntu24.04/) | Ubuntu 24.04 (Noble) | Ubuntu nightly            |
| [debian12](debian12/)       | Debian 12 (Bookworm) | Debian nightly            |
| [debian13](debian13/)       | Debian 13 (Trixie)   | Debian nightly            |
| [rocky9](rocky9/)           | Rocky Linux 9        | RHEL 9 nightly            |
| [rocky10](rocky10/)         | Rocky Linux 10       | RHEL 10 nightly           |
| [amzn2023](amzn2023/)       | Amazon Linux 2023    | Amazon Linux 2023 nightly |

## Common Tools

All images include the following development tools:

- Node.js 22.x LTS
- .NET SDK 10.0 with .NET 8.0 runtime
- Java (OpenJDK)
- Python 3
- Ruby
- PHP
- Ice 3.7.11 nightly (all runtime and development packages)

## Quick Start

Build the image (from the docker directory):

```shell
docker build -f <distribution>/Dockerfile -t ice-demos-<distribution>:3.7-nightly .
```

Run the container (from the ice-demos directory):

```shell
docker run -d --name ice-demos-<distribution>-test \
  -v $(pwd):/ice-demos \
  ice-demos-<distribution>:3.7-nightly \
  tail -f /dev/null
```

Build demos:

```shell
docker exec ice-demos-<distribution>-test make -C cpp98
```

```shell
docker exec ice-demos-<distribution>-test make -C cpp11
```

Interactive shell:

```shell
docker exec -it ice-demos-<distribution>-test bash
```

## Testing Demos

Once inside a container, you can test demos:

```shell
cd /ice-demos/cpp11/Ice/hello
./server &
./client
```

## Container Management

Stop the container:

```shell
docker stop <container-name>
```

Start it again later:

```shell
docker start <container-name>
```

Remove the container when done:

```shell
docker rm -f <container-name>
```

## Notes

- The worktree is mounted at `/ice-demos` inside the container
- Changes made inside the container are reflected in the worktree on the host
- `ICE_HOME` is set to `/usr` in all containers
- `DOTNET_ROOT` is set to `/usr/share/dotnet`
