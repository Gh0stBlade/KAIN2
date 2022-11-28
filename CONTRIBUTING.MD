# Contributing:

We're looking for a range of contributers to contribute matching code to the KAIN2 decompile project. All matching code must be from the Review Build (14/07/1999).
This is to prevent issues which may arise when merging code from different versions into the game. Auto decompiled code will be accepted, however it must be fully
matching with the original binary to be merged into the project (proof will be required and will be checked).

# Building:

Please select a platform:
  1. [Windows](#Windows)
  2. ~~Linux~~
  3. ~~Mac~~
  4. ~~Android~~
  5. ~~Web (Emscripten)~~
  6. ~~UWP~~

# Windows
  1. Install Visual Studio 17 2022.
  2. Install GitHub or GitHub Desktop.
  3. Install CMake (3.24.1 or higher).
  4. Clone the project (and all sub modules) using GitHub desktop or Git command line.
  5. Navigate to `Emulator/Platform/Windows` and execute `Generate-Project-Files.bat` to generate the project files (or you can do this via CMake commandline but you must always select Win32 architecture).
  6. Open the Solution file located at `(ROOT)/Build/Build_Win32/Soul Reaver.sln`
  7. Build KAIN2_Win32

# Game files setup:
  1. By default (debug builds) all files must be extracted from `BIGFILE.DAT` to run as it is coded to load files externally and not packed into one file. (You can do this using the tool in Tools/LST (build it then drag and drop BIGFILE.DAT ontop, copy BIGFILE.LST and /KAIN2/ folder to where the executable is located))
  2. By default (release builds) always load files from `BIGFILE.DAT` which must be in the same folder as the main executable.
  3. Move `BIGFILE.DAT` and `LOCALS.TBL` to where the built executable is located.

# It is built, now what?

If you wish to contribute functions to the project, it is important that you try to decompile the ones which are currently most needed. To find out which functions are higher priority, run the game in debug by launching `KAIN2_Win32_Debug.exe`. Whilst in-game press the number `6` on the keyboard. This will now have generated a _runtime bounty list_ called `VALKYRIE_RUNTIME_BOUNTY_LIST_KAIN2.txt`. If you open the bounty list, it should tell you which functions are being called but they're not currently implemented. Chose one to decompile.

# How do I know if a function is matching?

You can use http://decomp.me with the compiler set to `PsyQ 4.3` and `-O2` and `-G65536` compiler flags set to check if the code you decompiled matches when recompiled with the original compiler used for this game.

# I have a matching function, how do I contribute it?

We'd prefer that you follow the same commit style as what is currently being used i.e `[FILENAME]: Implement FUNCTIONNAME` and submit the commits via pull requests. Please ensure to link the decomp.me scratch where it's matching.




