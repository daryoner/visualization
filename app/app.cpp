
#include <cassert>
#include <cmath>

#include "../framework/scene.hpp"
#include "../framework/app.hpp"



//-------------------------------------------------------
//	Basic Vector2 class
//-------------------------------------------------------

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2( float vx, float vy );
	Vector2( Vector2 const &other );
};


Vector2::Vector2() :
	x( 0.f ),
	y( 0.f )
{
}


Vector2::Vector2( float vx, float vy ) :
	x( vx ),
	y( vy )
{
}


Vector2::Vector2( Vector2 const &other ) :
	x( other.x ),
	y( other.y )
{
}


Vector2 operator + ( Vector2 const &left, Vector2 const &right )
{
	return Vector2( left.x + right.x, left.y + right.y );
}


Vector2 operator * ( float left, Vector2 const &right )
{
	return Vector2( left * right.x, left * right.y );
}


//-------------------------------------------------------
//	Simple ship logic
//-------------------------------------------------------

class Ship
{
public:
	Ship();

	void init();
	void deinit();
	void update( float dt );
	void mouseClicked( Vector2 worldPosition, bool isLeftButton );

private:
	scene::Mesh *mesh;
	Vector2 position;
	float angle;
};


Ship::Ship() :
	mesh( nullptr )
{
}


void Ship::init()
{
	assert( !mesh );
	mesh = scene::createShipMesh();
	position = Vector2( 0.f, 0.f );
	angle = 0.f;
	
}


void Ship::deinit()
{
	scene::destroyMesh( mesh );
	mesh = nullptr;
}


void Ship::update( float dt )
{
	float linearSpeed = 0.f;
	float angularSpeed = 0.f;

	angle = angle + angularSpeed * dt;
	position = position + linearSpeed * dt * Vector2( std::cos( angle ), std::sin( angle ) );
	scene::placeMesh( mesh, position.x, position.y, angle );
}

void Ship::mouseClicked( Vector2 worldPosition, bool isLeftButton )
{
	
	if ( isLeftButton )
	{
		
	}
	else
	{
		
	}
}


//-------------------------------------------------------
//	game public interface
//-------------------------------------------------------

namespace app
{
	Ship ship;


	void init()
	{
		ship.init();
	}


	void deinit()
	{
		ship.deinit();
	}


	void update( float dt )
	{
		ship.update( dt );
	}

	void mouseClicked( float x, float y, bool isLeftButton )
	{
		Vector2 worldPosition( x, y );
		scene::screenToWorld( &worldPosition.x, &worldPosition.y );
		ship.mouseClicked( worldPosition, isLeftButton );
	}
}

