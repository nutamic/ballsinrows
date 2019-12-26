# Balls in Rows
Balls in Rows is a puzzle game about the balls and building them in the rows. The goal of this game is to build rows of 5 or more balls with the same color. If there is such row on the board after your step, the balls of the row will disappear. If not, 3 new balls will appear at random positions. The game is over when there are no space left on the board. Your goal is to build as many rows as you can.

## Building from source
### GNU/Linux
1. Make sure that all needed programs (qmake, moc, uiс, lrelease), make and compiler are working properly.

2. Install development versions of the required libraries (libarchive, QtCore, QtGui, QtSvg, QtWidgets).

3. Run sequencially in the project root:

		lrelease ballsinrows.pro
		qmake -o Makefile ballsinrows.pro
		make

	**Note:** *NEVER* run "make uninstall" because it will recursively delete some directories in /usr/share/icons/hicolor.

4. Since "make install" doesn't handle file collisions, it is a good idea to use package managers for system-wide package installation. To install the application only for current user run:

		cp -r share ~/.local/share/ballsinrows
		mkdir ~/.local/share/ballsinrows/translations
		cp translations/*.qm ~/.local/share/ballsinrows/translations

### Windows
1. Install Qt Creator.

2. Build the following dependencies:

- [zlib](https://github.com/madler/zlib)
- [xz-utils](https://git.tukaani.org/)
- [libarchive](https://github.com/libarchive/libarchive) (with zlib and lzma support)

	Place "archive.h" and "archive_entry.h" in the <headers_path> and the resulting libraries in the <libraries_path>.

3. Start Qt development command prompt and run sequencially in the project root:

		lrelease ballsinrows.pro
		qmake "QMAKE_INCDIR=<headers_path>" "QMAKE_LIBDIR=<libraries_path>" -o Makefile ballsinrows.pro
	
	Then start make. For MinGW Makefile the command will be "mingw32-make".

4. If you want to create an installation package, see [README.md](win32/installer/README.md) in win32/installer.
