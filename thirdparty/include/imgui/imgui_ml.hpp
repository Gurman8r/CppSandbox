#ifndef _IMGUI_IMPL_ML_HPP_
#define _IMGUI_IMPL_ML_HPP_

#include <imgui/imconfig.h>
#include <stdint.h>

/* * * * * * * * * * * * * * * * * * * * */

struct ImDrawData;

namespace ml
{
	class Window;
}

/* * * * * * * * * * * * * * * * * * * * */

IMGUI_API bool ImGui_ML_Init(const char * glsl_version, ml::Window * window, bool install_callbacks);
IMGUI_API void ImGui_ML_Shutdown();
IMGUI_API void ImGui_ML_NewFrame();
IMGUI_API void ImGui_ML_RenderDrawData(ImDrawData * draw_data);

/* * * * * * * * * * * * * * * * * * * * */

IMGUI_API bool ImGui_ML_CreateFontsTexture();
IMGUI_API void ImGui_ML_DestroyFontsTexture();
IMGUI_API bool ImGui_ML_CreateDeviceObjects();
IMGUI_API void ImGui_ML_DestroyDeviceObjects();

/* * * * * * * * * * * * * * * * * * * * */

IMGUI_API void ImGui_ML_MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods);
IMGUI_API void ImGui_ML_ScrollCallback(void * window, double xoffset, double yoffset);
IMGUI_API void ImGui_ML_KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
IMGUI_API void ImGui_ML_CharCallback(void * window, uint32_t c);

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_IMGUI_IMPL_ML_HPP_
