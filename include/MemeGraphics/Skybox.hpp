#ifndef _SKYBOX_HPP_
#define _SKYBOX_HPP_

#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/IDrawable.hpp>

namespace ml
{
	class ML_GRAPHICS_API Skybox final
		: public ITrackable
		, public IDrawable
		, public IReadable
	{
	public:
		enum
		{
			Up, Down, Left, Right, Front, Back,
			MAX_FACE
		};

		using Faces = std::array<Texture *, MAX_FACE>;

	public:
		Skybox();
		~Skybox();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const Texture * up()		const { return m_faces[Up];		}
		inline const Texture * down()	const { return m_faces[Down];	}
		inline const Texture * left()	const { return m_faces[Left];	}
		inline const Texture * right()	const { return m_faces[Right];	}
		inline const Texture * front()	const { return m_faces[Front];	}
		inline const Texture * back()	const { return m_faces[Back];	}

	private:
		Faces m_faces { 0, 0, 0, 0, 0, 0 };
		//Texture * m_faces[MAX_FACE] = { 0 };
	};
}

#endif // !_SKYBOX_HPP_
