#ifndef _VIDEO_MODE_HPP_
#define _VIDEO_MODE_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/Vector2.hpp>

namespace ml
{
	struct ML_WINDOW_API VideoMode final
		: public ITrackable
		, public IComparable<VideoMode>
	{
		VideoMode();
		VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel = 32);
		VideoMode(const vec2u & size, uint32_t bitsPerPixel = 32);
		VideoMode(const VideoMode & copy);
		~VideoMode();

		static const VideoMode & desktop();
		static const std::vector<VideoMode> & resolutions();

		bool isValidFullscreenMode() const;

		vec2u	 size;
		uint32_t bitsPerPixel;

		inline const uint32_t & width()  const { return size.front(); }
		inline const uint32_t & height() const { return size.back(); }

	public:
		void serialize(std::ostream & out) const override;
		bool equals(const VideoMode & value) const override;
		bool lessThan(const VideoMode & value) const override;

	};
}

#endif // !_VIDEO_MODE_HPP_
