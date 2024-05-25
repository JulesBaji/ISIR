#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitSphere : public ImplicitSurface
	{
	  public:
		ImplicitSphere()		  = delete;
		virtual ~ImplicitSphere() = default;

		ImplicitSphere( const std::string & p_name, const Vec3f & p_center, const float p_radius )
			: ImplicitSurface( p_name ), _center(p_center), _radius( p_radius )
		{
		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{ 
			return glm::length( p_point - _center ) - _radius;
		}

		// Evaluate normal by computing gradient at 'p_point'
		// Calcul des différences finies
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const override
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
		Vec3f _center;
		float _radius;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__
#pragma once
