#ifndef __RT_ISICG_CSG__
#define __RT_ISICG_CSG__

#include "implicit_surface.hpp"

namespace RT_ISICG
{

	enum class CSG_TYPE : int
	{
		UNION		 = 0,
		INTERSECTION = 1,
		DIFFERENCE = 2
	};
	class CSG : public ImplicitSurface
	{
	  public:
		CSG()		   = delete;
		virtual ~CSG()	= default;

		CSG( const std::string & p_name, ImplicitSurface *p_s1, ImplicitSurface *p_s2, CSG_TYPE p_type )
			: ImplicitSurface( p_name ), _s1( p_s1 ), _s2( p_s2 ), _type( p_type )
		{
			_fct[ 0 ] = &CSG::sdfUnion;
			_fct[ 1 ] = &CSG::sdfIntersection;
			_fct[ 2 ] = &CSG::sdfDifference;

		}

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const override
		{
			float sdf1 = _s1->_sdf( p_point );
			float sdf2 = _s2->_sdf( p_point );

			return ( this->*_fct[ (int) _type ] )( sdf1, sdf2 );
		}

		const float sdfUnion( float sdf1, float sdf2 ) const { return std::min( sdf1, sdf2 ); }

		const float sdfIntersection( float sdf1, float sdf2 ) const { return std::max( sdf1, sdf2 ); }

		const float sdfDifference( float sdf1, float sdf2 ) const { return std::max( sdf1, -sdf2 ); }

		ImplicitSurface *_s1;
		ImplicitSurface *_s2;
		const float ( CSG::*_fct[3] )( float, float ) const;
		const CSG_TYPE _type;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_CSG__