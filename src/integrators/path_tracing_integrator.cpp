#include "path_tracing_integrator.hpp"
#include <random>

namespace RT_ISICG
{
	Vec3f PathTracingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			return _pathTracing( p_scene, p_ray, p_tMin, p_tMax, hitRecord, 0, false );
		}
		else { return _backgroundColor; }
	}

	Vec3f PathTracingIntegrator::_pathTracing( const Scene & p_scene,
											   const Ray &	 p_ray,
											   const float	 p_tMin,
											   const float	 p_tMax,
											   HitRecord	 hitRecord,
											   const int p_nbBounces,
											   const bool p_inMaterial ) const
	{
		
		Vec3f directLighting = _whittedLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord, p_nbBounces, p_inMaterial );
		Vec3f indirectLighting = _indirectLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord, p_nbBounces, p_inMaterial );

		return directLighting + indirectLighting;
	}

	Vec3f PathTracingIntegrator::_whittedLighting( const Scene & p_scene,
											   const Ray &	 p_ray,
											   const float	 p_tMin,
											   const float	 p_tMax,
											   HitRecord	 hitRecord,
											   const int	 p_nbBounces,
											   const bool	 p_inMaterial) const
	{
		if ( p_nbBounces >= _nbBounces ) return BLACK;

		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				Ray reflection_ray = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflection_ray.offset( hitRecord._normal );

				return _pathTracing(
					p_scene, reflection_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, p_inMaterial );
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				Ray reflection_ray = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflection_ray.offset( hitRecord._normal );
				Vec3f reflectionColor
					= _pathTracing( p_scene, reflection_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, p_inMaterial );

				const float n1 = p_inMaterial ? hitRecord._object->getMaterial()->getIOR() : 1.f;
				const float n2 = !p_inMaterial ? hitRecord._object->getMaterial()->getIOR() : 1.f;

				float cos_theta_i = glm::dot( -p_ray.getDirection(), hitRecord._normal );

				float cos_theta_t
					= n1 / n2
					  * sqrtf( std::max( 0.f, 1.f - ( 1.f - cos_theta_i * cos_theta_i ) * ( n1 / n2 ) * ( n1 / n2 ) ) );

				float sqrt_rs = ( n1 * cos_theta_i - n2 * cos_theta_t ) / ( n1 * cos_theta_i + n2 * cos_theta_t );
				float sqrt_rp = ( n1 * cos_theta_t - n2 * cos_theta_i ) / ( n1 * cos_theta_t + n2 * cos_theta_i );

				const float kr = ( sqrt_rs * sqrt_rs + sqrt_rp * sqrt_rp ) / 2.f;
				if ( kr == 1.f ) return reflectionColor;

				const Vec3f refractionDir  = glm::refract( p_ray.getDirection(), hitRecord._normal, n1 / n2 );
				Ray			refraction_ray = Ray( hitRecord._point, refractionDir );
				refraction_ray.offset( -hitRecord._normal );
				Vec3f refractionColor = _pathTracing(
					p_scene, refraction_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, !p_inMaterial );

				return reflectionColor * kr + refractionColor * ( 1.f - kr );
			}
			else
				return _directLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord )
					+ _indirectLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, p_inMaterial );
		}
		return _backgroundColor;
	}

	Vec3f PathTracingIntegrator::_indirectLighting( const Scene &	  p_scene,
													 const Ray &	   p_ray,
													 const float	   p_tMin,
													 const float	   p_tMax,
													 HitRecord hitRecord, 
													 const int	  p_nbBounces,
													 const bool p_inMaterial) const
	{
		if ( p_nbBounces >= _nbBounces ) return BLACK;

		if ( !p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) ) return _backgroundColor;

		Vec3f SumIndirectLighting = VEC3F_ZERO;
		for (int i = 0; i < _nbRays; i++)
		{
			Vec3f indirectLighting = VEC3F_ZERO;

			// Lancer un rayon aléatoire dans une direction aléatoire
			Vec3f randomDirection = sampleHemisphere( hitRecord._normal );
			Ray	  randomRay( hitRecord._point, randomDirection );

			// Calculer la couleur de la lumière indirecte en échantillonnant récursivement le chemin du rayon aléatoire
			indirectLighting
				= _pathTracing( p_scene, randomRay, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, p_inMaterial );

			//return indirectLighting;

			// Retourner la couleur de l'éclairage indirect, pondérée par la probabilité d'échantillonnage de la
			// direction
			SumIndirectLighting += indirectLighting;
		}	
		Vec3f MeanIndirectLighting
			= ( SumIndirectLighting /= _nbRays ) * hitRecord._object->getMaterial()->getFlatColor();
		return MeanIndirectLighting;
	}



	// Fonction pour générer une direction aléatoire dans le demi-hémisphère supérieur
	Vec3f PathTracingIntegrator::sampleHemisphere( const Vec3f & normal ) const
	{
		// Générateur de nombres aléatoires
		static std::default_random_engine			 generator;
		static std::uniform_real_distribution<float> distribution( 0.0f, 1.0f );

		// Générer deux nombres aléatoires dans l'intervalle [0, 1]
		float u = distribution( generator );
		float v = distribution( generator );

		// Calculer les angles polaires et azimutaux
		float theta = acos( sqrt( 1.0f - u ) );
		float phi	= 2 * PIf * v;

		// Convertir en coordonnées cartésiennes
		float x = sin( theta ) * cos( phi );
		float y = sin( theta ) * sin( phi );
		float z = cos( theta );

		// Appliquer une transformation pour aligner avec la normale de la surface
		Vec3f tangent, bitangent;

		// Éviter les vecteurs dégénérés
		if ( fabs( normal.x ) > fabs( normal.y ) )
		{
			tangent = glm::normalize( glm::cross( Vec3f( 0.0f, 1.0f, 0.0f ), normal ) );
		}
		else { tangent = glm::normalize( glm::cross( Vec3f( 1.0f, 0.0f, 0.0f ), normal ) ); }

		bitangent = glm::normalize( glm::cross( normal, tangent ) );

		// Matrice TBN pour la transformation
		Mat3f TBN( tangent, bitangent, normal );
		return glm::normalize( TBN * Vec3f( x, y, z ) );
	}

} // namespace RT_ISICG