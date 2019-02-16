#ifndef _MANIFEST_HPP_
#define _MANIFEST_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	class ML_EDITOR_API Manifest final
		: public ITrackable
	{
	public:
		Manifest();
		~Manifest();

	private:

	};
}
#endif // !_MANIFEST_HPP_
