
# Amazon Linux 2023

Docker image for testing Ice 3.7 nightly packages on Amazon Linux 2023.

- [Amazon Linux 2023](#amazon-linux-2023)
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
| Java (OpenJDK) | 25             |
| Python         | 3.9            |
| Ruby           | 3.2            |
| PHP            | 8.4            |
| Ice            | 3.7.11 nightly |

## Build the Docker Image

From the docker directory:

```shell
docker build -f amzn2023/Dockerfile -t ice-demos-amzn2023:3.7-nightly .
```

## Run the Container

From the ice-demos directory:

```shell
docker run -d --name ice-demos-amzn2023-test \
  -v $(pwd):/ice-demos \
  ice-demos-amzn2023:3.7-nightly \
  tail -f /dev/null
```

## Build the C++ Demos

Build C++98 demos:

```shell
docker exec ice-demos-amzn2023-test make -j8 -C cpp98
```

Build C++11 demos:

```shell
docker exec ice-demos-amzn2023-test make -j8 -C cpp11
```

## Interactive Shell Access

```shell
docker exec -it ice-demos-amzn2023-test bash
```

## Ice Packages Installed

- `ice-all-runtime` - All Ice runtime components
- `ice-all-devel` - All Ice development components
