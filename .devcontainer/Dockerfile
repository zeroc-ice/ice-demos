FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

USER vscode
WORKDIR /home/vscode

RUN set -eux \
    && wget -q https://packages.microsoft.com/config/ubuntu/24.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb \
    && sudo dpkg -i packages-microsoft-prod.deb \
    && rm packages-microsoft-prod.deb \
    && sudo apt update \
    && sudo apt-get install -y php python3 ruby gdb \
    && sudo apt-get install -y openjdk-17-jdk \
    && sudo apt-get install -y cmake \
    && sudo rm -rf /var/lib/apt/lists/* \
    && sudo apt-get clean

# Install .NET 8.0
RUN set -eux \
    && curl https://dot.net/v1/dotnet-install.sh -o dotnet-install.sh -L \
    && chmod +x dotnet-install.sh \
    && ./dotnet-install.sh -v latest -c 8.0

# Install Nightly Ice
RUN set -eux \
    && sudo mkdir -p /etc/apt/keyrings \
    && curl -fsSL https://download.zeroc.com/GPG-KEY-zeroc-nightly | sudo gpg --dearmor -o /etc/apt/keyrings/zeroc-nightly.gpg \
    && echo "deb [signed-by=/etc/apt/keyrings/zeroc-nightly.gpg] https://download.zeroc.com/nexus/repository/ubuntu24.04-nightly nightly main" | sudo tee /etc/apt/sources.list.d/zeroc-nightly.list \
    && sudo apt-get update \
    && sudo apt-get install -y zeroc-ice-all-dev

# Set environment variables
ENV DOTNET_ROOT=/home/vscode/.dotnet
ENV PATH=$DOTNET_ROOT:$PATH
ENV LANG=en_US.UTF-8
