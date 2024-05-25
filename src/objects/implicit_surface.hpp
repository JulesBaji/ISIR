#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;

	  private:
		// Evaluate normal by computing gradient at 'p_point'
		// Calcul des différences finies
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			Vec3f gradient = VEC3F_ZERO;

			gradient.x = _sdf( p_point + Vec3f( _minDistance, 0.0f, 0.0f ) )
						 - _sdf( p_point - Vec3f( _minDistance, 0.0f, 0.0f ) );
			gradient.y = _sdf( p_point + Vec3f( 0.0f, _minDistance, 0.0f ) )
						 - _sdf( p_point - Vec3f( 0.0f, _minDistance, 0.0f ) );
			gradient.z = _sdf( p_point + Vec3f( 0.0f, 0.0f, _minDistance ) )
						 - _sdf( p_point - Vec3f( 0.0f, 0.0f, _minDistance ) );

			return glm::normalize( gradient );
		}

	  private:
		const float _minDistance = 1e-4f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
