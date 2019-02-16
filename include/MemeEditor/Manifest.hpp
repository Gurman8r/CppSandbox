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
		using FileMap = HashMap<String, HashMap<String, String>>;

	public:
		Manifest();
		~Manifest();

		bool cleanup() override;
		bool readFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const String &	getPath()	const { return m_path; }
		inline const FileMap &	getFiles()	const { return m_files; }

	private:
		String	m_path;
		FileMap	m_files;
	};
}
#endif // !_MANIFEST_HPP_
