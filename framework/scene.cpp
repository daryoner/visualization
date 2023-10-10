
#include <windows.h>
#include <GL/gl.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

#include "scene.hpp"


namespace scene
{
	constexpr float VIEW_WIDTH = 18.f;
	constexpr float VIEW_HEIGHT = 13.5f;
}

//-------------------------------------------------------
//	user interface: common mesh support
//-------------------------------------------------------

namespace scene
{
	class Mesh
	{
	public:
		float positionX = 0.f;
		float positionY = 0.f;
		float angle = 0.f;

		virtual ~Mesh();
		virtual void draw();
		virtual void update( float dt );

		static std::vector< Mesh* > meshes;
	};


	//-------------------------------------------------------
	std::vector< Mesh* > Mesh::meshes;


	//-------------------------------------------------------
	Mesh::~Mesh()
	{
	}


	//-------------------------------------------------------
	void Mesh::draw()
	{
		glLoadIdentity();
		glTranslatef( positionX, positionY, 0.f );
		glRotatef( angle * 180.f / 3.14159265f, 0.f, 0.f, 1.f );
	}


	//-------------------------------------------------------
	void Mesh::update( float dt )
	{
	}


	//-------------------------------------------------------
	template< class MeshClass >
	Mesh *createMesh()
	{
		Mesh *mesh = new MeshClass;
		Mesh::meshes.push_back( mesh );
		return mesh;
	}


	//-------------------------------------------------------
	void destroyMesh( Mesh *mesh )
	{
		auto it = std::find( Mesh::meshes.begin(), Mesh::meshes.end(), mesh );
		assert( it != Mesh::meshes.end() );
		Mesh::meshes.erase( it );
		delete mesh;
	}


	//-------------------------------------------------------
	void placeMesh( Mesh *mesh, float x, float y, float angle )
	{
		mesh->positionX = x;
		mesh->positionY = y;
		mesh->angle = angle;
	}
}


//-------------------------------------------------------
//	user interface: ShipMesh support
//-------------------------------------------------------

namespace
{
	class SquareMesh : public scene::Mesh
	{
	public:
		void draw() override;
	};


	//-------------------------------------------------------
	void SquareMesh::draw()
	{
		Mesh::draw();

		glBegin( GL_QUADS );
			glColor3f( 120/255.f, 120/255.f, 120/255.f ); // заливка

			glVertex2f( -0.8f, -0.8f );
			glVertex2f( -0.8f, 0.8f );
			glVertex2f( 0.8f, 0.8f );
			glVertex2f(0.8f, -0.8f);

		glEnd();

		glLineWidth( 2.f );
		glBegin( GL_LINE_LOOP );
		glColor3f(202 / 255.f, 202 / 255.f, 202 / 255.f); // обводка

		glVertex2f(-0.8f, -0.8f);
		glVertex2f(-0.8f, 0.8f);
		glVertex2f(0.8f, 0.8f);
		glVertex2f(0.8f, -0.8f);

		glEnd();
	}
}


namespace scene
{
	//-------------------------------------------------------
	Mesh *createSquareMesh()
	{
		return createMesh< SquareMesh >();
	}
}


//-------------------------------------------------------
//	user interface: utility functions
//-------------------------------------------------------

namespace scene
{
	void screenToWorld( float *x, float *y )
	{
		*x = 0.5f * VIEW_WIDTH * ( 2.f * *x - 1.f );
		*y = 0.5f * VIEW_HEIGHT * ( 2.f * *y - 1.f );
	}
}


//-------------------------------------------------------
//	engine only interface
//-------------------------------------------------------

namespace scene
{
	void update( float dt )
	{
		for ( Mesh *mesh : Mesh::meshes )
			mesh->update( dt );
	}


	void draw()
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glScalef( 2.f / VIEW_WIDTH, 2.f / VIEW_HEIGHT, 0.f );

		glDisable( GL_CULL_FACE );
		glClearColor( 97/255.f, 86/255.f, 71/255.f, 0.f );
		glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_MODELVIEW );

		for ( Mesh *mesh : Mesh::meshes )
			mesh->draw();
	}
}
