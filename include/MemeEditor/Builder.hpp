#ifndef _ML_BUILDER_HPP_
#define _ML_BUILDER_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/Document.hpp>
#include <MemeGraphics/Material.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_TEST_SHADER "built_shader"

namespace ml
{
	struct Uniform
	{
		String name;
		int32_t type;
		const void * data;

		inline friend bool operator==(const Uniform & a, const Uniform & b)
		{
			return a.name == b.name && a.name == b.name;
		}

		inline friend bool operator<(const Uniform & a, const Uniform & b)
		{
			return a.name < b.name && a.name < b.name;
		}

		inline friend std::ostream & operator<<(std::ostream & out, const Uniform & u)
		{
			return out;
		}
	};

	// Shader Builder/Editor
	class ML_EDITOR_API Builder final
		: public GUI_Window
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	public:
		using SourceBuffer	= typename char[2048];
		using UniformList	= List<Uniform>;

		struct BuilderData : public ITrackable
		{
			String			name;	// Name
			SourceBuffer	src;	// Source Text
			UniformList		uni;	// Uniforms

			BuilderData();
			BuilderData(const String & name);
			BuilderData(const String & name, const String & src);
			BuilderData(const String & name, const String & src, const UniformList & uni);
			BuilderData(const BuilderData & copy);

			inline operator bool() const { return (bool)(name); }
		};

		using BuilderDataMap = HashMap<String, BuilderData>;


	private:
		Builder();
		~Builder();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		void draw_shader_tab(const String & value);
		void draw_uniform_list(UniformList * value);
		void draw_shader_text(char * value);

		void draw_tools();


	private:
		inline BuilderData * set_data(const BuilderData & data)
		{
			return ((data)
				? (&(m_data[data.name] = data))
				: (NULL));
		}

		inline BuilderData * get_data(const String & value)
		{
			static BuilderDataMap::iterator it;
			return (((it = m_data.find(value)) != m_data.end())
				? (&it->second)
				: (NULL));
		}

		inline char * get_source(const String & label)
		{
			static BuilderData * temp;
			return ((temp = get_data(label))
				? (temp->src)
				: (NULL));
		}

		inline UniformList * get_list(const String & label)
		{
			static BuilderData * temp;
			return ((temp = get_data(label))
				? (&temp->uni)
				: (NULL));
		}

		inline Uniform * get_uniform(const String & value, const size_t index)
		{
			UniformList * list;
			return ((list = get_list(value))
				? (((list) && (!list->empty()))
					? (index < list->size())
						? (&list->at(index))
						: (NULL)
					: (NULL))
				: (NULL));
		}

		inline Uniform * get_selected_uniform(const String & label)
		{
			return get_uniform(label, m_selected);
		}

	private:
		size_t m_selected;

		BuilderDataMap m_data;

		Shader * m_shader;
	};
}

#endif // !_ML_BUILDER_HPP_