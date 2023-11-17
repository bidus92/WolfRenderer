call glslc.exe WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/shader.vert -o vert.spv
call glslc.exe WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/shader.frag -o frag.spv

move Shaders/vert.spv WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spir-v
move Shader/frag.spv WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spir-v
PAUSE	