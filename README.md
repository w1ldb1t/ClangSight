# ClangSight

## About

ClangSight is a tool designed to streamline the communication between a developer/researcher and clangd, specifically targeting already indexed C++ projects across various operating systems. As a foundational layer, ClangSight facilitates and simplifies the interaction. Still, it shall only be considered a mere demonstration of what could be accomplished by building on top of it.

## Prerequisites

In order to build this project you need to have a few tools already installed:
- [cmake](https://cmake.org/) (3.1 or newer)
- [ninja](https://ninja-build.org/) or [Visual Studio](https://visualstudio.microsoft.com/)
- C++ compiler that supports C++ 20

If your target environment is a Debian-based Linux distro, you can easily satisfy the requirements above by installing the following packages:

```sh
sudo apt install cmake
sudo apt install ninja-build
```

On the other hand, if your target environment is Windows it is easier to just install Visual Studio and follow the building instructions below through Visual Studio's "Native Tools Command Prompt". 

## Usage

1. Clone or download this repository
   ```
   git clone https://github.com/Michelangelo-S/ClangSight
   cd ClangSight
   ```
2. Configure with CMake
   ```
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
   ```
3. Build with Ninja
   ```
   ninja
   ```
4. Run the app
	- Linux:
	   ```
	   ./ClangSight
	   ```
	- Windows:
	   ```
	   .\ClangSight.exe
	   ```

5. Explore the available options
   ```
	./ClangSight --help
	Usage: ClangSight [-h] [--directory VAR] [--commands VAR] [--clangd VAR]
	
	Optional arguments:
	  -h, --help            shows help message and exits
	  -v, --version         prints version information and exits
	  -l, --directory       the root directory of the source code
	  -c, --commands        the directory where "compile_commands.json" resides
	  -d, --clangd          location of the clangd executable
   ```
