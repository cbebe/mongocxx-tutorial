#!/bin/sh

TOP_DIR=$(pwd)
sudo apt-get install libmongoc-1.0-0 libbson-1.0 cmake libssl-dev libsasl2-dev zlib1g-dev

wget https://github.com/mongodb/mongo-c-driver/releases/download/1.17.0/mongo-c-driver-1.17.0.tar.gz
tar xzf mongo-c-driver-1.17.0.tar.gz
cd mongo-c-driver-1.17.0
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
sudo make install

git clone https://github.com/mongodb/mongo-cxx-driver.git \
  --branch releases/stable --depth 1
cd mongo-cxx-driver/build

sudo cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBSONCXX_POLY_USE_MNMLSTC=1 \
  -DCMAKE_INSTALL_PREFIX=/usr/local

sudo make
sudo make install
cd $TOP_DIR
sudo rm -rf mongo-c-driver-1.17.0*
