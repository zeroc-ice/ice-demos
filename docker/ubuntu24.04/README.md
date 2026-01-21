
# Ubuntu 24.04 (Noble Numbat)

Docker image for testing Ice 3.7 nightly packages on Ubuntu 24.04.

- [Ubuntu 24.04 (Noble Numbat)](#ubuntu-2404-noble-numbat)
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
| Python         | 3.12           |
| Ruby           | 3.2            |
| PHP            | 8.3            |
| Ice            | 3.7.11 nightly |

## Build the Docker Image

From the docker directory:

```shell
docker build -f ubuntu24.04/Dockerfile -t ice-demos-ubuntu24.04:3.7-nightly .
```

## Run the Container

From the ice-demos directory:

```shell
docker run -d --name ice-demos-ubuntu24.04-test \
  -v $(pwd):/ice-demos \
  ice-demos-ubuntu24.04:3.7-nightly \
  tail -f /dev/null
```

## Build the C++ Demos

Build C++98 demos:

```shell
docker exec ice-demos-ubuntu24.04-test make -j8 -C cpp98
```

Build C++11 demos:

```shell
docker exec ice-demos-ubuntu24.04-test make -j8 -C cpp11
```

## Interactive Shell Access

```shell
docker exec -it ice-demos-ubuntu24.04-test bash
```

## Ice Packages Installed

- `zeroc-ice-all-runtime` - All Ice runtime components
- `zeroc-ice-all-dev` - All Ice development components
