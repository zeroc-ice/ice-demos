FROM rockylinux/rockylinux:10

RUN dnf install -y dnf-utils \
    && dnf config-manager --enable crb \
    && dnf install -y gcc gcc-c++ cmake ninja-build wget \
    && dnf install -y \
        https://download.zeroc.com/ice/nightly/3.9/el10/ice-repo-3.9-nightly-1.0.0-1.el10.noarch.rpm \
    && dnf install -y libice-c++-devel \
    && dnf clean all
