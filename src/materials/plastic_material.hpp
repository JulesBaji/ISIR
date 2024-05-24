#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_s )
			: BaseMaterial( p_name ), _lbrdf( p_diffuse ), _pbrdf( p_specular, p_s )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return 0.7f*_lbrdf.evaluate()  + 0.3f*_pbrdf.evaluate( p_ray, p_hitRecord, p_lightSample );
		}

		inline const Vec3f & getFlatColor() const override { return _lbrdf.getKd(); }

	  protected:
		PhongBRDF	_pbrdf;
		LambertBRDF _lbrdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__
