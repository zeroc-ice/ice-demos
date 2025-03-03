# C++ Demos

- [C++ Demos](#c-demos)
  - [Overview](#overview)
  - [macOS Prerequisites](#macos-prerequisites)
  - [Ubuntu Prerequisites](#ubuntu-prerequisites)
  - [Windows Prerequisites](#windows-prerequisites)

## Overview

This directory contains C++ sample programs for various Ice components. These examples
are provided to get you started on using a particular Ice feature or coding technique.

The subdirectories here correspond directly to Ice components, such as
[IceGrid](./IceGrid) and [Glacier2](./Glacier2).

Refer to the README.md file in each demo directory for usage instructions.

## macOS Prerequisites

- macOS Sequoia
- Xcode
- [Homebrew](https://brew.sh)
- CMake
- Nightly build of Ice

Install the nightly build of Ice:

```shell
brew tap zeroc-ice/nightly  https://download.zeroc.com/nexus/repository/nightly/homebrew-nightly.git
brew install zeroc-ice/nightly/ice
```

## Ubuntu Prerequisites

- Ubuntu 24.04
- GCC C++ Toolchain
- CMake
- Nightly build of Ice

Install the nightly build of Ice:

```shell
sudo mkdir -p /etc/apt/keyrings
sudo curl -fsSL https://download.zeroc.com/GPG-KEY-zeroc-nightly \
  | sudo gpg --dearmor -o /etc/apt/keyrings/zeroc-nightly.gpg

echo "deb [signed-by=/etc/apt/keyrings/zeroc-nightly.gpg] https://download.zeroc.com/nexus/repository/ubuntu24.04-nightly nightly main" \
  | sudo tee /etc/apt/sources.list.d/zeroc-nightly.list

sudo apt-get update
sudo apt-get install -y zeroc-ice-all-dev
```

## Windows Prerequisites

- Visual Studio 2022
- CMake (installed via Visual Studio installer)
- Nightly build of Ice

The CMake script will automatically download the required NuGet packages when you
build the demos. The NuGet packages are cached the [cmake](./cmake) directory.
