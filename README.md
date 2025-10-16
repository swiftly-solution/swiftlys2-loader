# SwiftlyS2 Loader

A dynamic loader plugin for SwiftlyS2, a scripting framework for Source2-based games.

## Building

This project uses [xmake](https://xmake.io/) as its build system.

### Prerequisites

- C++23 compatible compiler
- xmake build system

### Build Instructions

```bash
xmake
```

The compiled plugin will be built to `build/package/addons/swiftlys2/bin/` with platform-specific subdirectories:
- Windows: `win64/server.dll`
- Linux: `linuxsteamrt64/libserver.so`

## Installation

1. Build the project using xmake
2. Copy the built library from `build/package/addons/swiftlys2/bin/` to your game server's directory
3. Configure your game server to load the plugin

## Usage

### Command-Line Arguments

The loader supports the following command-line parameter:

- `sw_path`: Custom path to SwiftlyS2 installation (default: `addons/swiftlys2`)

Example:
```bash
-sw_path "custom/path/to/swiftlys2"
```

### Dependencies

- **SafetyHook**: Function hooking library
- **s2binlib**: Source2 binary utilities library
- **Zydis**: x86/x64 disassembler library (used by SafetyHook)

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Copyright (C) 2025 Swiftly Solution SRL via Sava Andrei-Sebastian and its contributors.

See the [GNU General Public License](https://www.gnu.org/licenses/) for more details.

## Acknowledgements

- [MetaMod:Source](https://github.com/alliedmodders/metamod-source) -> Call Flow and an idea from where to start
  -> Metamod:Source is distributed under the "zLib/libpng" license.

## Contributing

Contributions are welcome! Please ensure your code follows the project's coding standards and includes appropriate license headers.
