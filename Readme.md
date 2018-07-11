# Misstortion

Distortion plugin for VST-based hosts. Does its best to be like Logic's Clip Distortion, which makes it very useful for the hardstyle genre.

Download prebuilt binaries [on the Itch.io page](https://nimble.itch.io/misstortion).

* **Hard clip**: Simple hard clip distortion.
* **Soft clip**: Hyperbolic soft clip distortion.
* **Symmetry**: Make your waveform asymmetrical for more finetuned distortion.

It's free! It runs on Windows and Mac DAW's, both 32 bit and 64 bit, so there's no need to bridge them by hand.

# Linux (LV2)

[pdesaulniers](https://github.com/pdesaulniers) maintains a fork of Misstortion for [LV2](http://lv2plug.in/). You can find it here: https://github.com/pdesaulniers/misstortion

# Linux users

To build the plugin as an LV2, run the following commands:

```
git submodule update --init --recursive
./build_lv2_linux.sh
```

The plugin will be placed in the LV2/linux/gmake/build/ folder.

# License

Misstortion is (like the [JUCE](https://juce.com/) library) licensed under the GPL v3.
