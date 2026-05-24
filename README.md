# Kawaii Hizumi

Kawaii Hizumi is a small soft-clip distortion plugin built with JUCE.

- macOS GarageBand: AU component
- macOS Studio One: AU or VST3
- Windows Studio One: VST3
- Build system: CMake + JUCE
- Controls: Drive, Tone, Mix, Output, and Auto Mix

Auto Mix keeps the apparent balance steadier by lowering the effective Mix amount as Drive rises.

## Setup

Install the platform tools:

- macOS: Xcode
- Windows: Visual Studio 2022 with C++ desktop development

Put JUCE in this folder:

```sh
git clone --depth 1 https://github.com/juce-framework/JUCE.git JUCE
```

## macOS Build

The default macOS build targets both Apple Silicon and Intel Macs:

```text
arm64;x86_64
```

Configure and build AU, VST3, and Standalone:

```sh
cmake -S . -B build
cmake --build build --config Release
```

For CI or package-only builds, skip copying into the local plugin folders:

```sh
cmake -S . -B build -DKAWAII_COPY_PLUGIN_AFTER_BUILD=OFF
cmake --build build --config Release
```

Expected macOS outputs:

```text
~/Library/Audio/Plug-Ins/Components/Kawaii Hizumi.component
~/Library/Audio/Plug-Ins/VST3/Kawaii Hizumi.vst3
build/KawaiiHizumi_artefacts/Standalone/Kawaii Hizumi.app
```

If GarageBand is already open, restart it after building.

If you have the full Xcode app selected with `xcode-select`, you can also generate an Xcode project:

```sh
cmake -S . -B build-xcode -G Xcode
cmake --build build-xcode --config Release
```

To override the architecture list manually:

```sh
cmake -S . -B build -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

## Windows Build

Windows builds VST3 and Standalone. AU is Apple-only and is not available on Windows.

From a Visual Studio Developer PowerShell:

```powershell
git clone --depth 1 https://github.com/juce-framework/JUCE.git JUCE
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Expected Windows outputs:

```text
build\KawaiiHizumi_artefacts\Release\VST3\Kawaii Hizumi.vst3
build\KawaiiHizumi_artefacts\Release\Standalone\Kawaii Hizumi.exe
```

Copy the `.vst3` folder to:

```text
C:\Program Files\Common Files\VST3\
```
