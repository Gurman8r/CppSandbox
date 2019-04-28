#ifndef _ML_CONVEX_HULL_HPP_
#define _ML_CONVEX_HULL_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API ConvexHull
		: public ITrackable
	{
	public:
		enum Type
		{
			T_None,
			T_Point,
			T_Plane,
			T_Disc,
			T_Box,
			T_Sphere,
			T_Cylinder,
			T_Mesh,

			MAX_CONVEX_HULL_TYPE
		};

		enum Flag
		{
			F_None,
			F_3D,
			F_AxisAligned,

			MAX_CONVEX_HULL_FLAG
		};

		enum Property
		{
			P_Box_Width,
			P_Box_Height,
			P_Box_Depth,
			P_Box_HalfWidth,
			P_Box_HalfDepth,
			P_Box_HalfWidthSqr,
			P_Box_HalfHeightSqr,
			P_Box_HalfDepthSqr,

			P_Sph_Radius,
			P_Sph_Length,
			P_Sph_RadiusSqr,
			P_Sph_LengthSqr,

			MAX_CONVEX_HULL_PROPERTY
		};

	public:
		ConvexHull();
		~ConvexHull();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONVEX_HULL_HPP_