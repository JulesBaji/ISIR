#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <functional>
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		_buildRec( _root, 0, static_cast<unsigned int>(p_triangles->size()), 1 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord ); 
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const size_t p_depth )
	{
		if ( p_lastTriangleId - p_firstTriangleId <= _maxTrianglesPerLeaf || p_depth >= _maxDepth ) return;

		//p_node->_aabb = AABB();

		for ( unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
			p_node->_aabb.extend( ( *_triangles )[ i ].getAABB() );
		
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;

		const unsigned int axePartition = static_cast<unsigned int>(p_node->_aabb.largestAxis());
		const float middle		= p_node->_aabb.centroid()[ axePartition ];

		// Fonction de tri testant l'AABB de chaque TriangleMeshGeometry, renvoie un booléen
		std::function<bool( TriangleMeshGeometry )> splitFunction(
			[ axePartition, middle ]( TriangleMeshGeometry t ) -> bool
			{ 
				return t.getAABB().getMax()[ axePartition ] < middle;
			} );

		// Itérateur de type TriangleMeshGeometry avec fonction de tri
		std::vector<TriangleMeshGeometry>::iterator it = std::partition( _triangles->begin() + p_firstTriangleId,
												_triangles->begin() + p_lastTriangleId, 
												splitFunction );

		// Indice du triangle 
		const unsigned int idPartition = static_cast<unsigned int>(std::distance( _triangles->begin(), it ));

		p_node->_left  = new BVHNode();
		p_node->_right = new BVHNode();

		_buildRec( p_node, p_firstTriangleId, idPartition, p_depth + 1 );
		_buildRec( p_node, idPartition, p_lastTriangleId, p_depth + 1 );
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( !p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;

		if (p_node->isLeaf())
		{ 
			float p_t;
			Vec2f uv;
			for (unsigned int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++)
			{
				if ((*_triangles)[i].intersect(p_ray, p_t, uv))
				{
					if ( p_t >= p_tMin && p_t <= p_tMax )
					{

					}
				}
			}
		}
		// Si ça marche pas vérifier nullptr
		if ( p_node->_left ) _intersectRec( p_node, p_ray, p_tMin, p_tMax, p_hitRecord );
		else if ( p_node->_right ) _intersectRec( p_node, p_ray, p_tMin, p_tMax, p_hitRecord );
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		/// TODO
		return false;
	}
} // namespace RT_ISICG
