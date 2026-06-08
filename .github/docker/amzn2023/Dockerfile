FROM public.ecr.aws/amazonlinux/amazonlinux:2023

RUN dnf install -y tar gzip gcc gcc-c++ cmake ninja-build wget \
    && dnf install -y \
        https://download.zeroc.com/ice/3.8/amzn2023/ice-repo-3.8-1.0.0-1.amzn2023.noarch.rpm \
    && dnf install -y libice-c++-devel \
    && dnf clean all
