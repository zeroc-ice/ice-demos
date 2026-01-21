
# Debian 13 (Trixie)

Docker image for testing Ice 3.7 nightly packages on Debian 13.

- [Debian 13 (Trixie)](#debian-13-trixie)
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
| Java (OpenJDK) | 21             |
| Python         | 3.13           |
| Ruby           | 3.3            |
| PHP            | 8.4            |
| Ice            | 3.7.11 nightly |

## Build the Docker Image

From the docker directory:

```shell
docker build -f debian13/Dockerfile -t ice-demos-debian13:3.7-nightly .
```

## Run the Container

From the ice-demos directory:

```shell
docker run -d --name ice-demos-debian13-test \
  -v $(pwd):/ice-demos \
  ice-demos-debian13:3.7-nightly \
  tail -f /dev/null
```

## Build the C++ Demos

Build C++98 demos:

```shell
docker exec ice-demos-debian13-test make -j8 -C cpp98
```

Build C++11 demos:

```shell
docker exec ice-demos-debian13-test make -j8 -C cpp11
```

## Interactive Shell Access

```shell
docker exec -it ice-demos-debian13-test bash
```

## Ice Packages Installed

- `zeroc-ice-all-runtime` - All Ice runtime components
- `zeroc-ice-all-dev` - All Ice development components
