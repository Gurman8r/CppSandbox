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

			graph.update(
				"##Framerate",
				(float)ML_Time.calculateFPS((float)ImGui::GetIO().DeltaTime),
				"fps {0}"
			);

			/* * * * * * * * * * * * * * * * * * * * */
		}
		return endDraw();
	}

	void Profiler::GraphLines::update(CString label, const float sample, const String & fmt)
	{
		ImGui::Checkbox("Animate", &animate);

		if (!animate || refresh == 0.0f)
		{
			refresh = (float)ImGui::GetTime();
		}

		while (refresh < ImGui::GetTime())
		{
			values[offset] = sample;
			offset = (offset + 1) % IM_ARRAYSIZE(values);
			refresh += (1.0f / 60.0f);
		}

		ImGui::PlotLines(
			label,
			values,
			IM_ARRAYSIZE(values),
			offset,
			fmt.format(sample).c_str(),
			min,
			max,
			{ size[0], size[1] });
	}

	/* * * * * * * * * * * * * * * * * * * * */
}