FROM ubuntu:eoan
RUN apt-get update && apt-get install -y -q eatmydata \
    && eatmydata apt-get update && apt-get install -y -q  \
        cmake \
        gcc \
        git \
        libexpat1-dev \
        make \
        wget \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /opt/src/nifti_clib
RUN mkdir /nifti_build
COPY . /opt/src/nifti_clib/


WORKDIR /nifti_build
RUN cmake \
    -DBUILD_SHARED_LIBS=ON \
    -DNIFTI_USE_PACKAGING=ON \
    -DUSE_CIFTI_CODE=ON \
    -DUSE_FSL_CODE=ON \
    /opt/src/nifti_clib \
    && make \
    && ctest --output-on-failure
