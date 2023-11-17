# About WolfRenderer
 * 3D Game Engine utilizing the Vulkan Graphics API, SDL3, ImGui, and spdlog for Windows!

 * Project is made primarily with C/C++ and built using Premake and the Lua scripting language. 
   
 * **DISCLAIMER** This Game Engine is currently in its very early stages of development. Feel free to follow along on the journey  and watch (or partake) as this     project grows to something great!
    
 * Goal to incorporate functionality with other Operating Systems in the future; currently Windows only.
   
 * Inspired and influenced by the Cherno's Game Engine Series and Vulkan Tutorial! 

## Requirements: 

### Windows Operating System

*  _Why is This Required?_ Other OS implementations have not been incorporated into the design!
  
### Visual Studio

* [Visual Studio Download Link](https://visualstudio.microsoft.com/downloads/) 
* After clicking the link, Select "Community" and follow download instructions from there.

* _Why is This Required?_  Technically it isn't, though this project was made with Visual Studio and works best in its current iteration with this IDE.

### CMake
* [CMake Download Link](https://cmake.org/download/)
* Select your preferred download of CMake and follow instructions from there. Recommended to download the Windows x64 binary for ease of setup 

* _Why Is this required?_  SDL3 Requires CMake to build statically. Even though the primary build system is Premake, building SDL3 without issue requires CMake to generate the project.
  
   * All the necessary code is in the GenerateProjects.bat file to have everything flow seamlessly. Once all requirements are set on your computer, the .bat file will take care of the rest.

## Additional Notes
   * Premake, ImGui, SDL are included directly or as subrepositories in this project. 

# Setup
   * In the Project Repository, click **Code** and copy the link to clone your repository in location of your choosing.

   * Once repository has been cloned, double click the GenerateProjects.bat file and your Visual Studio project will be set up and ready to build in all formats! (Debug, Release, Dist) 

