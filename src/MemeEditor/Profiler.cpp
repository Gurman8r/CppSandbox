#include <MemeEditor/Profiler.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeCore/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Profiler::Profiler()
		: GUI_Window("Profiler")
	{
	}

	Profiler::~Profiler()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Profiler::onEvent(const IEvent * value)
	{
	}

	bool Profiler::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (ImGui::BeginMenuBar())
			{
				ImGui::Text("%s (WIP)", title());
				ImGui::EndMenuBar();
			}

			/* * * * * * * * * * * * * * * * * * * * */

			static bool animate = true;
			ImGui::Checkbox("Animate", &animate);

			static float	values[90] = { 0 };
			static int32_t	offset = 0;
			static float	refresh = 0.0f;
			static uint64_t	total = 0;

			if (!animate || refresh == 0.0f)
			{
				refresh = (float)ImGui::GetTime();
			}
			
			while (refresh < ImGui::GetTime())
			{
				const uint64_t fps = ML_Time.calculateFPS((float)ImGui::GetIO().DeltaTime);
				total += fps;
				values[offset] = (float)fps;
				offset = (offset + 1) % IM_ARRAYSIZE(values);
				refresh += 1.0f / 60.0f;
			}

			ImGui::PlotLines(
				"Framerate", 
				values, 
				IM_ARRAYSIZE(values), 
				offset, 
				String("avg {0}").format((float)total / ImGui::GetFrameCount()).c_str(),
				1.f, 
				128.f,
				{ 0, 80 });

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}