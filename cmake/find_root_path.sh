#!/bin/sh

prefix=$(echo -n $(basename $1) | sed 's/-gcc$//')

echo $(dirname $(which $1) )/../$prefix
