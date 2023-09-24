call cmake -S WolfRenderer/WolfRenderer/vendor/SDL3 -DSDL_STATIC=ON -DSDL_SHARED=OFF -B WolfRenderer/WolfRenderer/vendor/SDL3/build
call vendor\Premake\premake5.exe vs2022
PAUSE