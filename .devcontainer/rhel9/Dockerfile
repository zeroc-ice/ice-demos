FROM rockylinux/rockylinux:9

# Install required build tools and dependencies
RUN dnf install -y \
    sudo \
    make \
    curl-minimal \
    git \
    g++ \
    php python3 ruby gdb \
    java-17-openjdk-devel \
    cmake \
    && dnf clean all

# Install Ice repository and packages
RUN dnf install -y https://download.zeroc.com/ice/nightly/el9/ice-repo-nightly-1.0.0-1.el9.noarch.rpm \
    && dnf install -y \
    dsnode  \
    glacier2 \
    ice-slice \
    ice-utils \
    ice2slice \
    icebox \
    icebridge \
    icegrid \
    icegridgui \
    libice-c++-devel \
    libice3.8-c++ \
    icestorm \
    php-ice \
    python3-ice \
    && dnf clean all

# Create a non-root user and set permissions
RUN useradd -ms /bin/bash vscode \
    && mkdir -p /home/vscode \
    && chown -R vscode:vscode /home/vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/vscode

USER vscode
WORKDIR /home/vscode

ENV LANG=en_US.UTF-8
