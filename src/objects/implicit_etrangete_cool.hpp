#ifndef __RT_ISICG_IMPLICIT_ETRANGETE_COOL__
#define __RT_ISICG_IMPLICIT_ETRANGETE_COOL__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitEtrangeteCool : public ImplicitSurface
	{
	  public:
		ImplicitEtrangeteCool()	 = delete;
		virtual ~ImplicitEtrangeteCool() = default;

		ImplicitEtrangeteCool( const std::string & p_name,
							   const Vec3f &	   p_center,
							   const float		   p_radius,
							   const float		   p_height, 
			const float p_thickness)
			: ImplicitSurface( p_name ), _center( p_center ), _radius( p_radius ), _height( p_height ), _thickness(p_thickness)
		{
		}

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const override
		{
			// sampling independent computations (only depend on shape)
			float w = sqrt( _radius * _radius - _height * _height );

			Vec3f relativePos = p_point - _center;

			// sampling dependant computations
			Vec2f q = Vec2f( glm::length( Vec2f( relativePos.x, relativePos.z ) ), relativePos.y );
			return ( ( _height * q.x < w * q.y ) ? glm::length( q - Vec2f( w, _height ) ) : glm::abs( glm::length( q ) - _radius ) )
				   - _thickness;
		}

		const float _minDistance = 1e-4f;
		Vec3f		_center;
		float 		_radius;
		float		_height;
		float		_thickness;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_ETRANGETE_COOL__