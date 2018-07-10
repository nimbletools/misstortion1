if not _ACTION then
    -- prevent "attempt to ... (a nil value)" errors
elseif _ACTION == "gmake" then
    print ("=== Generating project files (GNU g++, " .. os.target():upper() .. ") ===")
else
    print "Action not specified\n"
end


workspace "misstortion"
    language "C++"
    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    location (os.target() .. "/" .. _ACTION .. "/")
    targetprefix ""

    files {
        "../Source/*.h",
        "../Source/*.cpp",

        "../JuceLibraryCode/include_juce_audio_basics.cpp",
        "../JuceLibraryCode/include_juce_audio_devices.cpp",
        "../JuceLibraryCode/include_juce_audio_formats.cpp",
        "../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp",
        "../JuceLibraryCode/include_juce_audio_processors.cpp",
        "../JuceLibraryCode/include_juce_audio_utils.cpp",
        "../JuceLibraryCode/include_juce_core.cpp",
        "../JuceLibraryCode/include_juce_cryptography.cpp",
        "../JuceLibraryCode/include_juce_data_structures.cpp",
        "../JuceLibraryCode/include_juce_events.cpp",
        "../JuceLibraryCode/include_juce_graphics.cpp",
        "../JuceLibraryCode/include_juce_gui_basics.cpp",
        "../JuceLibraryCode/include_juce_gui_extra.cpp",
	    "../JuceLibraryCode/BinaryData.cpp",
        "../JuceLibraryCode/BinaryData.h"
    }

    includedirs {
        "../JuceLibraryCode/",
        "../Libraries/DISTRHO-juce/modules/",
        "../Libraries/"
    }

    cppdialect "C++14"

    filter { "system:linux" }
        defines {
            "LINUX=1"
        }

        includedirs {
            "/usr/include",
            "/usr/include/freetype2"
        }

        linkoptions {
            -- check for unresolved symbols in shared libraries
            "-Wl,--no-undefined"
        }

        links {
            "dl",
            "freetype",
            "pthread",
            "rt",
            "X11",
            "Xext"
        }

    filter { "platforms:x32" }
        architecture "x86"

    filter { "platforms:x64" }
        architecture "x86_64"

    filter { "configurations:Debug" }
        defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
        optimize "Off"
        symbols "On"

    filter { "system:linux" }
        buildoptions { "-DHAVE_LROUND", "-fmessage-length=78" }

    filter { "system:linux", "configurations:Debug" }
        warnings "Extra"
        buildoptions { "-fno-inline", "-ggdb" }

    filter { "configurations:Release" }
        defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
        flags { "NoFramePointer" }
        optimize "Speed"

    filter { "system:linux", "configurations:Release" }
        warnings "Extra"
        buildoptions { "-fvisibility=hidden", "-pipe", "-Wno-deprecated"}

    project ("misstortion_lv2")
        kind "SharedLib"
        targetdir "./linux/gmake/build/Misstortion.lv2/"

        defines {
            "JucePlugin_Build_LV2=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_ASIO=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0",
            "JUCE_WEB_BROWSER=0",
            "JucePlugin_LV2URI=\"urn:juce:Misstortion\"",
            "JucePlugin_WantsLV2Latency=0",
            "JucePlugin_WantsLV2TimePos=0",
            "JucePlugin_WantsLV2State=0",
            "JucePlugin_WantsLV2Presets=0"
        }

        filter { "system:linux" }
            targetname "Misstortion"

        filter { "configurations:Debug" }
            objdir ("./linux/gmake/build/.intermediate_" .. os.target() .. "/lv2_debug")

        filter { "configurations:Release" }
            objdir ("./linux/gmake/build/.intermediate_" .. os.target() .. "/lv2_release")

--------------------------------------------------------------------------------
