#ifndef __RT_ISICG_IMPLICIT_CONE__
#define __RT_ISICG_IMPLICIT_CONE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitCone : public ImplicitSurface
	{
	  public:
		ImplicitCone()			  = delete;
		virtual ~ImplicitCone() = default;

		ImplicitCone( const std::string & p_name, const Vec3f & p_center, const Vec2f p_cos, const float p_height )
			: ImplicitSurface( p_name ), _center( p_center ), _cos( p_cos ), _height( p_height )
		{
		}

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point) const override
		{
			Vec2f q = _height * Vec2f( _cos.x / _cos.y, -1.0 );

			Vec3f relativePos = p_point - _center;

			Vec2f w = Vec2f( glm::length( Vec2f( relativePos.x, relativePos.z ) ), relativePos.y );
			Vec2f a = w - q * glm::clamp( glm::dot( w, q ) / glm::dot( q, q ), 0.f, 1.f );
			Vec2f b = w - q * Vec2f( glm::clamp( w.x / q.x, 0.f, 1.f ), 1.f );
			float k = glm::sign( q.y );
			float d = glm::min( glm::dot( a, a ), glm::dot( b, b ) );
			float s = glm::max( k * ( w.x * q.y - w.y * q.x ), k * ( w.y - q.y ) );
			return sqrt( d ) * glm::sign( s );
		}

		const float _minDistance = 1e-4f;
		Vec3f		_center;
		Vec2f		_cos;
		float		_height;
		
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_CONE__