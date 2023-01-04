# yacg
YACG or Yet Another Civilization Game is a civilization-like strategy game. Choose one of historical civilizations and play from ancient era to modern.  
Download it from itch.io:  
https://theneverman.itch.io/yet-another-civilization-game  
Join Discord:  
https://discord.gg/MBxsNUzJGZ

How to Build From Source
--------------

Linux
-----
YACG depends on `gtkmm3` and `ccache` and by default uses `clang++` to build. Make sure to install `gtkmm3`, you can use `g++` as your compiler. To do so edit `CC_LINUX` in Makefile.  
1. execute `make`
2. Executable named `yacg` will appear in build directory

Windows
-------
Direct building on Windows is currently not supported. However by editing Makefile it is possible to build YACG in MSYS2.  
1. Configure MSYS2 for `mingw-g++` install all required libraries for `gtkmm` with `pacman`
2. Edit Makefile Windows option to: `$(CC_WINDOWS) $(CFLAGS_WINDOWS) *.cpp -o yacg $(LIBS)` (remove export commands)
3. Executable named `yacg.exe` will appear in build directory

Windows in Linux
---------------
By default windows build target in Makefile is used to build Windows .exe file on Linux with Mingw.
1. Clone https://github.com/HolyBlackCat/quasi-msys2 and install `gtkmm3` libraries
2. Install `mingw-g++-x64` with your package manager
3. In Makefile change `CC_WINDOWS` to your Mingw version
4. In Makefile change `WIN_PKG` properties to same that quasi-msys2 to uses. This is used to make Mingw `pkg-config` detect quasi-msys2 packages
5. Execute `make windows`
6. Executable named `yacg.exe` will appear in build directory
