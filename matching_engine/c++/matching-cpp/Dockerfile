FROM debian:stable

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=GMT
RUN apt -y update \
 && apt -y dist-upgrade
RUN apt -y install software-properties-common build-essential linux-perf net-tools
RUN apt -y install libpthread-stubs0-dev g++ ccache
RUN apt -y update
RUN apt -y install gdb cmake vim lsof strace htop sudo iftop curl tmux
RUN apt install -y python3-pip && pip3 install pip && pip3 install conan
RUN apt -y upgrade
RUN mkdir -p /opt/matching/build/
WORKDIR /opt/matching/
RUN conan profile new default --detect
RUN conan profile update settings.compiler=gcc default
RUN conan profile update settings.compiler.version=8 default
RUN conan profile update settings.compiler.libcxx=libstdc++ default
RUN conan profile update env.CC=gcc default
RUN conan profile update env.CXX=g++ default
RUN conan profile update env.CXXFLAGS=-std=c++17 default
ENV CXXFLAGS=-std=c++17
ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++
EXPOSE 9001 8080
ADD . /opt/matching
RUN make BUILD_TYPE="Release"
RUN make test
