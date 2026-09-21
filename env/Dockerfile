FROM ubuntu:22.04 as env

RUN groupdel dialout

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    libssl-dev \
    zlib1g-dev \
    dos2unix \
    rsync \
    doxygen \
    graphviz \
    libc6-dbg \
    libcap-dev \
    valgrind \
    git \
    cmake \
    wget \
    zip

ARG USER_ID
ARG GROUP_ID

RUN addgroup --gid ${GROUP_ID} user >> /test
RUN adduser --disabled-password --gecos '' --uid ${USER_ID} --gid ${GROUP_ID} user >> /test

RUN mkdir -p /home/user
WORKDIR /home/user/repo

USER user

RUN git config --global --add safe.directory /home/user/repo
