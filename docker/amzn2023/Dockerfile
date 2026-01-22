# Dockerfile for testing Ice 3.7 nightly packages on Amazon Linux 2023
FROM public.ecr.aws/amazonlinux/amazonlinux:2023

# Install base dependencies (--allowerasing to replace curl-minimal and gnupg2-minimal)
RUN dnf install -y --allowerasing \
    wget \
    curl \
    gnupg2 \
    gcc \
    gcc-c++ \
    make \
    tar \
    gzip \
    && dnf clean all

# Install Node.js LTS (22.x)
RUN curl -fsSL https://rpm.nodesource.com/setup_22.x | bash - \
    && dnf install -y nodejs \
    && dnf clean all

# Install development languages from package manager
RUN dnf install -y \
    python3 \
    python3-pip \
    java-17-amazon-corretto-devel \
    ruby-devel \
    php-devel \
    php-cli \
    libicu-devel \
    && dnf clean all

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

# Install Ice 3.7 nightly repository (Amazon Linux 2023 packages)
RUN dnf install -y https://download.zeroc.com/ice/nightly/3.7/amzn2023/ice-repo-3.7-nightly-1.0.0-1.amzn2023.noarch.rpm \
    && dnf install -y ice-all-runtime ice-all-devel \
    && dnf clean all

# Set ICE_HOME for the build
ENV ICE_HOME=/usr

# Add script to setup nightly environment on bash startup
COPY scripts/ice-nightly.sh /etc/profile.d/ice-nightly.sh

# Set working directory
WORKDIR /ice-demos

# Default command - use login shell to source profile.d scripts
CMD ["/bin/bash", "-l"]
