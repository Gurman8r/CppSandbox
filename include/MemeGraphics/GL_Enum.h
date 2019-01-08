#ifndef _GRAPHICS_ENUM_H_
#define _GRAPHICS_ENUM_H_

#include <MemeGraphics/Export.h>
#include <inttypes.h>

namespace ml
{
	class ML_GRAPHICS_API Enum final
	{
	public:
		using Base = uint32_t;

	public:
		enum ContextBit : Base
		{
			ContextCoreProfileBit	= 0x1,	// GL_CONTEXT_CORE_PROFILE_BIT
			ContextCompatProfileBit = 0x2,	// GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
			ContextFlagDebugBit		= 0x2,	// GL_CONTEXT_FLAG_DEBUG_BIT
		};
		enum BufferBit : Base
		{
			DepthBufferBit	 = 0x100,		// GL_DEPTH_BUFFER_BIT
			StencilBufferBit = 0x400,		// GL_STENCIL_BUFFER_BIT
			ColorBufferBit	 = 0x4000,		// GL_COLOR_BUFFER_BIT
		};
		enum Flag : Base
		{
			CullFace		= 0x0B44,		// GL_CULL_FACE
			DepthTest		= 0x0B71,		// GL_DEPTH_TEST
			AlphaTest		= 0x0BC0,		// GL_ALPHA_TEST
			Blend			= 0x0BE2,		// GL_BLEND
			Texture2D		= 0x0DE1,		// GL_TEXTURE_2D
			Multisample		= 0x809D,		// GL_MULTISAMPLE
		};
		enum Usage : Base
		{
			StreamDraw		= 0x88E0,		// GL_STREAM_DRAW
			StaticDraw		= 0x88E4,		// GL_STATIC_DRAW
			DynamicDraw		= 0x88E8,		// GL_DYNAMIC_DRAW
		};
		enum Error : Base
		{
			NoError = 0,					// GL_NO_ERROR
			InvalidEnum = 0x0500,			// GL_INVALID_ENUM
			InvalidValue,					// GL_INVALID_VALUE
			InvalidOperation,				// GL_INVALID_OPERATION
			StackOverflow,					// GL_STACK_OVERFLOW
			StackUnderflow,					// GL_STACK_UNDERFLOW
			OutOfMemory,					// GL_OUT_OF_MEMORY
			InvalidFramebufferOperation,	// GL_INVALID_FRAMEBUFFER_OPERATION
		};
		enum StringID : Base
		{
			Vendor = 0x1F00,				// GL_VENDOR
			Renderer,						// GL_RENDERER
			Version,						// GL_VERSION
			Extensions,						// GL_EXTENSIONS
			ShadingLanguageVersion = 0x8B8C,// GL_SHADING_LANGUAGE_VERSION			
		};
		enum IntID : Base
		{
			MajorVersion = 0x821B,			// GL_MAJOR_VERSION
			MinorVersion,					// GL_MINOR_VERSION
			NumExtensions,					// GL_NUM_EXTENSIONS
			ContextFlags = 0x821E,			// GL_CONTEXT_FLAGS
			ContextProfileMask = 0x9126,	// GL_CONTEXT_PROFILE_MASK
		};
		enum BufferType : Base
		{
			ArrayBuffer = 0x8892,			// GL_ARRAY_BUFFER
			ElementArrayBuffer,				// GL_ELEMENT_ARRAY_BUFFER
		};
		enum ShaderType : Base
		{
			FragmentShader = 0x8B30,		// GL_FRAGMENT_SHADER
			VertexShader,					// GL_VERTEX_SHADER
			GeometryShader = 0x8DD9,		// GL_GEOMETRY_SHADER
		};
		enum Primitive : Base
		{
			Points = 0x000,					// GL_POINTS
			Lines,							// GL_LINES
			LineLoop,						// GL_LINE_LOOP
			LineStrip,						// GL_LINE_STRIP
			Triangles,						// GL_TRIANGLES
			TriangleStrip,					// GL_TRIANGLE_STRIP
			TriangleFan,					// GL_TRIANGLE_FAN
		};
		enum Equation : Base
		{
			Multiply	= 0x0103,			// GL_MULT
			Add			= 0x0104,			// GL_ADD
			Subtract	= 0x84E7,			// GL_SUBTRACT
		};
		enum Comparison : Base
		{
			Never = 0x200,					// GL_NEVER
			Less,							// GL_LESS
			Equal,							// GL_EQUAL
			LEqual,							// GL_LEQUAL
			Greater,						// GL_GREATER
			NotEqual,						// GL_NOTEQUAL
			GEqual,							// GL_GEQUAL
			Always,							// GL_ALWAYS
		};
		enum Factor : Base
		{
			SourceColor = 0x0300,			// GL_SRC_COLOR
			OneMinusSourceColor,			// GL_ONE_MINUS_SRC_COLOR
			SourceAlpha,					// GL_SRC_ALPHA
			OneMinusSourceAlpha,			// GL_ONE_MINUS_SRC_ALPHA
			DestAlpha,						// GL_DST_ALPHA
			OneMinusDestAlpha,				// GL_ONE_MINUS_DST_ALPHA
			DestColor,						// GL_DST_COLOR
			OneMinusDestColor,				// GL_ONE_MINUS_DST_COLOR
			SourceAlphaSaturate,			// GL_SRC_ALPHA_SATURATE
		};
		enum Face : Base
		{
			FrontLeft = 0x0400,				// GL_FRONT_LEFT
			FrontRight,						// GL_FRONT_RIGHT
			BackLeft,						// GL_BACK_LEFT
			BackRight,						// GL_BACK_RIGHT
			Front,							// GL_FRONT
			Back,							// GL_BACK
			Left,							// GL_LEFT
			Right,							// GL_RIGHT
			FrontAndBack,					// GL_FRONT_AND_BACK
		};
		enum Type : Base
		{
			Byte = 0x1400,					// GL_BYTE
			UnsignedByte,					// GL_UNSIGNED_BYTE
			Short,							// GL_SHORT
			UnsignedShort,					// GL_UNSIGNED_SHORT
			Int,							// GL_INT
			UnsignedInt,					// GL_UNSIGNED_INT
			Float,							// GL_FLOAT
		};
		enum ColorFormat : Base
		{
			Red = 0x1903,					// GL_RED
			Green,							// GL_GREEN
			Blue,							// GL_BLUE
			Alpha,							// GL_ALPHA
			RGB,							// GL_RGB
			RGBA,							// GL_RGBA
			Luminance,						// GL_LUMINANCE
			Luminance_Alpha,				// GL_LUMINANCE_ALPHA
		};
		enum InternalFormat : Base
		{
			SRGB = 0x8C40,					// GL_SRGB
			SRGB8,							// GL_SRGB8
			SRGB_Alpha,						// GL_SRGB_ALPHA
			SRGB8_Alpha8,					// GL_SRGB8_ALPHA8
			SLuminance_Alpha,				// GL_SLUMINANCE_ALPHA
			SLuminance8_Alpha8,				// GL_SLUMINANCE8_ALPHA8
			SLuminance,						// GL_SLUMINANCE
			SLuminance8,					// GL_SLUMINANCE8
		};
		enum FramebufferParam : Base
		{
			FramebufferSRGB		= 0x8DB9,	// GL_FRAMEBUFFER_SRGB
			FramebufferRead		= 0x8CA8,	// GL_READ_FRAMEBUFFER
			FramebufferDraw		= 0x8CA9,	// GL_DRAW_FRAMEBUFFER
			FramebufferComplete = 0x8CD5,	// GL_FRAMEBUFFER_COMPLETE
		};
		enum TexParam : Base
		{
			Nearest = 0x2600,				// GL_NEAREST
			Linear,							// GL_LINEAR
			NearestMipmapNearest = 0x2700,	// GL_NEAREST_MIPMAP_NEAREST
			LinearMipmapNearest,			// GL_LINEAR_MIPMAP_NEAREST
			NearestMipmapLinear,			// GL_NEAREST_MIPMAP_LINEAR
			LinearMipmapLinear,				// GL_LINEAR_MIPMAP_LINEAR
			TexMagFilter		= 0x2800,	// GL_TEXTURE_MAG_FILTER
			TexMinFilter,					// GL_TEXTURE_MIN_FILTER
			TexWrapS,						// GL_TEXTURE_WRAP_S
			TexWrapT,						// GL_TEXTURE_WRAP_T
			Clamp				= 0x2900,	// GL_CLAMP
			Repeat,							// GL_REPEAT
			ClampToEdge			= 0x812F,	// GL_CLAMP_TO_EDGE
			MaxTextureSize		= 0x0D33,	// GL_MAX_TEXTURE_SIZE
		};
		enum Unpack : Base
		{
			UnpackSwapBytes = 0x0CF0,		// GL_UNPACK_SWAP_BYTES
			UnpackLsbFirst,					// GL_UNPACK_LSB_FIRST
			UnpackRowLength,				// GL_UNPACK_ROW_LENGTH
			UnpackSkipRows,					// GL_UNPACK_SKIP_ROWS
			UnpackSkipPixels,				// GL_UNPACK_SKIP_PIXELS
			UnpackAlignment,				// GL_UNPACK_ALIGNMENT
		};
		enum Pack : Base
		{
			PackSwapBytes = 0x0D00,			// GL_PACK_SWAP_BYTES
			PackLsbFirst,					// GL_PACK_LSB_FIRST
			PackRowLength,					// GL_PACK_ROW_LENGTH
			PackSkipRows,					// GL_PACK_SKIP_ROWS
			PackSkipPixels,					// GL_PACK_SKIP_PIXELS
			PackAlignment,					// GL_PACK_ALIGNMENT
		};
		enum ColorAttachment : Base
		{
			ColorAttachment0 = 0x8CE0,		// GL_COLOR_ATTACHMENT0
			ColorAttachment1,				// GL_COLOR_ATTACHMENT1
			ColorAttachment2,				// GL_COLOR_ATTACHMENT2
			ColorAttachment3,				// GL_COLOR_ATTACHMENT3
			ColorAttachment4,				// GL_COLOR_ATTACHMENT4
			ColorAttachment5,				// GL_COLOR_ATTACHMENT5
			ColorAttachment6,				// GL_COLOR_ATTACHMENT6
			ColorAttachment7,				// GL_COLOR_ATTACHMENT7
			ColorAttachment8,				// GL_COLOR_ATTACHMENT8
			ColorAttachment9,				// GL_COLOR_ATTACHMENT9
			ColorAttachment10,				// GL_COLOR_ATTACHMENT10
			ColorAttachment11,				// GL_COLOR_ATTACHMENT11
			ColorAttachment12,				// GL_COLOR_ATTACHMENT12
			ColorAttachment13,				// GL_COLOR_ATTACHMENT13
			ColorAttachment14,				// GL_COLOR_ATTACHMENT14
			ColorAttachment15,				// GL_COLOR_ATTACHMENT15
		};
		enum TextureID : Base
		{
			Texture0 = 0x84C0,				// GL_TEXTURE0
			Texture1,						// GL_TEXTURE1
			Texture2,						// GL_TEXTURE2
			Texture3,						// GL_TEXTURE3
			Texture4,						// GL_TEXTURE4
			Texture5,						// GL_TEXTURE5
			Texture6,						// GL_TEXTURE6
			Texture7,						// GL_TEXTURE7
			Texture8,						// GL_TEXTURE8
			Texture9,						// GL_TEXTURE9
			Texture10,						// GL_TEXTURE10
			Texture11,						// GL_TEXTURE11
			Texture12,						// GL_TEXTURE12
			Texture13,						// GL_TEXTURE13
			Texture14,						// GL_TEXTURE14
			Texture15,						// GL_TEXTURE15
			Texture16,						// GL_TEXTURE16
			Texture17,						// GL_TEXTURE17
			Texture18,						// GL_TEXTURE18
			Texture19,						// GL_TEXTURE19
			Texture20,						// GL_TEXTURE20
			Texture21,						// GL_TEXTURE21
			Texture22,						// GL_TEXTURE22
			Texture23,						// GL_TEXTURE23
			Texture24,						// GL_TEXTURE24
			Texture25,						// GL_TEXTURE25
			Texture26,						// GL_TEXTURE26
			Texture27,						// GL_TEXTURE27
			Texture28,						// GL_TEXTURE28
			Texture29,						// GL_TEXTURE29
			Texture30,						// GL_TEXTURE30
			Texture31,						// GL_TEXTURE31
		};
	};
}

#endif // !_GRAPHICS_ENUM_H_
