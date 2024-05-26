#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/oren_nayar_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/plastic_material_blinn_phong.hpp"
#include "materials/microfacet_material.hpp"
#include "materials/mirror_material.hpp"
#include "materials/transparent_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "objects/implicit_sphere.hpp"
#include "objects/implicit_cone.hpp"
#include "objects/implicit_etrangete_cool.hpp"
#include "objects/implicit_plane.hpp"
#include "objects/implicit_fractal.hpp"
#include "objects/csg.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"
#include "objects/triangle_mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
		switch ( sceneNumber )
		{
			case 1: _init1(); break;
			case 2: _init2(); break;
			case 3: _init3(); break;
			case 4: _init4(); break;
			case 5: _init5(); break;
			case 6: _init6(); break;
			case 7: _init7(); break;
			case 8: _init8(); break;
			case 9: _init9(); break;
			case 10: _init10(); break;
			case 11: _init11(); break;
			case 12: _init12(); break;
			case 13: _init13(); break;
			case 14:
			default: _init14(); break;
		}
	}

	void Scene:: _init1()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
	}

	void Scene::_init2()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 100, "light1", Vec3f( 1, 10, 1 ) ) );
	}

	void Scene::_init3()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 3.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new QuadLight( Vec3f( 1, 10, 2 ), Vec3f( -2, 0, 0 ), Vec3f( 0, 0, 2 ), WHITE, 40 ) );
	}

	void Scene::_init4()
	{
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new ColorMaterial( " RedColor ", RED ) );
		_addMaterial( new ColorMaterial( " GreenColor ", GREEN ) );
		_addMaterial( new ColorMaterial( " BlueColor ", BLUE ) );
		_addMaterial( new ColorMaterial( " GreyColor ", GREY ) );
		_addMaterial( new ColorMaterial( " MagentaColor ", MAGENTA ) );
		_addMaterial( new ColorMaterial( " YellowColor ", YELLOW ) );
		_addMaterial( new ColorMaterial( " CyanColor ", CYAN ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// OBJ.
		loadFileTriangleMesh( "Conference", "data/conference/conference.obj" );
		_attachMaterialToObject( " CyanColor ", "Conference_defaultobject" );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( " GreyColor ", " PlaneGround " );
		_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( " RedColor ", " PlaneLeft " );
		_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( " GreenColor ", " PlaneCeiling " );
		_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( " BlueColor ", " PlaneRight " );
		_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( " MagentaColor ", " PlaneFront " );
		_addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		_attachMaterialToObject( " YellowColor ", " PlaneRear " );
		// ================================================================
		// Add lights .
		// ================================================================
		_addLight( new PointLight( WHITE, 100.f, "lightTp4", Vec3f( 0.f, 3.f, -5.f ) ) );
		// Add Lights
		_addLight( new QuadLight( Vec3f( 900, 600, -300 ), Vec3f( -800, 0, 0 ), Vec3f( 0, 0, 300 ), WHITE, 20 ) );
	}

	void Scene::_init5()
	{
		// ================================================================
		// Add materials .
		// ================================================================
		// ================================================================
		// Add objects .
		// ================================================================
		// OBJ.
		loadFileTriangleMesh( "Conference", "data/conference/conference.obj" );
		// ================================================================
		// Add lights .
		// ================================================================
		//
		_addLight( new QuadLight( Vec3f( 900, 600, -300 ), Vec3f( -800, 0, 0 ), Vec3f( 0, 0, 300 ), WHITE, 20 ) );
	}

	void Scene::_init6()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 3.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new MatteMaterial( "Grey", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "Red", RED, 0.6f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Grey", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 60.f, "lightTp5", Vec3f( 0.f, 0.f, -2.f ) ) );
	}

	void Scene::_init7()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 3.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new PlasticMaterial( "Grey", GREY, GREY, 8.f ) );
		_addMaterial( new PlasticMaterial( "Red", RED, GREY, 8.f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Grey", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 60.f, "lightTp5", Vec3f( 0.f, 0.f, -2.f ) ) );
	}

	void Scene::_init8()
	{
		// Add objects.
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -1.f, 3.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new PlasticMaterial( "Red", BLACK, RED, 8.f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 60.f, "lightTp5", Vec3f( 0.f, 2.f, 4.f ) ) );
	}

	void Scene::_init9()
	{
		// Add objects.
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -1.f, 3.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new PlasticMaterialBlinnPhong( "Red", BLACK, RED, 8.f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 60.f, "lightTp5", Vec3f( 0.f, 2.f, 4.f ) ) );
	}

	void Scene::_init10()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new MicrofacetMaterial( "Gold", Vec3f( 1.f, 0.85f, 0.57f ), Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.5f ) );
		_addMaterial( new MicrofacetMaterial( "Red", RED, GREY, 0.6f, 0.5f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Gold", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 60.f, "lightTp5", Vec3f( 0.f, 0.f, -2.f ) ) );
	}

	void Scene::_init11()
	{
		// Add objects.
		loadFileTriangleMesh( "Bunny", "data/Bunny.obj" );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new MicrofacetMaterial( "Gold", Vec3f( 1.f, 0.85f, 0.57f ), Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 1.f ) );
		_addMaterial( new MicrofacetMaterial( "Red", RED, GREY, 0.6f, 0.5f ) );
		
		// Link objects and materials.
		_attachMaterialToObject( "Gold", "Bunny" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 100.f, "lightTp5", Vec3f( 0.f, 6.f, -3.f ) ) );
	}

	void Scene::_init12()
	{
		// ================================================================
		// Add materials.
		// ================================================================
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		_addMaterial( new MirrorMaterial( "Mirror") );
		_addMaterial( new TransparentMaterial( "Transparent", 1.3f ) );

		// ================================================================
		// Add objects.
		// ================================================================s
		// Spheres.
		_addObject( new Sphere( "Sphere1", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Mirror", "Sphere1" );
		_addObject( new Sphere( "Sphere2", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Transparent", "Sphere2" );

		// Pseudo Cornell box made with infinite planes.
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneFront" );

		// ================================================================
		// Add lights.
		// ================================================================
		_addLight( new PointLight( WHITE, 100.f, "lightTP6", Vec3f( 0.f, 5.f, 0.f ) ) );
		 //_addLight(new QuadLight(Vec3f(1.f, 5.f, -2.f), Vec3f(-2.f, 0.f, 0.f),
			//Vec3f(0.f, 1.f, 2.f), WHITE, 40.f));
	}

	void Scene::_init13()
	{
		// Add objects.
		constexpr float	  angle30	= glm::radians( 30.f );
		Vec2f angleCone1 = glm::vec2( glm::sin( angle30 ), glm::cos( angle30 ) );
		ImplicitSphere *   Sphere1		 = new ImplicitSphere( "Sphere1", Vec3f( -5.f, 8.f, 8.f ), 1.f );
		ImplicitSphere *  Sphere2	 = new ImplicitSphere( "Sphere2", Vec3f( 0.f, 5.f, 5.f ), 1.f );
		ImplicitCone *	   Cone1		 = new ImplicitCone( "Cone1", Vec3f( -3.f, 5.f, 5.f ), angleCone1, 2.f );
		ImplicitCone	  *Cone2		 = new ImplicitCone( "Cone1", Vec3f( 0.f, 5.f, 5.f ), angleCone1, 5.f );
		_addObject( Sphere1 );
		_addObject( Cone1  );
		_addObject( new ImplicitEtrangeteCool( "EtrangeteCool1", Vec3f( 5.f, 0.f, 3.f ), 1.5f, 0.5f, 0.05f ) );
		_addObject( new ImplicitPlane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ), 0.f ) );
		_addObject( new CSG( "csg1", Cone2, Sphere2, CSG_TYPE::UNION ) );

		// Add materials.
		_addMaterial(
			new MicrofacetMaterial( "Gold", Vec3f( 1.f, 0.85f, 0.57f ), Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.5f ) );
		_addMaterial( new MicrofacetMaterial( "Red", RED, GREY, 0.6f, 0.5f ) );

		// Link objects and materials.
		_attachMaterialToObject( "Gold", "Sphere1" );
		_attachMaterialToObject( "Gold", "Sphere2" );
		_attachMaterialToObject( "Gold", "Cone1" );
		_attachMaterialToObject( "Gold", "Cone2" );
		_attachMaterialToObject( "Gold", "EtrangeteCool1" );
		_attachMaterialToObject( "Gold", "csg1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( WHITE, 300.f, "lightTp5", Vec3f( 0.f, 8.f, 0.f ) ) );
	}

	void Scene::_init14()
	{
		// ================================================================
		// Add materials.
		// ================================================================
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.7f ) );
		_addMaterial( new MirrorMaterial( "Mirror" ) );
		_addMaterial( new TransparentMaterial( "Transparent", 1.3f ) );
		_addMaterial(
			new MicrofacetMaterial( "Gold", Vec3f( 1.f, 0.85f, 0.57f ), Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.5f ) );

		// ================================================================
		// Add objects.
		// ================================================================s
		// Spheres.
		_addObject( new Sphere( "Sphere1", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Mirror", "Sphere1" );
		_addObject( new Sphere( "Sphere2", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Transparent", "Sphere2" );
		_addObject( new ImplicitFractal( "fractal1", Vec3f( 0.f, -2.f, 2.f ), .8f ) );
		_attachMaterialToObject( "WhiteMatte", "fractal1" );
		Vec2f			 angleCone1 = glm::vec2( glm::sin( glm::radians( 30.f ) ), glm::cos( glm::radians( 30.f ) ) );
		ImplicitSphere * Sphere2	= new ImplicitSphere( "Sphere2", Vec3f( 0.f, .1f, 2.f ), .2f );
		ImplicitCone *	 Cone2		= new ImplicitCone( "Cone1", Vec3f( 0.f, .1f, 2.f ), angleCone1, 1.3f );
		_addObject( new CSG( "chapeauDeClown", Cone2, Sphere2, CSG_TYPE::UNION ) );
		_attachMaterialToObject( "Mirror", "chapeauDeClown" );

		// Pseudo Cornell box made with infinite planes.
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneFront" );

		// ================================================================
		// Add lights.
		// ================================================================
		_addLight( new PointLight( WHITE, 100.f, "lightTP6", Vec3f( 0.f, 5.f, 0.f ) ) );
	}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			triMesh->buildBVH();
			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );


				// Couleur du Mesh !
				//_addMaterial( new ColorMaterial( std::string( mtlName.C_Str() ), kd ) );
				_addMaterial( new MicrofacetMaterial( "Gold", Vec3f( 1.f, 0.85f, 0.57f ), Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 1.f ) );
				_attachMaterialToObject( "Gold", meshName );
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::_addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			std::cout << "Material \'" << name << "\' added." << std::endl;
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
