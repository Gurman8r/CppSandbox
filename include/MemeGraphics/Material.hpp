#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <MemeGraphics/Shader.hpp>

namespace ml
{
	class ML_GRAPHICS_API Material final
		: public ITrackable
		, public IReadable
	{
	public:
		Material();
		~Material();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	private:
		const Texture * m_highlightMap;
		const Texture * m_normalMap;

	};
}

#endif // !_MATERIAL_HPP_
