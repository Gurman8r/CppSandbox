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
		using FileMap	= HashMap<String, String>;
		using FilePair	= Pair<String, String>;
		using DataMap	= HashMap<String, FileMap>;
		using DataPair  = Pair<String, FileMap>;

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
		inline const DataMap &	getDataMap()const { return m_data; }

	private:
		String	m_path;
		DataMap	m_data;
	};
}
#endif // !_MANIFEST_HPP_