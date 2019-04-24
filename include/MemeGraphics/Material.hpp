#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <MemeGraphics/Shader.hpp>	

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Placeholder
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

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		bool apply() const;
		
	public:
		inline const Shader		* shader() const	{ return m_shader; }
		inline const UniformSet & uniforms() const	{ return m_uniforms; }

	public:
		inline const Shader	*	& shader()			{ return m_shader; }
		inline UniformSet		& uniforms()		{ return m_uniforms; }

	private:
		const Shader *	m_shader;
		UniformSet		m_uniforms;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
