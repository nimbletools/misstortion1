#!/usr/bin/bash

set -e

# Make sure DISTRHO-juce has been cloned
if [ ! -n "$(ls -A ./Libraries/DISTRHO-juce)" ]; then
	echo "The DISTRHO-juce folder seems empty." \
		 "Please run \"git submodule update --init --recursive\" to clone the missing submodule, then retry running this script."
	exit 1
fi

make -C ./Libraries/DISTRHO-juce/extras/Projucer/Builds/LinuxMakefile/

# Generate JuceLibraryCode
./Libraries/DISTRHO-juce/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave Misstortion.jucer

# Build the plugin
make -C ./LV2/
