FROM ubuntu:18.04

COPY CMakeLists.txt /biliroku-build/
COPY src /biliroku-build/src/

ENV GIT_SSL_NO_VERIFY=true

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y g++ cmake git libcurl4-openssl-dev && \
    git clone https://github.com/Tencent/rapidjson.git && \
    cd /biliroku-build && \
    mkdir build && \
    cd build && \
    cmake -DRAPID_JSON_DIR=/rapidjson ../ && \
    make && \
    make install && \
    cp /biliroku-build/bin/biliroku /bin/ && \
    # clean
    rm -rf /biliroku-build && \
    rm -rf /rapidjson

ENTRYPOINT [ "/bin/biliroku" ]