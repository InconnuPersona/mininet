# mininet
A C networked port of minicraft by Markus "Notch" Persson; lua scripting is used for entity, item, menu, and tile code to ease content creation.

The repository does not include the assets of minicraft, but they may be easily taken from their source and inserted. When put in, they should correspond to the folders:
- res - icons.png
- res/snd - .wav files

## Differences from origin:

The code is in C and lua, which use SDL2 to aid platform rendering, sound, and network. The development began in summer of 2019, and was based off another project, [castle](https://github.com/InconnuPersona/castle), which is a private repository at the time.

In contrast to the original source code of minicraft, entities are told as units, inventories are managed under piles, and items are not unique structures. All implementation may be found in the unit-, item-, and resource files of the program. 

All unit, tile, and menu scripts are modifiable, but editing them decreases the networking compatibility unless the server uses the same modifications. Saving a copy of the resources folder is recommended beforehand.

Although the crossplatform ability of the program is hindered by specific errors, the code is made to be cross platform as possible.

## Building:

The repository offers only the code which you may use to compile the project. The original code was written in Eclipse C++ for Windows with both Cygwin and MinGW targets, therefore it is likely to remain stable on those platforms. An Android port may be compiled with the correct configuration, although the lua library must be setup properly for the library compilation. Removing localization of the '.' operator is one of the tasks that must be done to allow compilation with Android's lack of the locale material. If there are any mistakes or issues with the files for it, a further excerpt could be opened on the subject or correction.

On Visual Studio, semicolons placed after macro declarations must be removed to compile without error; the compiler possesses no tolerance for the aesthetic characters. dirent.h must also be implemented or supplied in some form.

## Dependency:
- [lua](https://www.lua.org/) 5.2 or later supporting bit32; the minimum version may change in the future.
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- [SDL2_net](https://www.libsdl.org/projects/SDL_net/)

Haven fun.
