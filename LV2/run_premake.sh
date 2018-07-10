#!/bin/bash

cd $(dirname $0)

premake5 --cc=gcc --os=linux gmake
