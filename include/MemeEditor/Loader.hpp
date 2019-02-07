#ifndef _LOADER_HPP_
#define _LOADER_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

#define assert_typeof_resource(T) \
static_assert(std::is_base_of<ml::IResource, T>::value, "Type must derive ml::IResource")

namespace ml
{
}

#endif // !_LOADER_HPP_
