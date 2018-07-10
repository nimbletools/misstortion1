#!/bin/bash

cd $(dirname $0)

premake5 --cc=clang --os=linux gmake
