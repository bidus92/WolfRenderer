call glslc.exe WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/shader.vert -o vert.spv
call glslc.exe WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/shader.frag -o frag.spv

move vert.spv WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spirv
move frag.spv WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spirv
PAUSE	