
#include <windows.h>
#include <GL/gl.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

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
		
		float rvalue;
		float gvalue;
		float bvalue;

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
	Mesh *createMesh(float rv, float gv, float bv)
	{
		
		Mesh *mesh = new MeshClass;
		mesh->rvalue = rv;
		mesh->gvalue = gv;
		mesh->bvalue = bv;
		Mesh::meshes.push_back( mesh );
		//std::cout << Mesh::meshes.size() << std::endl;
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

		int triangleAmount = 32;
		int lineAmount = 100;
		GLfloat radius = 0.7f;
		GLfloat twicePi = 2.f * std::atan(1)*4.f;

		glBegin(GL_TRIANGLE_FAN);
			glColor4f(rvalue, gvalue, bvalue, 1.f);
			
			glVertex2f(0.f, 0.f);
			for (int i = 0; i <= triangleAmount; i++)
			{
				glVertex2f(
					radius * cos(i * twicePi / triangleAmount),
					radius * sin(i * twicePi / triangleAmount)
				);
			}
		glEnd();

		glLineWidth(2.f);
		glBegin(GL_LINE_LOOP);
			glColor4f(234 / 255.f, 225 / 255.f, 219 / 255.f, 1.f);
			for (int i = 0; i <= lineAmount; i++)
			{
				glVertex2f(
					radius * cos(i * twicePi / lineAmount),
					radius * sin(i * twicePi / lineAmount)
				);
			}
		glEnd();
	}
}


namespace scene
{
	//-------------------------------------------------------
	Mesh *createSquareMesh(float rv, float gv, float bv)
	{
		return createMesh< SquareMesh >(rv, gv, bv);
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
		glClearColor( 183/255.f, 167/255.f, 160/255.f, 0.f );
		glClear( GL_COLOR_BUFFER_BIT );
		glMatrixMode( GL_MODELVIEW );

		for ( Mesh *mesh : Mesh::meshes )
			mesh->draw();
	}
}
