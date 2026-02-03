# Dockerfile for testing Ice 3.7 packages on Amazon Linux 2023
FROM public.ecr.aws/amazonlinux/amazonlinux:2023

# Build argument to select package quality: stable or nightly
ARG QUALITY=stable

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
    php8.4-devel \
    php8.4-cli \
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

# Install Ice 3.7 repository (stable or nightly based on QUALITY arg)
RUN if [ "$QUALITY" = "nightly" ]; then \
        ICE_REPO_URL="https://download.zeroc.com/ice/nightly/3.7/amzn2023/ice-repo-3.7-nightly-1.0.0-1.amzn2023.noarch.rpm"; \
    else \
        ICE_REPO_URL="https://download.zeroc.com/ice/3.7/amzn2023/ice-repo-3.7.amzn2023.noarch.rpm"; \
    fi && \
    dnf install -y "$ICE_REPO_URL" \
    && dnf install -y ice-all-runtime ice-all-devel \
    && dnf clean all

# Set ICE_HOME for the build
ENV ICE_HOME=/usr

# Persist QUALITY for runtime scripts
ENV QUALITY=$QUALITY

# Add script to setup environment on bash startup
COPY scripts/ice-nightly.sh /etc/profile.d/ice-nightly.sh

# Set working directory
WORKDIR /ice-demos

# Default command - use login shell to source profile.d scripts
CMD ["/bin/bash", "-l"]
