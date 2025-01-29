### SDL 3.2.0
- SDL [https://github.com/libsdl-org/SDL/releases/tag/release-3.2.0](3.2.0) was just released last week, so I took the time to make an SDL3 hello world, as per the tutorial video they linked to on their wiki [https://wiki.libsdl.org/SDL3/Tutorials/FrontPage](here).
- More libraries/frameworks may be added by using git submodules or by altering one or more of the CMakeLists.txt files. 

### Getting Setup
```sh
git clone --recurse-submodules https://github.com/natelwhite/sdl3_hello_world.git
cmake -B build # Generate Build System
cmake --build build # Execute Build System
```
