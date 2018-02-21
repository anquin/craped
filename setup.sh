#/usr/bin/env bash

set -e

cd diffex-ui && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd editor && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd gtk-ui && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd libsys && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd sharing && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd subeditor && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd tui && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd ui-core && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
cd lib && aclocal && libtoolize && autoheader && automake --add-missing && autoconf && cd ..
aclocal && autoheader && automake --add-missing && autoconf

mkdir build && cd build && ../configure --enable-debug --prefix=$HOME/subroot
