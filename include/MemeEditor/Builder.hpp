#ifndef _ML_BUILDER_HPP_
#define _ML_BUILDER_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/Document.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_TEST_SHADER "built_shader"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Shader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Builder final
		: public GUI_Window
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		struct BuildFile : public ITrackable, public INonCopyable
		{
			enum : size_t { MaxSize = 2048 };

			using Source = typename char[MaxSize];

			String	name;
			Source	data;

			BuildFile(const String & name, const String & data)
				: name(name)
				, data()
			{
				std::strcpy(this->data, data.c_str());
			}
		};

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Builder();
		~Builder();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		inline BuildFile * createFile(const String & name, const String & data)
		{
			return ((m_data.find(name) == m_data.end())
				? (m_data.insert({ name, new BuildFile(name, data) }).first->second)
				: (NULL)
			);
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Map<String, BuildFile *> m_data;
		Shader * m_shader;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUILDER_HPP_