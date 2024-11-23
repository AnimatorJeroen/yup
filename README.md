# YUP: Cross-Platform Application And Plugin Development Library

UI courtesy from https://www.drywestdesign.com/:
![SeaSynth](./images/seasynth_prototype.png)

Example rive animations ([source code](./examples/render/source/main.cpp)):
[![Web Render Example 1](./images/web_render_1.png)](https://kunitoki.github.io/yup/demos/web_render_1/)
[![Web Render Example 2](./images/web_render_2.png)](https://kunitoki.github.io/yup/demos/web_render_2/)
[![Web Render Example 3](./images/web_render_3.png)](https://kunitoki.github.io/yup/demos/web_render_3/)

[Renderer Youtube Video](https://youtube.com/shorts/3XC4hyDlrVs)

[![Build And Test MacOS](https://github.com/kunitoki/yup/actions/workflows/build_macos.yml/badge.svg)](https://github.com/kunitoki/yup/actions/workflows/build_macos.yml)
[![Build And Test Windows](https://github.com/kunitoki/yup/actions/workflows/build_windows.yml/badge.svg)](https://github.com/kunitoki/yup/actions/workflows/build_windows.yml)
[![Build And Test Linux](https://github.com/kunitoki/yup/actions/workflows/build_linux.yml/badge.svg)](https://github.com/kunitoki/yup/actions/workflows/build_linux.yml)
[![Build And Test Wasm](https://github.com/kunitoki/yup/actions/workflows/build_wasm.yml/badge.svg)](https://github.com/kunitoki/yup/actions/workflows/build_wasm.yml)

## Introduction
YUP is an open-source library dedicated to empowering developers with advanced tools for cross-platform application and plugin development, featuring state-of-the-art rendering and audio processing. Originating from a fork of [JUCE7](https://juce.com/)'s ISC-licensed modules, YUP builds on the robust, high-performance capabilities that made JUCE7 popular among audio and visual application developers. Unlike its successor JUCE8, which moved to a restrictive AGPL license and an even more costly commercial one, YUP maintains the more permissive ISC license and ensures that all of its dependencies are either liberally licensed or public domain, remaining a freely accessible and modifiable resource for developers worldwide.

> [!CAUTION]
> The project is still in embryonic stage, use it at your own risk!

## Features
YUP brings a suite of powerful features, including:
- **High-Performance Rendering:** From intricate visualizations to high-speed gaming graphics, YUP handles it all with ease and efficiency, relying on the open source [Rive](https://rive.app/) Renderer, backed by Metal, Direct3D, OpenGL and WebGPU.
- **Advanced Audio Processing:** Tailored for professionals, our audio toolkit delivers pristine sound quality with minimal latency, suitable for music production, live performance tools, and more. Based on the JUCE7 module for audio/midi input and output.
- **Open Source Audio Plugin Standards:** Facilitates the development of [CLAP](https://cleveraudio.org/) plugin abstractions, providing a framework for creating versatile and compatible audio plugins.
- **Cross-Platform Compatibility:** Consistent and reliable on Windows, macOS, Linux (with Wasm, iOS and Android in the pipe).
- **Extensive Testing Infrastructure:** Massive set of unit and integration tests to validate functionality.
- **Community-Driven Development:** As an open-source project, YUP thrives on contributions from developers around the globe.

## Supported Rendering Backends
|                          | **Windows**        | **macOS**          | **Linux**          | **WASM**           | **Android**<sup>(1)</sup> | **iOS**<sup>(1)</sup> |
|--------------------------|:------------------:|:------------------:|:------------------:|:------------------:|:-------------------------:|:---------------------:|
| **OpenGL 4.2**           | :white_check_mark: |                    | :white_check_mark: |                    | :white_check_mark:        |                       |
| **OpenGL ES2.0**         |                    |                    |                    | :white_check_mark: |                           |                       |
| **Metal**                |                    | :white_check_mark: |                    |                    |                           | :white_check_mark:    |
| **Direct3D 11**          | :white_check_mark: |                    |                    |                    |                           |                       |
| **Vulkan**<sup>(2)</sup> |                    |                    |                    |                    |                           |                       |

1. Platforms not fully supported by the windowing system
2. Renderer currently work in progress

## Development
> [!IMPORTANT]
> We are looking for collaborators to bring forward the framework!

## Getting Started
To begin using YUP, follow these detailed steps to set up the environment and run a simple example application:

### Prerequisites
Ensure you have the following installed:
- C++17 compatible compiler
- CMake 3.28 or later

### Installation
Clone the YUP repository and build the library using the following commands:

```bash
git clone https://github.com/kunitoki/yup.git
cd yup
mkdir build
cmake -B build
```

### Running Your First Application
Here is a simple example of creating a basic window using YUP:

```cpp
// TODO
```

Save this as `main.cpp` and compile it using:

```bash
# TODO
```

This will open a window titled "Hello YUP" with dimensions 800x600.

## Documentation
For full documentation, including more detailed tutorials and comprehensive API references, please visit [YUP Documentation](https://yup.github.io/docs).

## Community Engagement
Join our growing community and contribute to the YUP project. Connect with us and other YUP developers:
- **GitHub:** [YUP Repository](https://github.com/kunitoki/yup)

## Contributing
Interested in contributing? Here's how you can help:
1. Fork the repository.
2. Create a new branch for your changes (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

## License
YUP is distributed under the ISC License, supporting both personal and commercial use, modification, and distribution without restrictions.

## Acknowledgments
YUP was born in response to JUCE8’s shift to a more restrictive licensing model. By forking JUCE7’s community-driven, ISC-licensed modules, we aim to preserve and continue a legacy of high-quality, freely accessible software development. We are grateful to the JUCE7 community for laying the groundwork for this initiative.
