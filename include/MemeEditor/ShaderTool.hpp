#ifndef _ML_SHADER_TOOL_HPP_
#define _ML_SHADER_TOOL_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/Document.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_ShaderTool ml::ShaderTool::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ShaderTool
		: public GUI_Window
		, public ISingleton<ShaderTool>
	{
		friend class ISingleton<ShaderTool>;

	public:
		struct Item : public ITrackable
		{
			enum Type : int32_t { None = -1, Vertex, Geometry, Fragment, MAX_TYPE };

			CString		name;
			Type		type;
			Document	doc;
			UniformSet	uni;

			Item(CString name)
				: name(name)
				, type(None)
				, doc(name)
				, uni()
			{
			}
			Item(CString name, Type type)
				: name(name)
				, type(type)
				, doc(name)
				, uni()
			{
			}
			Item(CString name, Type type, const UniformSet & uni)
				: name(name)
				, type(type)
				, doc(name)
				, uni(uni)
			{
			}
			Item(const Item & copy)
				: name(copy.name)
				, type(copy.type)
				, doc(copy.doc)
				, uni(copy.uni)
			{
			}
		};

	private:
		ShaderTool();
		~ShaderTool();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	private:
		void draw_menu();
		void draw_tabs();

		bool edit_document_tab(Document * value);
		bool edit_document_name(char * buf, Document * value);
		bool edit_document_data(Document * value);

		bool edit_uniforms_tab(UniformSet * value);
		bool edit_uniforms_list(UniformSet * value);
		void draw_uniform_list_buttons(UniformSet * value);

	public:
		inline Item * getItem(const size_t index)
		{
			return (!m_items.empty()
				? (index < m_items.size())
					? (&m_items.at(index))
					: (NULL)
				: (NULL));
		}

		inline Item * getSelected()
		{
			return getItem((size_t)m_curItem);
		}

	private:
		struct Funcs;

		std::vector<Item> m_items;
		int32_t m_curItem;
		int32_t m_curUniform;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_TOOL_HPP_