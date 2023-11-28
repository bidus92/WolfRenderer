call cmake -S WolfRenderer/WolfRenderer/vendor/SDL3 -DSDL_STATIC=ON -DSDL_SHARED=OFF -B WolfRenderer/WolfRenderer/vendor/SDL3/build 
del WolfRenderer\WolfRenderer\vendor\SDL3\build\CMakeCache.txt
copy WolfRenderer\WolfRenderer\vendor\SDL3\DistBuildCompatibility\CMakeCache.txt  WolfRenderer\WolfRenderer\vendor\SDL3\build
call cmake -S WolfRenderer/WolfRenderer/vendor/SDL3 -DSDL_STATIC=ON -DSDL_SHARED=OFF -B WolfRenderer/WolfRenderer/vendor/SDL3/build 
start vendor/Premake/premake5.exe vs2022
call WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/CompileShaders.bat
PAUSE