FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y gcc g++ cmake ninja-build wget \
    && wget https://download.zeroc.com/ice/3.8/ubuntu24.04/ice-repo-3.8_1.0.0_all.deb \
        -O /tmp/ice-repo.deb \
    && dpkg -i /tmp/ice-repo.deb \
    && rm /tmp/ice-repo.deb \
    && apt-get update \
    && apt-get install -y libzeroc-ice-dev \
    && rm -rf /var/lib/apt/lists/*
