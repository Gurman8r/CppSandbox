#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <MemeGraphics/Shader.hpp>

namespace ml
{
	class ML_GRAPHICS_API Material final
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, const UniformSet & uniforms);
		Material(const Material & copy);
		~Material();

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	private:
		const Shader *	m_shader;
		UniformSet		m_uniforms;

	};
}

#endif // !_ML_MATERIAL_HPP_