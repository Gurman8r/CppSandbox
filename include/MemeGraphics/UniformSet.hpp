#ifndef _ML_UNIFORM_SET_HPP_
#define _ML_UNIFORM_SET_HPP_

#include <MemeCore/List.hpp>
#include <MemeGraphics/Uniform.hpp>

namespace ml
{
	class ML_GRAPHICS_API UniformSet
		: public ITrackable
	{
	public:
		using map_type			= HashMap<String, Uniform>;
		using iterator			= typename map_type::iterator;
		using const_iterator	= typename map_type::const_iterator;

	public:
		UniformSet();
		UniformSet(const map_type & value);
		UniformSet(const List<Uniform> & value);
		UniformSet(const std::initializer_list<Uniform> & value);
		UniformSet(const UniformSet & copy);
		~UniformSet();

	public:
		void clear();
		bool empty() const;
		bool insert(const Uniform & value);
		Uniform * find(const String & name);
		const Uniform * find(const String & name) const;
		size_t size() const;

	public:
		inline const Uniform & operator[](const String & name) const { return m_map.at(name); }
		inline Uniform & operator[](const String & name) { return m_map[name]; }

	public:
		inline iterator			begin()			{ return m_map.begin(); }
		inline iterator			end()			{ return m_map.end(); }
		inline const_iterator	begin() const	{ return m_map.begin(); }
		inline const_iterator	end()	const	{ return m_map.end(); }
		inline const_iterator	cbegin()const	{ return m_map.cbegin(); }
		inline const_iterator	cend()	const	{ return m_map.cend(); }

	private:
		map_type m_map;
	};
}
#endif // !_ML_UNIFORM_SET_HPP_