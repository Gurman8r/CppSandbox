/* * * * * * * * * * * * * * * * * * * * */

#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Shader.hpp>
#include <MemeWindow/Window.hpp>
#include <MemeWindow/WindowEvents.hpp>

/* * * * * * * * * * * * * * * * * * * * */

enum ClientAPI
{
	API_Unknown,
	API_OpenGL,
	API_Vulkan
};

static ml::Window *	g_Window = NULL;
static ClientAPI	g_ClientApi = API_Unknown;
static double       g_Time = 0.0;
static bool         g_MouseJustPressed[5] = { false, false, false, false, false };
static void *		g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

/* * * * * * * * * * * * * * * * * * * * */

static char         g_GlslVersionString[32] = "";
static uint32_t     g_FontTexture = 0;
static uint32_t     g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int32_t      g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int32_t      g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static uint32_t		g_VboHandle = 0, g_ElementsHandle = 0;

/* * * * * * * * * * * * * * * * * * * * */

static const char * ImGui_ML_GetClipboardText(void* user_data)
{
	return ((ml::Window *)user_data)->getClipboardString().c_str();
}

static void ImGui_ML_SetClipboardText(void* user_data, const char * text)
{
	((ml::Window *)user_data)->setClipboardString(text);
}

static bool ImGui_ML_Init(ml::Window * window, bool install_callbacks, ClientAPI client_api)
{
	g_Window = window;
	g_Time = 0.0;

	// Setup back-end capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendPlatformName = "imgui_impl_glfw";
	io.BackendRendererName = "imgui_impl_opengl3";


	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = ml::WindowKey::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = ml::WindowKey::Left;
	io.KeyMap[ImGuiKey_RightArrow] = ml::WindowKey::Right;
	io.KeyMap[ImGuiKey_UpArrow] = ml::WindowKey::Up;
	io.KeyMap[ImGuiKey_DownArrow] = ml::WindowKey::Down;
	io.KeyMap[ImGuiKey_PageUp] = ml::WindowKey::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = ml::WindowKey::PageDown;
	io.KeyMap[ImGuiKey_Home] = ml::WindowKey::Home;
	io.KeyMap[ImGuiKey_End] = ml::WindowKey::End;
	io.KeyMap[ImGuiKey_Insert] = ml::WindowKey::Insert;
	io.KeyMap[ImGuiKey_Delete] = ml::WindowKey::Delete;
	io.KeyMap[ImGuiKey_Backspace] = ml::WindowKey::Backspace;
	io.KeyMap[ImGuiKey_Space] = ml::WindowKey::Space;
	io.KeyMap[ImGuiKey_Enter] = ml::WindowKey::Enter;
	io.KeyMap[ImGuiKey_Escape] = ml::WindowKey::Escape;
	io.KeyMap[ImGuiKey_A] = ml::WindowKey::A;
	io.KeyMap[ImGuiKey_C] = ml::WindowKey::C;
	io.KeyMap[ImGuiKey_V] = ml::WindowKey::V;
	io.KeyMap[ImGuiKey_X] = ml::WindowKey::X;
	io.KeyMap[ImGuiKey_Y] = ml::WindowKey::Y;
	io.KeyMap[ImGuiKey_Z] = ml::WindowKey::Z;

	io.SetClipboardTextFn = ImGui_ML_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ML_GetClipboardText;
	io.ClipboardUserData = g_Window;

	g_MouseCursors[ImGuiMouseCursor_Arrow]		= g_Window->createCursor(ml::Cursor::Arrow);
	g_MouseCursors[ImGuiMouseCursor_TextInput]	= g_Window->createCursor(ml::Cursor::IBeam);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll]	= g_Window->createCursor(ml::Cursor::Arrow);   // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNS]	= g_Window->createCursor(ml::Cursor::VResize);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW]	= g_Window->createCursor(ml::Cursor::HResize);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = g_Window->createCursor(ml::Cursor::Arrow);  // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = g_Window->createCursor(ml::Cursor::Arrow);  // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_Hand]		= g_Window->createCursor(ml::Cursor::Hand);

	if (install_callbacks)
	{
		window->setMouseButtonCallback(ImGui_ML_MouseButtonCallback);
		window->setScrollCallback(ImGui_ML_ScrollCallback);
		window->setKeyCallback(ImGui_ML_KeyCallback);
		window->setCharCallback(ImGui_ML_CharCallback);
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
		io.MouseDown[i] = g_MouseJustPressed[i] || g_Window->getMouseButton(i) != 0;
		g_MouseJustPressed[i] = false;
	}

	// Update mouse position
	const ImVec2 mouse_pos_backup = io.MousePos;
	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
#ifdef __EMSCRIPTEN__
	const bool focused = true; // Emscripten
#else
	const bool focused = g_Window->isFocused();
#endif
	if (focused)
	{
		if (io.WantSetMousePos)
		{
			g_Window->setCursorPos({ (int32_t)mouse_pos_backup.x, (int32_t)mouse_pos_backup.y });
		}
		else
		{
			ml::vec2f mousePos = g_Window->getCursorPos();
			io.MousePos = ImVec2(mousePos[0], mousePos[1]);
		}
	}
}

static void ImGui_ML_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || g_Window->getInputMode() == ml::Cursor::Disabled)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		g_Window->setInputMode(ml::Cursor::Hidden);
	}
	else
	{
		// Show OS mouse cursor
		// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with ML 3.2, but 3.3 works here.
		g_Window->setCursor(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
		g_Window->setInputMode(ml::Cursor::Normal);
	}
}

static bool ImGui_ML_CompileShader(uint32_t & program, const char * const * vs, const char * const * fs)
{
	if (!ml::OpenGL::shadersAvailable())
	{
		ml::cerr << ("Shaders are not available on your system.") << ml::endl;
		return false;
	}

	if (!program && !(program = ml::OpenGL::createProgramObject()))
	{
		ml::cerr << ("Failed creating shader object") << ml::endl;
		return false;
	}

	// Create the vertex shader if needed
	if (vs)
	{
		// Create and Compile the shader
		g_VertHandle = ml::OpenGL::createShaderObject(ml::GL::VertexShader);
		ml::OpenGL::shaderSource(g_VertHandle, 2, vs, NULL);
		ml::OpenGL::compileShader(g_VertHandle);

		// Check the Compile log
		if (!ml::OpenGL::getProgramParameter(g_VertHandle, ml::GL::ObjectCompileStatus))
		{
			const char * log = ml::OpenGL::getProgramInfoLog(g_VertHandle);
			ml::OpenGL::deleteShader(g_VertHandle);
			ml::OpenGL::deleteShader(program);
			ml::cerr << ("Failed to compile vertex source: ") << log << ml::endl;
			return false;
		}

		// Attach the shader to the program, and delete it
		ml::OpenGL::attachShader(program, g_VertHandle);
	}

	// Create the fragment shader if needed
	if (fs)
	{
		// Create and Compile the shader
		g_FragHandle = ml::OpenGL::createShaderObject(ml::GL::FragmentShader);
		ml::OpenGL::shaderSource(g_FragHandle, 2, fs, NULL);
		ml::OpenGL::compileShader(g_FragHandle);

		// Check the Compile log
		if (!ml::OpenGL::getProgramParameter(g_FragHandle, ml::GL::ObjectCompileStatus))
		{
			const char * log = ml::OpenGL::getProgramInfoLog(g_FragHandle);
			ml::OpenGL::deleteShader(g_FragHandle);
			ml::OpenGL::deleteShader(program);
			ml::cerr << ("Failed to compile fragment source: ") << log << ml::endl;
			return false;
		}

		// Attach the shader to the program, and delete it
		ml::OpenGL::attachShader(program, g_FragHandle);
	}

	// Link the program
	ml::OpenGL::linkShader(program);

	// Check the link log
	if (!ml::OpenGL::getProgramParameter(program, ml::GL::ObjectLinkStatus))
	{
		const char * log = ml::OpenGL::getProgramInfoLog(program);
		ml::OpenGL::deleteShader(program);
		ml::cerr << ("Failed to link source: ") << log << ml::endl;
		return false;
	}

	ml::OpenGL::flush();

	return true;
}

/* * * * * * * * * * * * * * * * * * * * */

bool ImGui_ML_Init(const char * glsl_version)
{
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

bool ImGui_ML_InitForOpenGL(ml::Window * window, bool install_callbacks)
{
	return ImGui_ML_Init(window, install_callbacks, API_OpenGL);
}

void ImGui_ML_Shutdown()
{
	ImGui_ML_DestroyDeviceObjects();

	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	{
		g_Window->destroyCursor(g_MouseCursors[cursor_n]);
		g_MouseCursors[cursor_n] = NULL;
	}
	g_ClientApi = API_Unknown;
}

void ImGui_ML_NewFrame()
{
	if (!g_FontTexture)
		ImGui_ML_CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	// Setup display size (every frame to accommodate for window resizing)
	ml::vec2f size = ml::vec2f(g_Window->size());
	ml::vec2f display = ml::vec2f(g_Window->getFramebufferSize());
	io.DisplaySize = ImVec2(size[0], size[1]);
	io.DisplayFramebufferScale = ImVec2(size[0] > 0 ? (display[0] / size[0]) : 0, size[1] > 0 ? (display[1] / size[1]) : 0);

	// Setup time step
	double current_time = g_Window->getTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	ImGui_ML_UpdateMousePosAndButtons();
	ImGui_ML_UpdateMouseCursor();

#ifdef ML_MAP_GAMEPAD
	// Gamepad navigation mapping [BETA]
	memset(io.NavInputs, 0, sizeof(io.NavInputs));
	if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
	{
		// Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { if (buttons_count > BUTTON_NO && buttons[BUTTON_NO] == ML_PRESS) io.NavInputs[NAV_NO] = 1.0f; }
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
#endif // ML_MAP_GAMEPAD

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
	uint32_t last_active_texture = ml::OpenGL::getInt(ml::GL::ActiveTexture);
	ml::OpenGL::activeTexture(ml::GL::Texture0);
	int32_t last_program = ml::OpenGL::getInt(ml::GL::CurrentProgram);
	int32_t last_texture = ml::OpenGL::getInt(ml::GL::TextureBinding2D);
	int32_t last_sampler = ml::OpenGL::getInt(ml::GL::SamplerBinding);
	int32_t last_array_buffer = ml::OpenGL::getInt(ml::GL::ArrayBufferBinding);
	int32_t last_vertex_array = ml::OpenGL::getInt(ml::GL::VertexArrayBinding);

	int32_t last_polygon_mode[2]; 
	ml::OpenGL::getIntv(ml::GL::PolygonMode, last_polygon_mode);

	int32_t last_viewport[4]; 
	ml::OpenGL::getIntv(ml::GL::Viewport, last_viewport);

	int32_t last_scissor_box[4]; 
	ml::OpenGL::getIntv(ml::GL::ScissorBox, last_scissor_box);

	uint32_t last_blend_src_rgb = ml::OpenGL::getInt(ml::GL::BlendSourceRGB);
	uint32_t last_blend_dst_rgb = ml::OpenGL::getInt(ml::GL::BlendDestRGB);
	uint32_t last_blend_src_alpha = ml::OpenGL::getInt(ml::GL::BlendSourceAlpha);
	uint32_t last_blend_dst_alpha = ml::OpenGL::getInt(ml::GL::BlendDestAlpha);
	uint32_t last_blend_equation_rgb = ml::OpenGL::getInt(ml::GL::BlendEquationRGB);
	uint32_t last_blend_equation_alpha = ml::OpenGL::getInt(ml::GL::BlendEquationAlpha);

	bool last_enable_blend = ml::OpenGL::isEnabled(ml::GL::Blend);
	bool last_enable_cull_face = ml::OpenGL::isEnabled(ml::GL::CullFace);
	bool last_enable_depth_test = ml::OpenGL::isEnabled(ml::GL::DepthTest);
	bool last_enable_scissor_test = ml::OpenGL::isEnabled(ml::GL::ScissorTest);
	bool clip_origin_lower_left = true;

	uint32_t last_clip_origin = ml::OpenGL::getInt(ml::GL::ClipOrigin); // Support for GL 4.5's glClipControl(ml::GL::UpperLeft)
	if (last_clip_origin == ml::GL::UpperLeft)
		clip_origin_lower_left = false;

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	ml::OpenGL::enable(ml::GL::Blend);
	ml::OpenGL::blendEquation(ml::GL::FuncAdd);
	ml::OpenGL::blendFunc(ml::GL::SourceAlpha, ml::GL::OneMinusSourceAlpha);
	ml::OpenGL::disable(ml::GL::CullFace);
	ml::OpenGL::disable(ml::GL::DepthTest);
	ml::OpenGL::enable(ml::GL::ScissorTest);
	ml::OpenGL::polygonMode(ml::GL::FrontAndBack, ml::GL::Fill);

	// Setup viewport, orthographic projection matrix
	// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
	ml::OpenGL::viewport(0, 0, (int32_t)fb_width, (int32_t)fb_height);
	float L = draw_data->DisplayPos.x;
	float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
	float T = draw_data->DisplayPos.y;
	float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

	const float m00 = 2.0f / (R - L);
	const float m05 = 2.0f / (T - B);
	const float m12 = (R + L) / (L - R);
	const float m13 = (T + B) / (B - T);
	const float ortho_projection[4][4] =
	{
		{	m00,	0.0f,	0.0f,	0.0f },
		{	0.0f,	m05,	0.0f,	0.0f },
		{	0.0f,	0.0f,	-1.0f,	0.0f },
		{	m12,	m13,	0.0f,   1.0f },
	};
	ml::OpenGL::useShader(g_ShaderHandle);
	ml::OpenGL::uniform1i(g_AttribLocationTex, 0);
	ml::OpenGL::uniformMatrix4fv(g_AttribLocationProjMtx, 1, false, &ortho_projection[0][0]);
	ml::OpenGL::bindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
	// Recreate the VAO every time
	// (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
	
	uint32_t vao_handle = ml::OpenGL::genVertexArrays(1);
	ml::OpenGL::bindVertexArray(vao_handle);
	ml::OpenGL::bindBuffer(ml::GL::ArrayBuffer, g_VboHandle);
	ml::OpenGL::enableVertexAttribArray(g_AttribLocationPosition);
	ml::OpenGL::enableVertexAttribArray(g_AttribLocationUV);
	ml::OpenGL::enableVertexAttribArray(g_AttribLocationColor);
	ml::OpenGL::vertexAttribPointer(g_AttribLocationPosition, 2, ml::GL::Float, false, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, pos));
	ml::OpenGL::vertexAttribPointer(g_AttribLocationUV, 2, ml::GL::Float, false, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, uv));
	ml::OpenGL::vertexAttribPointer(g_AttribLocationColor, 4, ml::GL::UnsignedByte, true, sizeof(ImDrawVert), (void*)IM_OFFSETOF(ImDrawVert, col));

	// Draw
	ImVec2 pos = draw_data->DisplayPos;
	for (int32_t n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		ml::OpenGL::bindBuffer(ml::GL::ArrayBuffer, g_VboHandle);
		ml::OpenGL::bufferData(ml::GL::ArrayBuffer, (int32_t)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const void*)cmd_list->VtxBuffer.Data, ml::GL::StreamDraw);

		ml::OpenGL::bindBuffer(ml::GL::ElementArrayBuffer, g_ElementsHandle);
		ml::OpenGL::bufferData(ml::GL::ElementArrayBuffer, (int32_t)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const void*)cmd_list->IdxBuffer.Data, ml::GL::StreamDraw);

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
						ml::OpenGL::scissor((int32_t)clip_rect.x, (int32_t)(fb_height - clip_rect.w), (int32_t)(clip_rect.z - clip_rect.x), (int32_t)(clip_rect.w - clip_rect.y));
					else
						ml::OpenGL::scissor((int32_t)clip_rect.x, (int32_t)clip_rect.y, (int32_t)clip_rect.z, (int32_t)clip_rect.w); // Support for GL 4.5's glClipControl(ml::GL::UpperLeft)

					// Bind texture, Draw
					ml::OpenGL::bindTexture(ml::GL::Texture2D, (uint32_t)(intptr_t)pcmd->TextureId);
					ml::OpenGL::drawElements(
						ml::GL::Triangles,
						(int32_t)pcmd->ElemCount,
						sizeof(ImDrawIdx) == 2 ? ml::GL::UnsignedShort : ml::GL::UnsignedInt,
						idx_buffer_offset);
				}
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}
	ml::OpenGL::deleteVertexArrays(1, &vao_handle);

	// Restore modified GL state
	ml::OpenGL::useShader(last_program);
	ml::OpenGL::bindTexture(ml::GL::Texture2D, last_texture);
	ml::OpenGL::bindSampler(0, last_sampler);
	ml::OpenGL::activeTexture(last_active_texture);
	ml::OpenGL::bindVertexArray(last_vertex_array);
	ml::OpenGL::bindBuffer(ml::GL::ArrayBuffer, last_array_buffer);
	ml::OpenGL::blendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	ml::OpenGL::blendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
	
	if (last_enable_blend) ml::OpenGL::enable(ml::GL::Blend); else ml::OpenGL::disable(ml::GL::Blend);
	if (last_enable_cull_face) ml::OpenGL::enable(ml::GL::CullFace); else ml::OpenGL::disable(ml::GL::CullFace);
	if (last_enable_depth_test) ml::OpenGL::enable(ml::GL::DepthTest); else ml::OpenGL::disable(ml::GL::DepthTest);
	if (last_enable_scissor_test) ml::OpenGL::enable(ml::GL::ScissorTest); else ml::OpenGL::disable(ml::GL::ScissorTest);
	
	ml::OpenGL::polygonMode(ml::GL::FrontAndBack, (uint32_t)last_polygon_mode[0]);
	ml::OpenGL::viewport(last_viewport[0], last_viewport[1], (int32_t)last_viewport[2], (int32_t)last_viewport[3]);
	ml::OpenGL::scissor(last_scissor_box[0], last_scissor_box[1], (int32_t)last_scissor_box[2], (int32_t)last_scissor_box[3]);
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
	int32_t last_texture = ml::OpenGL::getInt(ml::GL::TextureBinding2D);
	g_FontTexture = ml::OpenGL::genTextures(1);
	ml::OpenGL::bindTexture(ml::GL::Texture2D, g_FontTexture);
	ml::OpenGL::texParameter(ml::GL::Texture2D, ml::GL::TexMinFilter, ml::GL::Linear);
	ml::OpenGL::texParameter(ml::GL::Texture2D, ml::GL::TexMagFilter, ml::GL::Linear);
	ml::OpenGL::pixelStore(ml::GL::UnpackRowLength, 0);
	ml::OpenGL::texImage2D(ml::GL::Texture2D, 0, ml::GL::RGBA, width, height, 0, ml::GL::RGBA, ml::GL::UnsignedByte, pixels);

	// Store our identifier
	io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

	// Restore state
	ml::OpenGL::bindTexture(ml::GL::Texture2D, last_texture);

	return true;
}

void ImGui_ML_DestroyFontsTexture()
{
	if (g_FontTexture)
	{
		ImGuiIO& io = ImGui::GetIO();
		ml::OpenGL::deleteTextures(1, &g_FontTexture);
		io.Fonts->TexID = 0;
		g_FontTexture = 0;
	}
}

bool ImGui_ML_CreateDeviceObjects()
{
	// Backup GL state
	int32_t last_texture = ml::OpenGL::getInt(ml::GL::TextureBinding2D);
	int32_t last_array_buffer = ml::OpenGL::getInt(ml::GL::ArrayBufferBinding);
	int32_t last_vertex_array = ml::OpenGL::getInt(ml::GL::VertexArrayBinding);

	// Parse GLSL version string
	int32_t glsl_version = 130;
	sscanf(g_GlslVersionString, "#version %d", &glsl_version);

	const char * vertex_shader_glsl_120 =
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

	const char * vertex_shader_glsl_130 =
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

	const char * vertex_shader_glsl_300_es =
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

	const char * vertex_shader_glsl_410_core =
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

	const char * fragment_shader_glsl_120 =
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

	const char * fragment_shader_glsl_130 =
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	const char * fragment_shader_glsl_300_es =
		"precision mediump float;\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"layout (location = 0) out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	const char * fragment_shader_glsl_410_core =
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"uniform sampler2D Texture;\n"
		"layout (location = 0) out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	// Select shaders matching our GLSL versions
	const char * vertex_shader = NULL;
	const char * fragment_shader = NULL;
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
	const char * vertex_shader_with_version[2] = { g_GlslVersionString, vertex_shader };
	const char * fragment_shader_with_version[2] = { g_GlslVersionString, fragment_shader };

	ImGui_ML_CompileShader(g_ShaderHandle, vertex_shader_with_version, fragment_shader_with_version);

	g_AttribLocationTex		= ml::OpenGL::getUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = ml::OpenGL::getUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition= ml::OpenGL::getAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV		= ml::OpenGL::getAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor	= ml::OpenGL::getAttribLocation(g_ShaderHandle, "Color");

	// Create buffers
	g_VboHandle = ml::OpenGL::genBuffers(1);
	g_ElementsHandle = ml::OpenGL::genBuffers(1);

	ImGui_ML_CreateFontsTexture();

	// Restore modified GL state
	ml::OpenGL::bindTexture(ml::GL::Texture2D, last_texture);
	ml::OpenGL::bindBuffer(ml::GL::ArrayBuffer, last_array_buffer);
	ml::OpenGL::bindVertexArray(last_vertex_array);

	return true;
}

void ImGui_ML_DestroyDeviceObjects()
{
	if (g_VboHandle) ml::OpenGL::deleteBuffers(1, &g_VboHandle);
	if (g_ElementsHandle) ml::OpenGL::deleteBuffers(1, &g_ElementsHandle);
	g_VboHandle = g_ElementsHandle = 0;

	if (g_ShaderHandle && g_VertHandle) ml::OpenGL::detachShader(g_ShaderHandle, g_VertHandle);
	if (g_VertHandle) ml::OpenGL::deleteShader(g_VertHandle);
	g_VertHandle = 0;

	if (g_ShaderHandle && g_FragHandle) 
		ml::OpenGL::detachShader(g_ShaderHandle, g_FragHandle);

	if (g_FragHandle) ml::OpenGL::deleteShader(g_FragHandle);
	g_FragHandle = 0;

	if (g_ShaderHandle) ml::OpenGL::deleteShader(g_ShaderHandle);
	g_ShaderHandle = 0;

	ImGui_ML_DestroyFontsTexture();
}

/* * * * * * * * * * * * * * * * * * * * */

void ImGui_ML_MouseButtonCallback(void * window, int32_t button, int32_t action, int32_t mods)
{
	if (action == ML_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
	{
		g_MouseJustPressed[button] = true;
	}

	ML_EventSystem.fireEvent(ml::MouseButtonEvent(button, action, mods));
}

void ImGui_ML_ScrollCallback(void * window, double xoffset, double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;

	ML_EventSystem.fireEvent(ml::ScrollEvent(xoffset, yoffset));
}

void ImGui_ML_KeyCallback(void * window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	ImGuiIO& io = ImGui::GetIO();

	if (action == ML_PRESS)
	{
		io.KeysDown[key] = true;
	}

	if (action == ML_RELEASE)
	{
		io.KeysDown[key] = false;
	}

	// Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[ml::WindowKey::LeftControl] || io.KeysDown[ml::WindowKey::RightControl];
	io.KeyShift = io.KeysDown[ml::WindowKey::LeftShift] || io.KeysDown[ml::WindowKey::RightShift];
	io.KeyAlt = io.KeysDown[ml::WindowKey::LeftAlt] || io.KeysDown[ml::WindowKey::RightAlt];
	io.KeySuper = io.KeysDown[ml::WindowKey::LeftSuper] || io.KeysDown[ml::WindowKey::RightSuper];

	ML_EventSystem.fireEvent(ml::KeyEvent(key, scancode, action, mods));
}

void ImGui_ML_CharCallback(void * window, uint32_t c)
{
	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
	{
		io.AddInputCharacter((uint16_t)c);
	}

	ML_EventSystem.fireEvent(ml::CharEvent(c));
}

/* * * * * * * * * * * * * * * * * * * * */