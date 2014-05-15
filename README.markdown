Requirements
--------

次に挙げるソフトウェアが必須となります。
予めインストールしておくようにしておきましょう。

* [VirtualBox](https://www.virtualbox.org/)([https://www.virtualbox.org/](https://www.virtualbox.org/))
* [Vagrant](http://www.vagrantup.com/)([http://www.vagrantup.com/](http://www.vagrantup.com/)) 
* Git

筆者の環境で、それぞれのソフトウェアのバージョンは次のようになっています。

    $ vagrant --version
    Vagrant 1.4.3

    $ VBoxManage --version
    4.3.8r92456

    $ git --version
    git version 1.8.5.2 (Apple Git-48)


Starting up
--------

まず GitHub からリポジトリを clone します。
clone してできたディレクトリに、カレントディレクトリを移します。

    $ git clone git@github.com/tube-shaped-fhish-paste-cake/sqe.git path/to/work/sqe
    $ cd path/to/work/sqe

Vagrant で仮想マシンを起動し、プロビジョニングを行います。
プロビジョニング処理によって、ビルドに必要なツールやコンパイラがインストールされるなどします。

    $ vagrant up
    $ vagrant provision
