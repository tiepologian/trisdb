trisdb
======

An open-source in-memory triplestore database with a beautiful GUI and incredible performance.

## Installing on Ubuntu/Debian
We provide binaries for both x86 and amd64 Ubuntu 14.04 and above.
```
sudo add-apt-repository ppa:trisdb/ppa
sudo apt-get update
sudo apt-get install trisdb
```

## Building from source
You should be able to build TrisDB on most Linux-based systems.

### Requirements
* CMake >= v2.8
* C++11 compatible compiler (e.g. g++ 4.8)

### Dependencies
* libboost-all-dev >= 1.48
* libreadline6-dev
* libprotobuf-dev
* libsnappy-dev

### Installation
```
cd build/
cmake ../src/
make
sudo make install
```

## Usage
Start TrisDb
```
sudo start trisdb
```
Open the shell
```
trisdb --shell
```
Stop TrisDb
```
sudo stop trisdb
```

![](http://i1033.photobucket.com/albums/a416/Gianluca_Tiepolo/gpromo_zps7fd0fa75.jpg)
