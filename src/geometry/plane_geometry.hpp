#ifndef __RT_ISICG_PLANE_GEOMETRY__
#define __RT_ISICG_PLANE_GEOMETRY__

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class PlaneGeometry : public BaseGeometry
	{
	  public:
		PlaneGeometry()		  = delete;
		virtual ~PlaneGeometry() = default;

		PlaneGeometry( const Vec3f & p_center, const Vec3f & p_normale )
			: _center( p_center ), _normale( glm::normalize(p_normale) )
		{
		}

		inline const Vec3f & getCenter() const { return _center; }
		inline const Vec3f & getNormale() const { return _normale; }

		bool intersect( const Ray & p_ray, float & p_t1 ) const;

		inline Vec3f computeNormal( const Vec3f & p_point ) const { return _normale; }

	  private:
		Vec3f _center = VEC3F_ZERO;
		Vec3f _normale = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLANE_GEOMETRY__
