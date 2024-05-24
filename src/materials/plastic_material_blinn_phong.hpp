#ifndef __RT_ISICG_PLASTIC_MATERIAL_BLINN_PHONG__
#define __RT_ISICG_PLASTIC_MATERIAL_BLINN_PHONG__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/blinn_phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterialBlinnPhong : public BaseMaterial
	{
	  public:
		PlasticMaterialBlinnPhong( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_s )
			: BaseMaterial( p_name ), _lbrdf( p_diffuse ), _bpbrdf( p_specular, p_s )
		{
		}

		virtual ~PlasticMaterialBlinnPhong() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return 0.7f * _lbrdf.evaluate() + 0.3f * _bpbrdf.evaluate( p_ray, p_hitRecord, p_lightSample );
		}

		inline const Vec3f & getFlatColor() const override { return _lbrdf.getKd(); }

	  protected:
		BlinnPhongBRDF _bpbrdf;
		LambertBRDF _lbrdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL_BLINN_PHONG__
