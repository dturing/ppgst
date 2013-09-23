#!/bin/sh
CONF=""

set -x
aclocal -I autoconfig/m4
libtoolize --force --copy
autoheader
automake --add-missing --copy
autoconf

echo CONFIGURING with $CONF
./configure $CONF
