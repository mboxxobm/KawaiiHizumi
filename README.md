# Kawaii Drive

GarageBand and Studio One for Mac can load this small distortion plugin.

- GarageBand: AU component
- Studio One: AU or VST3
- Build system: CMake + JUCE
- First effect: soft clip distortion with Drive, Tone, Mix, and Output controls

## Setup

Install Xcode, then put JUCE in this folder:

```sh
git clone --depth 1 https://github.com/juce-framework/JUCE.git JUCE
```

Configure and build:

```sh
cmake -S . -B build
cmake --build build --config Release
```

With `COPY_PLUGIN_AFTER_BUILD TRUE`, JUCE should copy the plugins to the user plugin folders after a successful build.

If you have the full Xcode app selected with `xcode-select`, you can also generate an Xcode project:

```sh
cmake -S . -B build-xcode -G Xcode
cmake --build build-xcode --config Release
```

Expected outputs:

```text
~/Library/Audio/Plug-Ins/Components/Kawaii Drive.component
~/Library/Audio/Plug-Ins/VST3/Kawaii Drive.vst3
```

If GarageBand is already open, restart it after building.
