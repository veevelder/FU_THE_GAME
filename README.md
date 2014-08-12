Fuck You The Game
===========
The title pretty much explains everything.

SDL needs to be installed in order to work.

For Windows, MinGW was used to compile.
Credit goes to Lazy Foo Productions for providing tutorials and instructions on how to set up SDL for multiple platforms.
(http://lazyfoo.net/)

Installing SDL
===========
## On Linux
```
apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
```
or
```
yum install SDL-devel SDL_mixer-devel SDL_image-devel SDL_ttf-devel
```

## On Mac
Install Homebrew (http://brew.sh/) past the following in the terminal

  ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
  
Then install SDL with

  brew install sdl
  
## On Windows
We used MinGW to compile for windows. First is to install MinGW if you havent already.

Follow this guide to help install MinGW: http://www.mingw.org/wiki/Getting_Started

Make sure gcc is installed.

Download the SDL Development Libraries

  http://www.libsdl.org/release/SDL-devel-1.2.15-mingw32.tar.gz
  
1) Open gz archive and there should be a *.tar archive inside.

2) Open the *.tar and there should be a folder inside of that.

3) Open the folder and it'll contain a bunch of subfolders.

4) Copy the contents of the lib subfolder to the MinGW lib folder.
The MinGW lib folder should be at C:\MinGW\lib.

5) After that, open the include subfolder in the archive and extract the folder named "SDL" to the MinGW include folder, which should be at C:\MinGW\include.

6) Now take the SDL.dll from the archive (it should be inside the bin subfolder) and extract it. You're going to put this in the same directory as your exe when you compile it.

7) Download the SDL extra libraries

  http://www.libsdl.org/projects/SDL_image/release/SDL_image-devel-1.2.12-VC.zip

  http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.12-VC.zip

  http://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-devel-2.0.11-VC.zip

8) Open up the zip archive and there should be a folder inside.

9) Open the folder and it'll contain 2 subfolders.

10) First, open the include subfolder in the archive and extract the header file inside to the SDL subfolder inside of the MinGW include folder. It should be at C:\MinGW\include\SDL.

11) Now find the lib folder from the archive. Take the library file(s) from the archive and put them with the rest of the SDL library files. The MinGW lib folder should be at C:\MinGW\lib.

12) Now extract all of the *.dll file(s) from the x86 folder in the archive and put them in the same directory as your exe.

