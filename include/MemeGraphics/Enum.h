#ifndef _GRAPHICS_ENUM_H_
#define _GRAPHICS_ENUM_H_

#include <MemeGraphics/Export.h>

namespace ml
{
	class ML_GRAPHICS_API Enum
	{
	public:
		enum Usage
		{
			Stream	= 0x88E0,
			Dynamic = 0x88E8,
			Static	= 0x88E4,
		};
		enum Type
		{
			Byte = 0x1400,
			UnsignedByte,
			Short,
			UnsignedShort,
			Int,
			UnsignedInt,
			Float,
		};
		enum Flag
		{
			DepthTest		= 0x0B71,
			CullFace		= 0x0B44,
			FrameBufferSRGB = 0x8DB9,
			MultiSample		= 0x809D,
			AlphaTest		= 0x0BC0,
			Blend			= 0x0BE2,
			Texture2D		= 0x0DE1,
		};
		enum Primitive
		{
			Points = 0x000,
			Lines,
			LineLoop,
			LineStrip,
			Triangles,
			TriangleStrip,
			TriangleFan,
		};
		enum Func
		{
			Never = 0x200,
			Less,
			Equal,
			LEqual,
			Greater,
			NotEqual,
			GEqual,
			Always,
		};
		enum BlendFunc
		{
			SourceColor = 0x300,
			OneMinusSourceColor,
			SourceAlpha,
			OneMinusSourceAlpha,
			DestAlpha,
			OneMinusDestAlpha,
			DestColor,
			OneMinusDestColor,
			SourceAlphaSaturate,
		};
		enum Face
		{
			FrontLeft = 0x0400,
			FrontRight,
			BackLeft,
			BackRight,
			Front,
			Back,
			Left,
			Right,
			FrontAndBack,
		};
		enum Format
		{
			Red = 0x1903,
			Green,
			Blue,
			Alpha,
			RGB,
			RGBA,
			Luminance,
			LuminanceAlpha,			
		};
	};
}

#endif // !_GRAPHICS_ENUM_H_
