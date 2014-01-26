#!/bin/sh

# This script is supposed to get a fresh copy of libbacktrace from GCC svn
# repository If it doesn't, which means the relevent source files in GCC
# repository are updated significantly, fix this script accordingly.

# The svn command
SVN=svn

# Repository base.
BASE=svn://gcc.gnu.org/svn/gcc

if [ -z "$1" ]; then
	echo "Usage: $0 <target directory> [<branch>]"
	exit 1
fi
TARGET="$1"
BRANCH="$2"
if [ -z "$BRANCH" ]; then
	BASE=$BASE/trunk
else
	BASE=$BASE/branches/$BRANCH
fi

if [ -d "$TARGET" ]; then
	echo "Directory $TARGET already exists, remove it first or use a different directory."
	exit 1
fi

needed="autoconf automake libtoolize"

for f in $needed; do
	if type $needed 1>/dev/null 2>/dev/null; then
		continue
	else
		echo "$f is unavailable, please install it first."
		exit 1
	fi
done

$SVN export $BASE/libbacktrace $TARGET/backtrace
$SVN export $BASE/include $TARGET/include
$SVN export $BASE/config $TARGET/config

cd $TARGET/backtrace

# TODO The use of autotools is very unprofessional.
sed -i '/dnl Ensure exactly this Autoconf version is used/d' ../config/override.m4
autoconf_version=`autoconf -V | grep "autoconf" | tr ' ' '\n' | tail -1`
sed -i "s/2.64/${autoconf_version}/g" ../config/override.m4

aclocal -I .. -I ../config --install
autoconf
libtoolize --install --copy
automake --add-missing --copy
rm -rf autom4te.cache
