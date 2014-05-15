#!/bin/sh

echo '------------------------------------------------------------'
echo '  Setting up sqe development environment'
echo '------------------------------------------------------------'

# install requirements
apt-get update
apt-get install -y gcc make unzip git

# 'download Squirrel archive
