#include <MemeEditor/SceneView.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SceneView::SceneView()
		: base_type("Scene View")
	{
	}

	SceneView::~SceneView()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	void SceneView::onEvent(const IEvent * value)
	{
	}

	bool SceneView::draw(bool * p_open)
	{
		return beginDraw(p_open, ImGuiWindowFlags_None);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneView::beginDraw(bool * p_open, int32_t flags)
	{
		if (base_type::beginDraw(p_open, flags))
		{
			ImGui::BeginChild("Viewport", { -1, -1 });
		}
		return good();
	}

	bool SceneView::endDraw()
	{
		if (good())
		{
			ImGui::EndChild();
		}
		return base_type::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SceneView::updateScene(const Texture * texture)
	{
		return updateScene(std::remove_cv_t<Texture *>(texture));
	}

	bool SceneView::updateScene(Texture * texture)
	{
		if (good() && (texture && (*texture)))
		{
			// Texture Size
			const ml::vec2f src = texture->size();

			// Window Size
			const ml::vec2f dst = ml::vec2f {
				ImGui::GetWindowSize().x,
				ImGui::GetWindowSize().y
			};

			auto scaleToFit = [](const ml::vec2f & src, const ml::vec2f & dst)
			{
				const ml::vec2f
					hs = (dst[0] / src[0]),
					vs = (dst[1] / src[1]);
				return (src * (((hs) < (vs)) ? (hs) : (vs)));
			};

			const ml::vec2f scl = scaleToFit(src, dst);
			const ml::vec2f pos = ((dst - scl) * 0.5f);

			ImGui::SetCursorPos({ pos[0], pos[1] });

			ImGui::Image(texture->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}