#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lightning_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"
#include <glm/gtc/random.hpp>

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST:
			_integrator = new RayCastIntegrator(); 
			break;
		case IntegratorType::DIRECT_LIGHTING:
		default:
		{
			_integrator = new DirectLightingIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				Vec3f meanColor = VEC3F_ZERO;
				for (int k = 0; k < _nbPixelSamples; k++)
				{
					float i_normalized = ( i + glm::linearRand( 0.f, 1.0f ) ) / (float)( width - 1 );
					float j_normalized = ( j + glm::linearRand( 0.f, 1.0f ) ) / (float)( height - 1 );
					Ray	  ray		   = p_camera->generateRay( i_normalized, j_normalized );
					meanColor += _integrator->Li( p_scene, ray, 0.f, 100.f );
				}			
				meanColor /= _nbPixelSamples;
				p_texture.setPixel( i, j, glm::clamp(meanColor, 0.f, 1.f) );
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
