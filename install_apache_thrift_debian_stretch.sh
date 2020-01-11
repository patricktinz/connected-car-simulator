#!/bin/bash
#
# run as root

# install Apache Thrift
echo "Installing Apache Thrift ..."
sudo apt-get update
git clone https://github.com/apache/thrift.git
sudo apt-get install automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config
cd thrift
./bootstrap.sh
./configure
./configure --with-boost=/usr/local
make
sudo make install
cd ..
apt-get install automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config --no-install-recommends bison build-essential cmake flex pkg-config autoconf automake g++ git libtool make libboost-dev libssl-dev
