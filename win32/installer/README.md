# Installer for Windows
To create an installation package do the following:
1. Install Visual Studio 2019 Community and Visual Studio Installer Projects extension.

2. After building the application run "make install" to copy the resulting binary to data directory (for MinGW Makefile the command will be "mingw32-make install").

3. Open the project that match the toolchain used during compilation, i.e. BallsInRows-x86 for 32-bit and BallsInRows-x64 for 64-bit.

	**Note:** Projects for both architechtures are using the same data directory. Update the contents of this directory before building an installer for another architechture.

4. See what files are missing and copy them from the Qt SDK.

5. Build the installer project.
