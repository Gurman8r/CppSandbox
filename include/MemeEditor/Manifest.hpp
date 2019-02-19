#ifndef _MANIFEST_HPP_
#define _MANIFEST_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>

namespace ml
{
	class ML_EDITOR_API Manifest final
		: public ITrackable
		, public IReadable
	{
	public:
		using file_name	= String;
		using file_path	= String;
		using file_type	= String;

		using file_map	= HashMap<file_name, file_path>;
		using file_pair	= Pair<file_name, file_path>;

		using type_map	= HashMap<file_type, file_map>;
		using type_pair = Pair<file_type, file_map>;

	public:
		Manifest();
		Manifest(const Manifest & copy);
		~Manifest();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const String &	getPath()	const { return m_path; }
		inline const type_map &	getDataMap()const { return m_data; }

	private:
		String	 m_path;
		type_map m_data;
	};
}
#endif // !_MANIFEST_HPP_