#include "implicit_surface.hpp"
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace RT_ISICG
{
	class ImplicitFractal : public ImplicitSurface
	{
	  public:
		ImplicitFractal()		   = delete;
		virtual ~ImplicitFractal() = default;

		ImplicitFractal( const std::string & p_name, const Vec3f & p_center, const float p_radius )
			: ImplicitSurface( p_name ), _center( p_center ), _radius( p_radius )
		{
		}

		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const override
		{

			Vec3f p	  = (p_point - _center)/ _radius;
			float d	  = sdBox( p, Vec3f( 1.f ) );

			float ani = glm::smoothstep( -0.2f, 0.2f, -glm::cos( 0.5f * iTime ) );
			float off = 1.5f * glm::sin( 0.01f * iTime );

			float s = 1.0;
			for ( int m = 0; m < 4; m++ )
			{
				p = glm::mix( p, ma * ( p + off ), ani );

				Vec3f a = glm::mod( p * s, 2.f ) - 1.f;
				s *= 3.0;
				Vec3f  r	 = abs( 1.f - 3.f * abs( a ) );
				float da = glm::max( r.x, r.y );
				float  db	 = glm::max( r.y, r.z );
				float  dc	 = glm::max( r.z, r.x );
				float c	 = ( glm::min( da, glm::min( db, dc ) ) - 1.0 ) / s;

				if ( c > d ) d = c;
			}
			return d * _radius;
		}

		float sdBox( Vec3f p, Vec3f b ) const
		{
			Vec3f di = glm::abs( p ) - b;
			float mc = maxcomp( di );
			return glm::min( mc, glm::length( glm::max( di, 0.f ) ) );
		}

		float maxcomp( const glm::vec3 & v ) const { return std::max( std::max( v.x, v.y ), v.z ); }

	  private:
		const Vec3f _center;
		const float _radius;
		const glm::mat3 ma = glm::mat3( 0.60f, 0.00f, 0.80f, 0.00f, 1.00f, 0.00f, -0.80f, 0.00f, 0.60f );
		const float		iTime = 0.f;
	};

} // namespace RT_ISICG
