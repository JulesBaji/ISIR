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
		p_hitRecord._distance = p_tMax;
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
		//p_node->_aabb = AABB();

		for ( unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
			p_node->_aabb.extend( _triangles->at( i ).getAABB() );

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;

		if ( p_lastTriangleId - p_firstTriangleId + 1 <= _maxTrianglesPerLeaf || p_depth >= _maxDepth ) return;

		const unsigned int axePartition = static_cast<unsigned int>(p_node->_aabb.largestAxis());
		const float middle		= p_node->_aabb.centroid()[ axePartition ];

		// Fonction de tri testant l'AABB de chaque TriangleMeshGeometry, renvoie un booléen
		std::function<bool( TriangleMeshGeometry )> splitFunction(
			[ axePartition, middle ]( TriangleMeshGeometry t ) -> bool
			{ 
				return t.getAABB().centroid()[ axePartition ] < middle;
			} );

		// Itérateur de type TriangleMeshGeometry avec fonction de tri
		std::vector<TriangleMeshGeometry>::iterator it = std::partition( _triangles->begin() + p_firstTriangleId,
												_triangles->begin() + p_lastTriangleId, 
												splitFunction );

		// Indice du triangle 
		const unsigned int idPartition = static_cast<unsigned int>(std::distance( _triangles->begin(), it ));

		p_node->_left  = new BVHNode();
		p_node->_right = new BVHNode();

		_buildRec( p_node->_left, p_firstTriangleId, idPartition, p_depth + 1 );
		_buildRec( p_node->_right, idPartition, p_lastTriangleId, p_depth + 1 );
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= p_node->_lastTriangleId; // Hit triangle id.
		Vec2f  uvHit;
		if (p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
		{
			if ( p_node->isLeaf() )
			{ 
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					float t;
					Vec2f uv;
					if ( _triangles->at( i ).intersect( p_ray, t, uv ) )
					{
						if ( t >= p_tMin && t < tClosest )
						{
							tClosest = t;
							hitTri	 = i;
							uvHit	 = uv;
						}
					}
				}
				if ( hitTri != p_node->_lastTriangleId ) // Intersection found.
				{
					p_hitRecord._point	= p_ray.pointAtT( tClosest );
					p_hitRecord._normal = _triangles->at( hitTri ).getFaceNormal( uvHit );
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._distance = tClosest;
					p_hitRecord._object	  = (const BaseObject *)( _triangles->at( hitTri ).getRefMesh() );

					return true;
				}
				return false;
			}
			if ( p_node->_left )
			{ 
				if ( _intersectRec( p_node->_left, p_ray, p_tMin, p_hitRecord._distance, p_hitRecord ) ) return true;
			}
			if ( p_node->_right ) 
			{ 
				if ( _intersectRec( p_node->_right, p_ray, p_tMin, p_hitRecord._distance, p_hitRecord ) ) return true;
			}
		}
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if (p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
		{
			if ( p_node->isLeaf() )
			{
				float p_t;
				Vec2f uv;
				for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				{
					if ( _triangles->at( i ).intersect( p_ray, p_t, uv ) )
					{
						std::cout << "test" << std::endl;
						if ( p_t >= p_tMin && p_t <= p_tMax ) return true;
					}
				}
				return false;
			}
			if ( p_node->_left ) return _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			if ( p_node->_right ) return _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
		}
		return false;
	}
} // namespace RT_ISICG
