/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include <MemeCore/EventSystem.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_ml.hpp>
#include <GLFW/glfw3.h>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

int32_t gui_main(int32_t argc, char ** argv);

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Settings
	if (!SETTINGS.loadFromFile(CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	return gui_main(argc, argv);

	// Create Demo
	DEMO::Demo demo;

	// Enter
	ML_EventSystem.fireEvent(DEMO::EnterEvent(argc, argv));
	if (demo.checkStatus() == ml::Debug::Error)
	{
		return ml::Debug::logError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
	
	// Load
	ML_EventSystem.fireEvent(DEMO::LoadEvent());
	if (demo.checkStatus() == ml::Debug::Error)
	{
		return ml::Debug::logError("Failed Loading Resources")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Start
	ML_EventSystem.fireEvent(DEMO::StartEvent());

	// Loop
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	ml::NativeInput	input;
	do
	{	// Begin Step
		loopTimer.start();
		input.beginStep();
		{
			// Update
			ML_EventSystem.fireEvent(DEMO::UpdateEvent(elapsed, input));

			// Draw
			ML_EventSystem.fireEvent(DEMO::DrawEvent(elapsed));
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();

	} while (demo.isOpen());

	// Exit
	ML_EventSystem.fireEvent(DEMO::ExitEvent());

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */

int32_t gui_main(int32_t argc, char ** argv)
{
	using namespace ml;

	// Setup window
	glfwSetErrorCallback([](int32_t err, const char * desc)
	{
		Debug::log("IMGUI: {0} {1}", err, desc);
	});

	const char* glsl_version = "#version 410";

	GLFWwindow * window;
	if (glfwInit() == GLFW_TRUE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		if (window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL))
		{
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);
		}
		else
		{
			return Debug::logError("Failed creating window")
				|| Debug::pause(EXIT_FAILURE);
		}
	}
	else
	{
		return Debug::logError("Failed initializing GLFW")
			|| Debug::pause(EXIT_FAILURE);
	}

	OpenGL::errorPause(true);
	if (!OpenGL::init(false))
	{
		return Debug::logError("Failed initializing GLEW")
			|| Debug::pause(EXIT_FAILURE);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ML_InitForOpenGL(window, true);
	ImGui_ML_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ML_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);

			if (ImGui::Button("Button"))
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		OpenGL::viewport(0, 0, display_w, display_h);
		OpenGL::clearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		OpenGL::clear(GL::ColorBufferBit);
		ImGui_ML_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ML_Shutdown();
	ImGui_ML_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}