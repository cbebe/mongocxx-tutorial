#!/bin/sh

cd $(dirname $0)/../lib

rm -rf served
git clone https://github.com/evanugarte/served.git
sudo apt-get install -y libboost-all-dev

[ -d "SimpleJSON" ] || mkdir SimpleJSON
cd SimpleJSON
curl https://raw.githubusercontent.com/nbsdx/SimpleJSON/master/json.hpp -o json.hpp

cd - >/dev/null
