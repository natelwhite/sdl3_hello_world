### SDL 3.2.0
- SDL [https://github.com/libsdl-org/SDL/releases/tag/release-3.2.0](3.2.0) was just released last week! 
- More libraries/frameworks may be added by using git submodules or by altering one or more of the CMakeLists.txt files. 

### Getting Setup
- Assuming you have git and cmake installed...
```sh
git clone --recurse-submodules https://github.com/natelwhite/sdl3_hello_world.git
cmake -B build # Generate Build System
cmake --build build # Execute Build System
```
