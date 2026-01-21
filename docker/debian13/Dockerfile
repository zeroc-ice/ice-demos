# Dockerfile for testing Ice 3.7 nightly packages on Debian 13 (trixie)
FROM debian:13

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install base dependencies
RUN apt-get update && apt-get install -y \
    wget \
    curl \
    gnupg \
    ca-certificates \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js LTS (22.x)
RUN curl -fsSL https://deb.nodesource.com/setup_22.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

# Install development languages from package manager
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    default-jdk \
    ruby-dev \
    php-dev \
    php-cli \
    libicu-dev \
    && rm -rf /var/lib/apt/lists/*

# Install .NET 10 SDK and .NET 8 runtime using official install script
RUN curl -fsSL https://dot.net/v1/dotnet-install.sh -o /tmp/dotnet-install.sh \
    && chmod +x /tmp/dotnet-install.sh \
    && /tmp/dotnet-install.sh --channel 10.0 --install-dir /usr/share/dotnet \
    && /tmp/dotnet-install.sh --channel 8.0 --runtime dotnet --install-dir /usr/share/dotnet \
    && rm /tmp/dotnet-install.sh \
    && ln -s /usr/share/dotnet/dotnet /usr/bin/dotnet

# Set .NET environment variables
ENV DOTNET_ROOT=/usr/share/dotnet
ENV PATH="${PATH}:/usr/share/dotnet"

# Install Ice 3.7 nightly repository and packages
RUN wget "https://download.zeroc.com/ice/nightly/3.7/debian13/ice-repo-3.7-nightly_1.0.0_all.deb" -O /tmp/ice-repo.deb \
    && dpkg -i /tmp/ice-repo.deb \
    && rm /tmp/ice-repo.deb \
    && apt-get update \
    && apt-get install -y zeroc-ice-all-runtime zeroc-ice-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Set ICE_HOME for the build
ENV ICE_HOME=/usr

# Add script to setup nightly environment on bash startup
COPY scripts/ice-nightly.sh /etc/profile.d/ice-nightly.sh

# Set working directory
WORKDIR /ice-demos

# Default command - use login shell to source profile.d scripts
CMD ["/bin/bash", "-l"]
