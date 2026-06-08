FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

USER vscode
WORKDIR /home/vscode

RUN set -eux \
    && wget -q https://packages.microsoft.com/config/ubuntu/24.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb \
    && sudo dpkg -i packages-microsoft-prod.deb \
    && rm packages-microsoft-prod.deb \
    && sudo apt update \
    && sudo apt install -y gdb libbz2-dev \
    && sudo apt-get install -y php python3  \
    && sudo apt-get install -y openjdk-17-jdk \
    && sudo apt-get install -y cmake \
    && sudo rm -rf /var/lib/apt/lists/* \
    && sudo apt-get clean

# Install .NET 8.0
RUN set -eux \
    && curl https://dot.net/v1/dotnet-install.sh -o dotnet-install.sh -L \
    && chmod +x dotnet-install.sh \
    && ./dotnet-install.sh -v latest -c 8.0

# Install Ice 3.8 release
RUN set -eux \
    && sudo mkdir -p /etc/apt/keyrings \
    && curl -fsSL https://download.zeroc.com/ice/3.8/ubuntu24.04/ice-repo-3.8_1.0.0_all.deb -o ice-repo-3.8.deb \
    && sudo dpkg -i ice-repo-3.8.deb \
    && rm ice-repo-3.8.deb \
    && sudo apt-get update \
    && sudo apt-get install -y \
    libzeroc-ice-dev \
    libzeroc-ice3.8 \
    php-zeroc-ice \
    python3-zeroc-ice \
    zeroc-dsnode \
    zeroc-glacier2 \
    zeroc-ice-slice \
    zeroc-ice-utils \
    zeroc-icebox \
    zeroc-icebridge \
    zeroc-icegrid \
    zeroc-icestorm

# Install nightly Ice for Ruby
RUN sudo apt-get install -y ruby ruby-dev \
    && sudo rm -rf /var/lib/apt/lists/* \
    && sudo apt-get clean \
    && sudo gem install zeroc-ice --source https://download.zeroc.com/nexus/repository/rubygems-nightly --pre

# Set environment variables
ENV DOTNET_ROOT=/home/vscode/.dotnet
ENV PATH=$DOTNET_ROOT:$PATH
ENV LANG=en_US.UTF-8
