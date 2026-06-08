FROM rockylinux:9

RUN dnf install -y dnf-utils \
    && dnf config-manager --enable crb \
    && dnf install -y gcc gcc-c++ cmake ninja-build wget \
    && dnf install -y \
        https://download.zeroc.com/ice/3.8/el9/ice-repo-3.8-1.0.0-1.el9.noarch.rpm \
    && dnf install -y libice-c++-devel \
    && dnf clean all
