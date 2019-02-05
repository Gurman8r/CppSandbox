
#include <MemeCore/IO.hpp>
#include <MemeGraphics/OpenGL.hpp>

#include <imgui/imgui_impl_ml.hpp>
#include <imgui/imgui.h>

//#define GLEW_STATIC
//#include <GL/glew.h>
//#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>

/* * * * * * * * * * * * * * * * * * * * */

using namespace ml;

/* * * * * * * * * * * * * * * * * * * * */

static char         g_GlslVersionString[32] = "";
static uint32_t     g_FontTexture = 0;
static uint32_t     g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int32_t      g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int32_t      g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static uint32_t		g_VboHandle = 0, g_ElementsHandle = 0;

/* * * * * * * * * * * * * * * * * * * * */

enum GlfwClientApi
{
	GlfwClientApi_Unknown,
	GlfwClientApi_OpenGL,
	GlfwClientApi_Vulkan
};

static GLFWwindow*          g_Window = NULL;
static GlfwClientApi        g_ClientApi = GlfwClientApi_Unknown;
static double               g_Time = 0.0;
static bool                 g_MouseJustPressed[5] = { false, false, false, false, false };
static GLFWcursor*          g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

/* * * * * * * * * * * * * * * * * * * * */

static GLFWmousebuttonfun   g_PrevUserCallbackMousebutton = NULL;
static GLFWscrollfun        g_PrevUserCallbackScroll = NULL;
static GLFWkeyfun           g_PrevUserCallbackKey = NULL;
static GLFWcharfun          g_PrevUserCallbackChar = NULL;

/* * * * * * * * * * * * * * * * * * * * */

static const char* ImGui_ML_GetClipboardText(void* user_data)
{
	return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_ML_SetClipboardText(void* user_data, const char* text)
{
	glfwSetClipboardString((GLFWwindow*)user_data, text);
}

static bool ImGui_ML_Init(GLFWwindow* window, bool install_callbacks, GlfwClientApi client_api)
{
	g_Window = window;
	g_Time = 0.0;

	// Setup back-end capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendPlatformName = "imgui_impl_glfw";

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io.SetClipboardTextFn = ImGui_ML_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ML_GetClipboardText;
	io.ClipboardUserData = g_Window;

	g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
	g_PrevUserCallbackMousebutton = NULL;
	g_PrevUserCallbackScroll = NULL;
	g_PrevUserCallbackKey = NULL;
	g_PrevUserCallbackChar = NULL;
	if (install_callbacks)
	{
		g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ML_MouseButtonCallback);
		g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ML_ScrollCallback);
		g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ML_KeyCallback);
		g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ML_CharCallback);
	}

	g_ClientApi = client_api;
	return true;
}

static void ImGui_ML_UpdateMousePosAndButtons()
{
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	for (int32_t i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
		g_MouseJustPressed[i] = false;
	}

	// Update mouse position
	const ImVec2 mouse_pos_backup = io.MousePos;
	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
#ifdef __EMSCRIPTEN__
	const bool focused = true; // Emscripten
#else
	const bool focused = glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) != 0;
#endif
	if (focused)
	{
		if (io.WantSetMousePos)
		{
			glfwSetCursorPos(g_Window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
			io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
		}
	}
}

static void ImGui_ML_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(g_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		// Show OS mouse cursor
		// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
		glfwSetCursor(g_Window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

static bool ImGui_ML_CompileShader(uint32_t & program, const char * const * vs, const char * const * fs)
{
	if (!OpenGL::shadersAvailable())
	{
		cerr << ("Shaders are not available on your system.") << endl;
		return false;
	}

	if (!program && !(program = OpenGL::createProgramObject()))
	{
		cerr << ("Failed creating shader object") << endl;
		return false;
	}

	// Create the vertex shader if needed
	if (vs)
	{
		// Create and Compile the shader
		g_VertHandle = OpenGL::createShaderObject(GL::VertexShader);
		OpenGL::shaderSource(g_VertHandle, 2, vs, NULL);
		OpenGL::compileShader(g_VertHandle);

		// Check the Compile log
		if (!OpenGL::getProgramParameter(g_VertHandle, GL::ObjectCompileStatus))
		{
			const char * log = OpenGL::getProgramInfoLog(g_VertHandle);
			OpenGL::deleteShader(g_VertHandle);
			OpenGL::deleteShader(program);
			cerr << ("Failed to compile vertex source: ") << log << endl;
			return false;
		}

		// Attach the shader to the program, and delete it
		OpenGL::attachShader(program, g_VertHandle);
	}

	// Create the fragment shader if needed
	if (fs)
	{
		// Create and Compile the shader
		g_FragHandle = OpenGL::createShaderObject(GL::FragmentShader);
		OpenGL::shaderSource(g_FragHandle, 2, fs, NULL);
		OpenGL::compileShader(g_FragHandle);

		// Check the Compile log
		if (!OpenGL::getProgramParameter(g_FragHandle, GL::ObjectCompileStatus))
		{
			const char * log = OpenGL::getProgramInfoLog(g_FragHandle);
			OpenGL::deleteShader(g_FragHandle);
			OpenGL::deleteShader(program);
			cerr << ("Failed to compile fragment source: ") << log << endl;
			return false;
		}

		// Attach the shader to the program, and delete it
		OpenGL::attachShader(program, g_FragHandle);
	}

	// Link the program
	OpenGL::linkShader(program);

	// Check the link log
	if (!OpenGL::getProgramParameter(program, GL::ObjectLinkStatus))
	{
		const char * log = OpenGL::getProgramInfoLog(program);
		OpenGL::deleteShader(program);
		cerr << ("Failed to link source: ") << log << endl;
		return false;
	}

	OpenGL::flush();

	return true;
}

/* * * * * * * * * * * * * * * * * * * * */

bool ImGui_ML_Init(const char * glsl_version)
{
	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_impl_opengl3";

	// Store GLSL version string so we can refer to it later in case we recreate shaders. Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
#ifdef USE_GL_ES3
	if (glsl_version == NULL)
		glsl_version = "#version 300 es";
#else
	if (glsl_version == NULL)
		glsl_version = "#version 130";
#endif
	IM_ASSERT((int32_t)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
	strcpy(g_GlslVersionString, glsl_version);
	strcat(g_GlslVersionString, "\n");

	return true;
}

bool ImGui_ML_InitForOpenGL(GLFWwindow * window, bool install_callbacks)
{
	return ImGui_ML_Init(window, install_callbacks, GlfwClientApi_OpenGL);
}

void ImGui_ML_Shutdown()
{
	ImGui_ML_DestroyDeviceObjects();

	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	{
		glfwDestroyCursor(g_MouseCursors[cursor_n]);
		g_MouseCursors[cursor_n] = NULL;
	}
	g_ClientApi = GlfwClientApi_Unknown;
}

void ImGui_ML_NewFrame()
{
	if (!g_FontTexture)
		ImGui_ML_CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	// Setup display size (every frame to accommodate for window resizing)
	int32_t w, h;
	int32_t display_w, display_h;
	glfwGetWindowSize(g_Window, &w, &h);
	glfwGetFramebufferSize(g_Window, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	ImGui_ML_UpdateMousePosAndButtons();
	ImGui_ML_UpdateMouseCursor();

	// Gamepad navigation mapping [BETA]
	memset(io.NavInputs, 0, sizeof(io.NavInputs));
	if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
	{
		// Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == GLFW_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); if (v > 1.0f) v = 1.0f; if (io.NavInputs[NAV_NO] < v) io.NavInputs[NAV_NO] = v; }
		int32_t axes_count = 0, buttons_count = 0;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axes_count);
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttons_count);
		MAP_BUTTON(ImGuiNavInput_Activate, 0);     // Cross / A
		MAP_BUTTON(ImGuiNavInput_Cancel, 1);     // Circle / B
		MAP_BUTTON(ImGuiNavInput_Menu, 2);     // Square / X
		MAP_BUTTON(ImGuiNavInput_Input, 3);     // Triangle / Y
		MAP_BUTTON(ImGuiNavInput_DpadLeft, 13);    // D-Pad Left
		MAP_BUTTON(ImGuiNavInput_DpadRight, 11);    // D-Pad Right
		MAP_BUTTON(ImGuiNavInput_DpadUp, 10);    // D-Pad Up
		MAP_BUTTON(ImGuiNavInput_DpadDown, 12);    // D-Pad Down
		MAP_BUTTON(ImGuiNavInput_FocusPrev, 4);     // L1 / LB
		MAP_BUTTON(ImGuiNavInput_FocusNext, 5);     // R1 / RB
		MAP_BUTTON(ImGuiNavInput_TweakSlow, 4);     // L1 / LB
		MAP_BUTTON(ImGuiNavInput_TweakFast, 5);     // R1 / RB
		MAP_ANALOG(ImGuiNavInput_LStickLeft, 0, -0.3f, -0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickRight, 0, +0.3f, +0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickUp, 1, +0.3f, +0.9f);
		MAP_ANALOG(ImGuiNavInput_LStickDown, 1, -0.3f, -0.9f);
#undef MAP_BUTTON
#undef MAP_ANALOG
		if (axes_count > 0 && buttons_count > 0)
			io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
		else
			io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
	}
}

void ImGui_ML_RenderDrawData(ImDrawData * draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int32_t fb_width = (int32_t)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
	int32_t fb_height = (int32_t)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width <= 0 || fb_height <= 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	uint32_t last_active_texture = OpenGL::getInt(GL::ActiveTexture);
	OpenGL::activeTexture(GL::Texture0);
	int32_t last_program = OpenGL::getInt(GL::CurrentProgram);
	int32_t last_texture = OpenGL::getInt(GL::TextureBinding2D);
	int32_t last_sampler = OpenGL::getInt(GL::SamplerBinding);
	int32_t last_array_buffer = OpenGL::getInt(GL::ArrayBufferBinding);
	int32_t last_vertex_array = OpenGL::getInt(GL::VertexArrayBinding);

	int32_t last_polygon_mode[2]; 
	OpenGL::getIntv(GL::PolygonMode, last_polygon_mode);

	int32_t last_viewport[4]; 
	OpenGL::getIntv(GL::Viewport, last_viewport);

	int32_t last_scissor_box[4]; 
	OpenGL::getIntv(GL::ScissorBox, last_scissor_box);

	uint32_t last_blend_src_rgb = OpenGL::getInt(GL::BlendSourceRGB);
	uint32_t last_blend_dst_rgb = OpenGL::getInt(GL::BlendDestRGB);
	uint32_t last_blend_src_alpha = OpenGL::getInt(GL::BlendSourceAlpha);
	uint32_t last_blend_dst_alpha = OpenGL::getInt(GL::BlendDestAlpha);
	uint32_t last_blend_equation_rgb = OpenGL::getInt(GL::BlendEquationRGB);
	uint32_t last_blend_equation_alpha = OpenGL::getInt(GL::BlendEquationAlpha);

	bool last_enable_blend = OpenGL::isEnabled(GL::Blend);
	bool last_enable_cull_face = OpenGL::isEnabled(GL::CullFace);
	bool last_enable_depth_test = OpenGL::isEnabled(GL::DepthTest);
	bool last_enable_scissor_test = OpenGL::isEnabled(GL::ScissorTest);
	bool clip_origin_lower_left = true;

	uint32_t last_clip_origin = OpenGL::getInt(GL::ClipOrigin); // Support for GL 4.5's glClipControl(GL::UpperLeft)
	if (last_clip_origin == GL::UpperLeft)
		clip_origin_lower_left = false;

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	OpenGL::enable(GL::Blend);
	OpenGL::blendEquation(GL::FuncAdd);
	OpenGL::blendFunc(GL::SourceAlpha, GL::OneMinusSourceAlpha);
	OpenGL::disable(GL::CullFace);
	OpenGL::disable(GL::DepthTest);
	OpenGL::enable(GL::ScissorTest);
	OpenGL::polygonMode(GL::FrontAndBack, GL::Fill);

	// Setup viewport, orthographic projection matrix
	// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
	OpenGL::viewport(0, 0, (int32_t)fb_width, (int32_t)fb_height);
	float L = draw_data->DisplayPos.x;
	float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
	float T = draw_data->DisplayPos.y;
	float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
	const float ortho_projection[4][4] =
	{
		{ 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
		{ 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
		{ 0.0f,         0.0f,        -1.0f,   0.0f },
		{ (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
	};
	OpenGL::useShader(g_ShaderHandle);
	OpenGL::uniform1i(g_AttribLocationTex, 0);
	OpenGL::uniformMatrix4fv(g_AttribLocationProjMtx, 1, false, &ortho_projection[0][0]);
	OpenGL::bindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
	// Recreate the VAO every time
	// (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
	uint32_t vao_handle = OpenGL::genVertexArrays(1);
	OpenGL::bindVertexArray(vao_handle);
	OpenGL::bindBuffer(GL::ArrayBuffer, g_VboHandle);
	OpenGL::enableVertexAttribArray(g_AttribLocationPosition);
	OpenGL::enableVertexAttribArray(g_AttribLocationUV);
	OpenGL::enableVertexAttribArray(g_AttribLocationColor);
	OpenGL::vertexAttribPointer(g_AttribLocationPosition, 2, GL::Float, false, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, pos));
	OpenGL::vertexAttribPointer(g_AttribLocationUV, 2, GL::Float, false, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, uv));
	OpenGL::vertexAttribPointer(g_AttribLocationColor, 4, GL::UnsignedByte, true, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, col));

	// Draw
	ImVec2 pos = draw_data->DisplayPos;
	for (int32_t n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		OpenGL::bindBuffer(GL::ArrayBuffer, g_VboHandle);
		OpenGL::bufferData(GL::ArrayBuffer, (int32_t)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const void*)cmd_list->VtxBuffer.Data, GL::StreamDraw);

		OpenGL::bindBuffer(GL::ElementArrayBuffer, g_ElementsHandle);
		OpenGL::bufferData(GL::ElementArrayBuffer, (int32_t)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const void*)cmd_list->IdxBuffer.Data, GL::StreamDraw);

		for (int32_t cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				// User callback (registered via ImDrawList::AddCallback)
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
				if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
				{
					// Apply scissor/clipping rectangle
					if (clip_origin_lower_left)
						OpenGL::scissor((int32_t)clip_rect.x, (int32_t)(fb_height - clip_rect.w), (int32_t)(clip_rect.z - clip_rect.x), (int32_t)(clip_rect.w - clip_rect.y));
					else
						OpenGL::scissor((int32_t)clip_rect.x, (int32_t)clip_rect.y, (int32_t)clip_rect.z, (int32_t)clip_rect.w); // Support for GL 4.5's glClipControl(GL::UpperLeft)

					// Bind texture, Draw
					OpenGL::bindTexture(GL::Texture2D, (uint32_t)(intptr_t)pcmd->TextureId);
					OpenGL::drawElements(
						GL::Triangles,
						(int32_t)pcmd->ElemCount,
						sizeof(ImDrawIdx) == 2 ? GL::UnsignedShort : GL::UnsignedInt,
						idx_buffer_offset);
				}
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}
	OpenGL::deleteVertexArrays(1, &vao_handle);

	// Restore modified GL state
	OpenGL::useShader(last_program);
	OpenGL::bindTexture(GL::Texture2D, last_texture);
	OpenGL::bindSampler(0, last_sampler);
	OpenGL::activeTexture(last_active_texture);
	OpenGL::bindVertexArray(last_vertex_array);
	OpenGL::bindBuffer(GL::ArrayBuffer, last_array_buffer);
	OpenGL::blendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	OpenGL::blendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	if (last_enable_blend) OpenGL::enable(GL::Blend); else OpenGL::disable(GL::Blend);
	if (last_enable_cull_face) OpenGL::enable(GL::CullFace); else OpenGL::disable(GL::CullFace);
	if (last_enable_depth_test) OpenGL::enable(GL::DepthTest); else OpenGL::disable(GL::DepthTest);
	if (last_enable_scissor_test) OpenGL::enable(GL::ScissorTest); else OpenGL::disable(GL::ScissorTest);
	OpenGL::polygonMode(GL::FrontAndBack, (uint32_t)last_polygon_mode[0]);
	OpenGL::viewport(last_viewport[0], last_viewport[1], (int32_t)last_viewport[2], (int32_t)last_viewport[3]);
	OpenGL::scissor(last_scissor_box[0], last_scissor_box[1], (int32_t)last_scissor_box[2], (int32_t)last_scissor_box[3]);
}

/* * * * * * * * * * * * * * * * * * * * */

bool ImGui_ML_CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	uint8_t* pixels;
	int32_t width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

	// Upload texture to graphics system
	int32_t last_texture = OpenGL::getInt(GL::TextureBinding2D);
	g_FontTexture = OpenGL::genTextures(1);
	OpenGL::bindTexture(GL::Texture2D, g_FontTexture);
	OpenGL::texParameter(GL::Texture2D, GL::TexMinFilter, GL::Linear);
	OpenGL::texParameter(GL::Texture2D, GL::TexMagFilter, GL::Linear);
	OpenGL::pixelStore(GL::UnpackRowLength, 0);
	OpenGL::texImage2D(GL::Texture2D, 0, GL::RGBA, width, height, 0, GL::RGBA, GL::UnsignedByte, pixels);

	// Store our identifier
	io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

	// Restore state
	OpenGL::bindTexture(GL::Texture2D, last_texture);

	return true;
}

void ImGui_ML_DestroyFontsTexture()
{
	if (g_FontTexture)
	{
		ImGuiIO& io = ImGui::GetIO();
		OpenGL::deleteTextures(1, &g_FontTexture);
		io.Fonts->TexID = 0;
		g_FontTexture = 0;
	}
}

bool ImGui_ML_CreateDeviceObjects()
{
	// Backup GL state
	int32_t last_texture = OpenGL::getInt(GL::TextureBinding2D);
	int32_t last_array_buffer = OpenGL::getInt(GL::ArrayBufferBinding);
	int32_t last_vertex_array = OpenGL::getInt(GL::VertexArrayBinding);

	// Parse GLSL version string
	int32_t glsl_version = 130;
	sscanf(g_GlslVersionString, "#version %d", &glsl_version);

	const char* vertex_shader_glsl_120 =
		"uniform mat4 ProjMtx;\n"
		"attribute vec2 Position;\n"
		"attribute vec2 UV;\n"
		"attribute vec4 Color;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"    Frag_UV = UV;\n"
		"    Frag_Color = Color;\n"
		"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const char* vertex_shader_glsl_130 =
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"    Frag_UV = UV;\n"
		"    Frag_Color = Color;\n"
		"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const char* vertex_shader_glsl_300_es =
		"precision mediump float;\n"
		"layout (location = 0) in vec2 Position;\n"
		"layout (location = 1) in vec2 UV;\n"
		"layout (location = 2) in vec4 Color;\n"
		"uniform mat4 ProjMtx;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"    Frag_UV = UV;\n"
		"    Frag_Color = Color;\n"
		"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const char* vertex_shader_glsl_410_core =
		"layout (location = 0) in vec2 Position;\n"
		"layout (location = 1) in vec2 UV;\n"
		"layout (location = 2) in vec4 Color;\n"
		"uniform mat4 ProjMtx;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"    Frag_UV = UV;\n"
		"    Frag_Color = Color;\n"
		"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const char* fragment_shader_glsl_120 =
		"#ifdef GL_ES\n"
		"    precision mediump float;\n"
		"#endif\n"
		"uniform sampler2D Texture;\n"
		"varying vec2 Frag_UV;\n"
		"varying vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
		"}\n";

	const char* fragment_shader_glsl_130 =
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	const char* fragment_shader_glsl_300_es =
		"precision mediump float;\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"layout (location = 0) out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	const char* fragment_shader_glsl_410_core =
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"uniform sampler2D Texture;\n"
		"layout (location = 0) out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	// Select shaders matching our GLSL versions
	const char* vertex_shader = NULL;
	const char* fragment_shader = NULL;
	if (glsl_version < 130)
	{
		vertex_shader = vertex_shader_glsl_120;
		fragment_shader = fragment_shader_glsl_120;
	}
	else if (glsl_version == 410)
	{
		vertex_shader = vertex_shader_glsl_410_core;
		fragment_shader = fragment_shader_glsl_410_core;
	}
	else if (glsl_version == 300)
	{
		vertex_shader = vertex_shader_glsl_300_es;
		fragment_shader = fragment_shader_glsl_300_es;
	}
	else
	{
		vertex_shader = vertex_shader_glsl_130;
		fragment_shader = fragment_shader_glsl_130;
	}

	// Create shaders
	const char* vertex_shader_with_version[2] = { g_GlslVersionString, vertex_shader };
	const char* fragment_shader_with_version[2] = { g_GlslVersionString, fragment_shader };

	ImGui_ML_CompileShader(g_ShaderHandle, vertex_shader_with_version, fragment_shader_with_version);

	g_AttribLocationTex		= OpenGL::getUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = OpenGL::getUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition= OpenGL::getAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV		= OpenGL::getAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor	= OpenGL::getAttribLocation(g_ShaderHandle, "Color");

	// Create buffers
	g_VboHandle = OpenGL::genBuffers(1);
	g_ElementsHandle = OpenGL::genBuffers(1);

	ImGui_ML_CreateFontsTexture();

	// Restore modified GL state
	OpenGL::bindTexture(GL::Texture2D, last_texture);
	OpenGL::bindBuffer(GL::ArrayBuffer, last_array_buffer);
	OpenGL::bindVertexArray(last_vertex_array);

	return true;
}

void ImGui_ML_DestroyDeviceObjects()
{
	if (g_VboHandle) OpenGL::deleteBuffers(1, &g_VboHandle);
	if (g_ElementsHandle) OpenGL::deleteBuffers(1, &g_ElementsHandle);
	g_VboHandle = g_ElementsHandle = 0;

	if (g_ShaderHandle && g_VertHandle) OpenGL::detachShader(g_ShaderHandle, g_VertHandle);
	if (g_VertHandle) OpenGL::deleteShader(g_VertHandle);
	g_VertHandle = 0;

	if (g_ShaderHandle && g_FragHandle) OpenGL::detachShader(g_ShaderHandle, g_FragHandle);
	if (g_FragHandle) OpenGL::deleteShader(g_FragHandle);
	g_FragHandle = 0;

	if (g_ShaderHandle) OpenGL::deleteShader(g_ShaderHandle);
	g_ShaderHandle = 0;

	ImGui_ML_DestroyFontsTexture();
}


/* * * * * * * * * * * * * * * * * * * * */

void ImGui_ML_MouseButtonCallback(GLFWwindow * window, int32_t button, int32_t action, int32_t mods)
{
	if (g_PrevUserCallbackMousebutton != NULL)
		g_PrevUserCallbackMousebutton(window, button, action, mods);

	if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
		g_MouseJustPressed[button] = true;
}

void ImGui_ML_ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	if (g_PrevUserCallbackScroll != NULL)
		g_PrevUserCallbackScroll(window, xoffset, yoffset);

	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;
}

void ImGui_ML_KeyCallback(GLFWwindow * window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	if (g_PrevUserCallbackKey != NULL)
		g_PrevUserCallbackKey(window, key, scancode, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui_ML_CharCallback(GLFWwindow * window, uint32_t c)
{
	if (g_PrevUserCallbackChar != NULL)
		g_PrevUserCallbackChar(window, c);

	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
		io.AddInputCharacter((unsigned short)c);
}
