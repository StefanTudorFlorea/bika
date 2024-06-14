#------------------------------------------------------------------------------
# BUILD PHASE
#------------------------------------------------------------------------------
FROM ubuntu:latest as build

# system
RUN apt-get update && \
    apt-get -y install git curl zip unzip tar cmake g++ pkg-config autoconf python3 python3-pip python-is-python3


# application
WORKDIR /app

# 3rd-party
RUN mkdir tools && cd tools && \
    git clone https://github.com/Microsoft/vcpkg.git \
    && cd vcpkg && ./bootstrap-vcpkg.sh
COPY vcpkg.json .
RUN ./tools/vcpkg/vcpkg install

# source
COPY src/ src/
COPY config/ config
COPY CMakeLists.txt .

# build
RUN mkdir build && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . --target all

#------------------------------------------------------------------------------
# DEPLOY PHASE
#------------------------------------------------------------------------------
FROM ubuntu:latest as runtime

# copy results from build phase
WORKDIR /app
COPY --from=build /app/build/bin /app
