#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Camera final
		: public ITrackable
	{
	public:
		Camera();
		Camera(const Camera & copy);
		~Camera();

	public:
		void update(const vec2i & resolution);

	public:
		void orbit(const vec3f & target, const float speed);

	public:
		inline const vec3f		& position()	const	{ return m_position;	}
		inline const Transform	& transform()	const	{ return m_transform;	}
		inline const Transform	& ortho()		const	{ return m_ortho;		}
		inline const Transform	& persp()		const	{ return m_persp;		}
		inline const float		& fieldOfView() const	{ return m_fieldOfView; }
		inline const float		& perspNear()	const	{ return m_perspNear;	}
		inline const float		& perspFar()	const	{ return m_perspFar;	}
		inline const float		& orthoNear()	const	{ return m_orthoNear;	}
		inline const float		& orthoFar()	const	{ return m_orthoFar;	}

	public:
		inline vec3f			& position()			{ return m_position;	}
		inline Transform		& transform()			{ return m_transform;	}
		inline Transform		& ortho()				{ return m_ortho;		}
		inline Transform		& persp()				{ return m_persp;		}
		inline float			& fieldOfView()			{ return m_fieldOfView; }
		inline float			& perspNear()			{ return m_perspNear;	}
		inline float			& perspFar()			{ return m_perspFar;	}
		inline float			& orthoNear()			{ return m_orthoNear;	}
		inline float			& orthoFar()			{ return m_orthoFar;	}

	private:
		vec2i		m_resolution;
		vec3f		m_position;
		Transform	m_transform;
		Transform	m_ortho;
		Transform	m_persp;
		float		m_fieldOfView;
		float		m_perspNear;
		float		m_perspFar;
		float		m_orthoNear;
		float		m_orthoFar;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_