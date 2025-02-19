**How to install SDL2 using MSYS2 if you already have it.**
If you don't have you can download it here:

    https://github.com/msys2/msys2-installer/releases/download/2024-12-08/msys2-x86_64-20241208.exe

**Open MSYS2 and write this command:**
**Install the MinGW-w64 GCC compiler:**

    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain

  **Press Enter and then Y to install.**


**Set Up the Environment:**

    export PATH="$PATH:/mingw64/bin"

**Open MSYS2 and write this command:**

    pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-libavif mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libjxl mingw-w64-x86_64-libpng mingw-w64-x86_64-libtiff mingw-w64-x86_64-libwebp

**This command will install SDL2, SDL2_image, SDL2_mixer, SDL2_net, SDL2_ttf, and all the required libraries like libavif, libjpeg-turbo, libjxl, libpng, libtiff, and libwebp.**

**Or you can use this command:**

    pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_ttf

**This command will only install SDL2, SDL2_image, SDL2_mixer, SDL2_net, SDL2_ttf.**


**All the file can be found by these instructions:**

  -Executable and Dynamic Library Files: These are the files you will link against when compiling your programs. They can be found in (.exe, .dll files):
  
        msys64/mingw64/bin/

  -Header Files: The header files, which you include in your source code, are located in (.h files):
  
        msys64/mingw64/include/SDL2/

  -Static Library Files: These files are used for static linking and are located in (.a, .lib files):
  
        msys64/mingw64/lib/


**After that you can clone my project and run with this command on terminal:**

    mingw32-make

**Or you can change it to whatever you want by change the mingw32-make.exe name which located in "msys64/ucrt64/mingw32-make.exe". Recommended change it to make. So you can run my project with this command:**

    make
    
