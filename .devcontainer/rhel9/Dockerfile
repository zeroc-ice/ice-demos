FROM registry.access.redhat.com/ubi9

RUN tee /etc/yum.repos.d/ice-nightly.repo <<EOF
[ice-nightly]
name=Ice Nightly Repository
baseurl=https://download.zeroc.com/nexus/repository/rhel9-nightly
enabled=1
gpgcheck=0
EOF

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
    ice-all-devel \
    && dnf clean all

# Create a non-root user and set permissions
RUN useradd -ms /bin/bash vscode \
    && mkdir -p /home/vscode \
    && chown -R vscode:vscode /home/vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/vscode

USER vscode
WORKDIR /home/vscode

ENV LANG=en_US.UTF-8
