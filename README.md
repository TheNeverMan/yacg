# yacg
YACG or Yet Another Civilization Game is a civilization-like strategy game. Choose one of historical civilizations and play from ancient era to modern.  
If you want to know more, please join YACG Discord Server:  
https://discord.gg/MBxsNUzJGZ

How to install
--------------

Linux
-----

1. Download Linux release from Releases tab.  
2. Unpack archive.  
3. Enjoy.  

Windows
-------

1. Firsty you must install GTK+ for Windows Runtime Environment, download it from https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer and install.
2. Download Windows release from Releases tab.  
3. Unpack archive.  
4. Launch the game, if your antivirus complains about YACG being a virus, turn it off.  

Building
--------
Build YACG yourself.  

1. Clone the repo.  
2. run `make` in the root directory.  

YACG depends on `gtkmm3` and `clang` so you need to make sure that you have them installed.
Alternatively you can use `g++` to build it, if so you need to correct the Makefile.
