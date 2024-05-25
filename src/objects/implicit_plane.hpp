#ifndef __RT_ISICG_IMPLICIT_PLANE__
#define __RT_ISICG_IMPLICIT_PLANE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitPlane : public ImplicitSurface
	{
	  public:
		ImplicitPlane()			  = delete;
		virtual ~ImplicitPlane() = default;

		ImplicitPlane( const std::string & p_name, const Vec3f & p_center, const Vec3f p_normal, const float p_height )
			: ImplicitSurface( p_name ), _center( p_center ), _normal( glm::normalize(p_normal) ), _height( p_height )
		{
		}

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const override
		{ 
			return glm::dot( p_point - _center, _normal ) + _height;
		}

		const float _minDistance = 1e-4f;
		Vec3f		_center;
		Vec3f		_normal;
		float		_height;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_PLANE__
