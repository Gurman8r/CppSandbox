#ifndef _IMGUI_IMPL_ML_HPP_
#define _IMGUI_IMPL_ML_HPP_

/* * * * * * * * * * * * * * * * * * * * */

struct ImDrawData;
struct GLFWwindow;

/* * * * * * * * * * * * * * * * * * * * */

bool     ImGui_ML_Init(const char * glsl_version = 0);
bool     ImGui_ML_InitForOpenGL(GLFWwindow * window, bool install_callbacks);
void     ImGui_ML_Shutdown();
void     ImGui_ML_NewFrame();
void     ImGui_ML_RenderDrawData(ImDrawData * draw_data);

/* * * * * * * * * * * * * * * * * * * * */

bool     ImGui_ML_CreateFontsTexture();
void     ImGui_ML_DestroyFontsTexture();
bool     ImGui_ML_CreateDeviceObjects();
void     ImGui_ML_DestroyDeviceObjects();

/* * * * * * * * * * * * * * * * * * * * */

void     ImGui_ML_MouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
void     ImGui_ML_ScrollCallback(GLFWwindow * window, double xoffset, double yoffset);
void     ImGui_ML_KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
void     ImGui_ML_CharCallback(GLFWwindow * window, unsigned int c);

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_IMGUI_IMPL_ML_HPP_
