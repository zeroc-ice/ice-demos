
# Debian 12 (Bookworm)

Docker image for testing Ice 3.7 nightly packages on Debian 12.

- [Debian 12 (Bookworm)](#debian-12-bookworm)
  - [Included Tools](#included-tools)
  - [Build the Docker Image](#build-the-docker-image)
  - [Run the Container](#run-the-container)
  - [Build the C++ Demos](#build-the-c-demos)
  - [Interactive Shell Access](#interactive-shell-access)
  - [Ice Packages Installed](#ice-packages-installed)

## Included Tools

| Tool           | Version        |
|----------------|----------------|
| Node.js        | 22.x LTS       |
| npm            | 10.x           |
| .NET SDK       | 10.0           |
| .NET Runtime   | 8.0, 10.0      |
| Java (OpenJDK) | 17             |
| Python         | 3.11           |
| Ruby           | 3.1            |
| PHP            | 8.2            |
| Ice            | 3.7.11 nightly |

## Build the Docker Image

From the docker directory:

```shell
docker build -f debian12/Dockerfile -t ice-demos-debian12:3.7-nightly .
```

## Run the Container

From the ice-demos directory:

```shell
docker run -d --name ice-demos-debian12-test \
  -v $(pwd):/ice-demos \
  ice-demos-debian12:3.7-nightly \
  tail -f /dev/null
```

## Build the C++ Demos

Build C++98 demos:

```shell
docker exec ice-demos-debian12-test make -j8 -C cpp98
```

Build C++11 demos:

```shell
docker exec ice-demos-debian12-test make -j8 -C cpp11
```

## Interactive Shell Access

```shell
docker exec -it ice-demos-debian12-test bash
```

## Ice Packages Installed

- `zeroc-ice-all-runtime` - All Ice runtime components
- `zeroc-ice-all-dev` - All Ice development components
