#!/bin/bash
echo '------------------------------------------------------------'
echo '  Setting up sqe development environment'
echo '------------------------------------------------------------'

export LANGUAGE=en_US.UTF-8
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
locale-gen en_US.UTF-8
dpkg-reconfigure locales

# install requirements
apt-get update
apt-get install -y gcc g++ make wget git

# download Squirrel archive, and 'make' it (32bit)
mkdir -p /home/vagrant/tmp
pushd /home/vagrant/tmp
if [ ! -e squirrel_3.0.6.tar.gz ]; then
  wget -O squirrel_3.0.6_stable.tar.gz 'http://sourceforge.jp/frs/g_redir.php?m=jaist&f=%2Fsquirrel%2Fsquirrel3%2Fsquirrel+3.0.6+stable%2Fsquirrel_3_0_6_stable.tar.gz' 
  tar zxvf squirrel_3.0.6_stable.tar.gz
  cd SQUIRREL3
  make
fi
popd

echo "export LIBRARY_PATH=/home/vagrant/tmp/SQUIRREL3/lib:${LIBRARY_PATH}" >> /home/vagrant/.bashrc
echo "export C_INCLUDE_PATH=/home/vagrant/tmp/SQUIRREL3/include:${INCLUDE_PATH}"  >> /home/vagrant/.bashrc
echo "export CPLUS_INCLUDE_PATH=/home/vagrant/tmp/SQUIRREL3/include:${INCLUDE_PATH}"  >> /home/vagrant/.bashrc
