# Contributing:

We're looking for a range of contributers to contribute matching code to the KAIN2 decompile project. All matching code must be from the Review Build (14/07/1999).
This is to prevent issues which may arise when merging code from different versions into the game. Auto decompiled code will be accepted, however it must be fully
matching with the original binary to be merged into the project (proof will be required and will be checked).

# Building:

Please select a platform:
  1. [Windows](#Windows)
  2. [Linux](#Linux)
  3. [Mac](#Mac)
  4. [Android](#Android)
  5. ~~Web (Emscripten)~~
  6. [UWP](#Windows)

# Windows
  1. Install Visual Studio 17 2022.
  2. Install Git or GitHub Desktop.
  3. Install CMake (3.24.1 or higher).
  4. Clone the project (and all submodules) using GitHub desktop or Git command line.
  5. Navigate to one of these directories:

  - `(ROOT)/External/libValkyrie/Emulator/Platform/Win32`
  - `(ROOT)/External/libValkyrie/Emulator/Platform/Win64`
  - `(ROOT)/External/libValkyrie/Emulator/Platform/WindowsStore` (for UWP)

  and execute `Generate-Project-Files.bat` to generate the project files (or you can do this via CMake command line but you must always select Win32 architecture).

  6. Open the Solution file located at one of these locations (depending on which configuration you chose at step 5):
  - `(ROOT)/Build/Build_Win32/Soul Reaver.sln`
  - `(ROOT)/Build/Build_Win64/Soul Reaver.sln`
  - `(ROOT)/Build/Build_WindowsStore/Soul Reaver.sln`
  7. Build the project

# Linux
  1. Make sure you are compiling on a 32-bit distro of Linux.
  2. Install Git (sudo apt-get install git)
  3. Install CMake (3.24.1 or higher) `sudo apt-get install -y cmake `.
  4. Install `sudo apt-get install -y libegl1-mesa-dev`.
  5. Install `sudo apt-get install -y libgles2-mesa-dev`.
  6. Install `sudo apt-get install -y libdbus-1-dev`.
  7. Install `sudo apt-get install -y libudev-dev`.
  8. Install `sudo apt-get install -y libasound2-dev`.
  9. Install `sudo apt-get install -y libpulse-dev`.
  10. Install `sudo apt-get install -y libxcursor-dev`.
  11. Install `sudo apt-get install -y libxi-dev`.
  13. Install `sudo apt-get install -y libxrandr-dev`.
  14. Install `sudo apt-get install -y libxss-dev`.
  15. Clone the project (and all sub modules) using Git command line `git clone --recursive https://github.com/Gh0stBlade/KAIN2.git`.
  16. Navigate to `(ROOT)/External/libValkyrie/Emulator/Platform/Linux` and execute `Generate-Project-Files-Win32.sh`.
  17. Run KAIN2

# Mac
  1. Install Xcode 14.2.
  2. Install GitHub or GitHub Desktop.
  3. Install CMake (3.24.1 or higher).
  4. Clone the project (and all sub modules) using GitHub desktop or Git command line.
  5. Open CMake and navigate to the root where you cloned in step 3 using "browse source" button
  6. "Where to build binaries" can be any path you wish.
  7. Open the Solution file located at the path you selected in step 6 within Xcode.
  8. Build KAIN2_Mac.
  
# Android
  1. Install Android Studio plus the latest Android NDK.
  2. Install Github or GitHub Desktop.
  3. Install CMake (3.24.1 or higher).
  4. Clone the project (and all sub modules) using GitHub desktop or Git command line.
  5. Open `(ROOT)/External/libValkyrie/Emulator/Platform/Android` as a project within Android Studio.
  6. Build KAIN2_Android.

# Game files setup:
  1. For debug builds all game files must be extracted from the `BIGFILE.DAT` file. You can do this using the tool in Tools/LST (build it then drag and drop `BIGFILE.DAT` ontop, copy `BIGFILE.LST` and `/KAIN2/` folder to where the executable is located))
  2. Release builds always load the game data from the `BIGFILE.DAT` file which must be in the same folder as the main executable.
  3. Move `BIGFILE.DAT` and `LOCALS.TBL` to where the built executable is located.
  4. Note: 64-bit support is W.I.P! If building for a 64-bit platform, all assets (when extracted from `BIGFILE.DAT` must be passed through the ShiftGlyph tool located in `ROOT/KAIN2/Tools/ShiftGlyph/`.

# It is built, now what?

If you wish to contribute functions to the project, it is important that you try to decompile the ones which are currently most needed. To find out which functions are higher priority, run the game in debug by launching `KAIN2_Win32_Debug.exe`. Whilst in-game press the number `6` on the keyboard. This will now have generated a _runtime bounty list_ called `VALKYRIE_RUNTIME_BOUNTY_LIST_KAIN2.txt`. If you open the bounty list, it should tell you which functions are being called but they're not currently implemented. Choose one to decompile.

# How do I know if a function is matching?

You can use http://decomp.me with the `Legacy of Kain: Soul Reaver` preset to check if the code you decompiled matches when recompiled with the original compiler used for this game.

# I have a matching function, how do I contribute it?

We'd prefer that you follow the same commit style as what is currently being used i.e `[FILENAME]: Implement FUNCTIONNAME` and submit the commits via pull requests. Please ensure to link the decomp.me scratch where it's matching.
