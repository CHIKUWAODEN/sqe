Requirements
--------

次に挙げるソフトウェアが必須となります。
予めインストールしておくようにしておきましょう。

* [VirtualBox](https://www.virtualbox.org/)([https://www.virtualbox.org/](https://www.virtualbox.org/))
* [Vagrant](http://www.vagrantup.com/)([http://www.vagrantup.com/](http://www.vagrantup.com/)) 
* Git

筆者の環境で、それぞれのソフトウェアのバージョンは次のようになっています。
*`$` はプロンプト記号です。今後の解説においてホストマシン上でのプロンプト記号を `$` 
ホストマシン上でのプロンプト記号を `$$` とします*

    $ vagrant --version
    Vagrant 1.6.1

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
プロビジョニング処理によって、ビルドに必要なツールやコンパイラがインストールされたり、環境変数が設定されます。

    $ vagrant up
    $ vagrant provision

プロビジョニングを上手く終えることができたら、仮想マシンにログインします。
`vagrant ssh` コマンドによって、ユーザー vagrant として仮想マシンにログイン可能です。
Vagant によって共有フォルダが設定されており、
`/home/vagrant/sqe` 以下に sqe のソースコードが見えるような状態になっています。

    $ vagrant ssh
    $$ cd ~/sqe

ここまでで実際にビルドする準備は整いました。
ビルドの方法について引き続き次のセクションを御覧ください。

- - - - 

Build and Targets
-------

sqe プロジェクトでは古典的かつ簡単な Makefile によるビルド環境を提供しています。
Makefile には sqe 本体のビルド、SEL のビルド、簡単なテストの実行を行うターゲットが設定されています。


### 各ターゲットについて

**sqe32**

sqe インタプリタ本体と SEL のビルドが行なわれます。
sqe はプロジェクトディレクトリの `bin` ディレクトリに配置されます。
SEL は後述する `install_extend` ターゲットにより、所定の位置に配置されるようになっています。

**install_extend**

ビルド済みの SEL を sqe が参照できる所定の位置に設置します。
具体的には `~/.sqe/sel` ディレクトリ以下に SEL をコピーするといった操作が行なわれます。

**test**

テストを実行します。
test ディレクトリ以下に配置されている簡単なテスト用のスクリプトなどを実行します。
