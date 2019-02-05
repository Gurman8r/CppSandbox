#ifndef _IMGUI_MAIN_HPP_
#define _IMGUI_MAIN_HPP_

#include <MemeGraphics/RenderWindow.hpp>
#include <MemeCore/Debug.hpp>
#include "Settings.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_ml.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>

/* * * * * * * * * * * * * * * * * * * * */

int32_t gui_main(int32_t argc, char ** argv)
{
	using namespace ml;

	// Setup window
	glfwSetErrorCallback([](int32_t err, const char * desc)
	{
		Debug::log("IMGUI: {0} {1}", err, desc);
	});

	const char* glsl_version = "#version 460";

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

	if (glewInit() != GLEW_OK)
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
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
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

int32_t foo_main(int32_t argc, char ** argv)
{
	using namespace ml;

	const char* glsl_version = "#version 330";

	RenderWindow m_window;
	if (ml::Debug::log("Creating Window...") && m_window.create(
		SETTINGS.title,
		ml::VideoMode({ SETTINGS.width, SETTINGS.height }, SETTINGS.bitsPerPixel),
		ml::Window::Style::Default,
		ml::Context(3, 3)
	)
	&& m_window.setup()
	)
	{
		m_window.setCursor(ml::Window::Normal);
		m_window.setPosition((ml::VideoMode::desktop().size - m_window.size()) / 2);
	}
	else
	{
		return Debug::logError("Failed Loading Window")
			|| Debug::pause(EXIT_FAILURE);
	}

	// Initialize OpenGL loader
	if (glewInit() != GLEW_OK)
	{
		return Debug::pause(EXIT_FAILURE)
			|| Debug::logError("Failed to initialize OpenGL loader");
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ML_InitForOpenGL((GLFWwindow *)(m_window.ptr()), true);
	ImGui_ML_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (m_window.isOpen())
	{
		m_window.pollEvents();

		// Start the Dear ImGui frame
		ImGui_ML_NewFrame();
		ImGui_ML_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
		{
			ImGui::ShowDemoWindow(&show_demo_window);
		}

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
		m_window.makeContextCurrent();
		m_window.setViewport(vec2i::Zero, vec2i(m_window.getFramebufferSize()));
		m_window.clear({ clear_color.x, clear_color.y, clear_color.z, clear_color.w });
		ImGui_ML_RenderDrawData(ImGui::GetDrawData());

		m_window.makeContextCurrent();
		m_window.swapBuffers();
	}

	// Cleanup
	ImGui_ML_Shutdown();
	ImGui_ML_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

#endif // !_IMGUI_MAIN_HPP_