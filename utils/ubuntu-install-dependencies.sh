#!/bin/bash

# This script is for testing purposes only, as we don't jave a deb package yet

# Make sure only root can run our script
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

if [[ $(dpkg-query -W -f='${Status}\n' libreadline6-dev) == "install ok installed" ]]; then
    echo "libreadline6-dev already installed"
else
    echo "Installing libreadline6-dev"
    apt-get install libreadline6-dev -y
fi

if [[ $(dpkg-query -W -f='${Status}\n' libboost-all-dev) == "install ok installed" ]]; then
    echo "libboost-all-dev already installed"
else
    echo "Installing libboost-all-dev"
    apt-get install libboost-all-dev -y
fi

if [[ $(dpkg-query -W -f='${Status}\n' libprotobuf-dev) == "install ok installed" ]]; then
    echo "libprotobuf-dev already installed"
else
    echo "Installing libprotobuf-dev"
    apt-get install libprotobuf-dev -y
fi

if [[ $(dpkg-query -W -f='${Status}\n' libasio-dev) == "install ok installed" ]]; then
    echo "libasio-dev already installed"
else
    echo "Installing libasio-dev"
    apt-get install libasio-dev -y
fi

echo
echo "Finished ;-)"
