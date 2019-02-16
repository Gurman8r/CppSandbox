#ifndef _MANIFEST_HPP_
#define _MANIFEST_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	class ML_EDITOR_API Manifest final
		: public ITrackable
		, public IResource
	{
	public:
		using FileMap = HashMap<String, String>;
		using TypeMap = HashMap<String, FileMap>;

	public:
		Manifest();
		~Manifest();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		const TypeMap & getTypes() const;
		const FileMap & getFiles(const String & type) const;

	public:
		inline const String & path() const { return m_path; }

	private:
		TypeMap	m_data;
		String	m_path;
	};
}
#endif // !_MANIFEST_HPP_
