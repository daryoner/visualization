
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

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

Vector2 operator - (Vector2 const& left, Vector2 const& right)
{
	return Vector2(left.x - right.x, left.y - right.y);
}

Vector2 operator * ( float left, Vector2 const &right )
{
	return Vector2( left * right.x, left * right.y );
}

bool operator == (Vector2 const& left, Vector2 const& right)
{
	return (fabs(left.x - right.x) < 0.001f) && (fabs(left.y - right.y) < 0.001f);
}

bool operator != (Vector2 const& left, Vector2 const& right)
{
	return !((fabs(left.x - right.x) < 0.001f) && (fabs(left.y - right.y) < 0.001f));
}

namespace aniMath
{
	float Lerp(float left, float right, float rate)
	{
		return left + (right - left) * rate;
	}
}

namespace params
{
	int circleAmount = 10;
}

//-------------------------------------------------------
//	Simple square logic
//-------------------------------------------------------

class Square
{
public:
	Square();

	void init(int factor);
	void deinit();
	void update( float dt );
	void mouseClicked( Vector2 worldPosition, bool isLeftButton );
	void setTarget(Vector2 newTarget);
	void AnimateToTarget(float dt);


	int value;
	int currentIndex;


private:
	scene::Mesh *mesh;
	Vector2 position;
	float angle;
	Vector2 target;
	float animationSpeed;
	
};


Square::Square() :
	mesh( nullptr )
{
}


void Square::init(int factor)
{
	std::srand(factor * std::time(0));
	assert( !mesh );

	
	//std::cout << 255. - (218. * (factor / 9.)) << std::endl;

	mesh = scene::createSquareMesh((255.-(11.*(value/ (float)(params::circleAmount - 1))))/255., (255. - (218. * (value / (float)(params::circleAmount - 1))))/255.,(255. - (149. * (value / (float)(params::circleAmount - 1))))/255.);

	//position = Vector2()
	position = Vector2(-8. + (16./ (float)(params::circleAmount - 1)) * factor , 3.);
	
	target = position;
	angle = 0.f;
	animationSpeed = 2.f;
	
}


void Square::deinit()
{
	scene::destroyMesh( mesh );
	mesh = nullptr;
}


void Square::update( float dt )
{
	
	if (position != target)
	{
		AnimateToTarget(dt);
	}
	scene::placeMesh( mesh, position.x, position.y, angle );
}

void Square::mouseClicked( Vector2 worldPosition, bool isLeftButton )
{
	
	if ( isLeftButton )
	{
		//setTarget(worldPosition);
		
	}
	else
	{
		
	}
}

void Square::setTarget(Vector2 newTarget)
{
	
		

	target = newTarget;
}

void Square::AnimateToTarget(float dt)
{
	
	position = Vector2(aniMath::Lerp(position.x, target.x, dt * animationSpeed), aniMath::Lerp(position.y, target.y, dt*animationSpeed));
}


enum class QsortStatus
{
	NewSortBorders,
	SetPivot,//pivot is set, move array down
	SwapFound,
	SetBorders


};

class SortBorders
{
public:
	int L, R;

	SortBorders();
	SortBorders(int, int);
};

SortBorders::SortBorders()
{

}

SortBorders::SortBorders(int left, int right)
{
	L = left;
	R = right;
}


class SquareSorter
{
public:
	SquareSorter();
	~SquareSorter();

	void NextStep(std::vector<Square*> &v);

private:
	QsortStatus qSortStatus = QsortStatus::NewSortBorders;
	int i, j, mid;
	std::vector<SortBorders> sb;
	Square* pivot;
};

void SquareSorter::NextStep(std::vector<Square*> &v)
{
	switch (qSortStatus)
	{
	case QsortStatus::NewSortBorders:
		//set sort borders, set pivot
		break;
	case QsortStatus::SetPivot:
		//pivot is set, search for elements, show elements
		break;
	case QsortStatus::SwapFound:
		//swap highlighted elements, move back
		break;
	case QsortStatus::SetBorders:
		//set new borders, separate vectors, 
		break;

	}
}

SquareSorter::SquareSorter()
{
	sb.push_back(SortBorders());
	sb[0].L = 0;
	sb[0].R = params::circleAmount - 1;
	i = sb[0].L;
	j = sb[0].R;
	mid = sb[0].L + (sb[0].R - sb[0].L) / 2;
}

SquareSorter::~SquareSorter()
{
}


//-------------------------------------------------------
//	game public interface
//-------------------------------------------------------

namespace app
{
	//Square square;
	std::vector<Square*> squareVector;
	static std::vector<bool> colorPickerVector;
	bool clicked = false;
	SquareSorter ss;



	void init()
	{
		for (int i = 0; i < params::circleAmount; i++)
			colorPickerVector.push_back(false);
		
		//square.init();
		for (int i = 0; i < params::circleAmount; i++)
		{

			int newValue;
			squareVector.push_back(new Square());
			do
			{
				newValue = std::rand() % params::circleAmount;

			} while (colorPickerVector[newValue]);
			colorPickerVector[newValue] = true;
			squareVector[i]->value = newValue;
			squareVector[i]->currentIndex = i;
			//std::cout << squareVector[i]->value << " " << squareVector[i]->currentIndex << std::endl;

			squareVector[i]->init(i);
		}
	}


	void deinit()
	{
		for (int i = 0; i < params::circleAmount; i++)
		{
			squareVector[i]->deinit();
		}
	}


	void update( float dt )
	{
		for (int i = 0; i < params::circleAmount; i++)
		{
			
			squareVector[i]->setTarget(Vector2(-8. + (16. / (float)(params::circleAmount - 1)) * squareVector[i]->currentIndex, 3.));
			squareVector[i]->update(dt);
		}
	}

	bool MyCompare(Square* left, Square* right)
	{
		return left->value < right->value;
	}

	bool NotMyCompare(Square* left, Square* right)
	{
		return left->value > right->value;
	}

	void mouseClicked( float x, float y, bool isLeftButton )
	{
		Vector2 worldPosition( x, y );
		scene::screenToWorld( &worldPosition.x, &worldPosition.y );
		

		ss.NextStep(squareVector);

		if (!clicked)
		{
			std::sort(squareVector.begin(), squareVector.end(), MyCompare);
			clicked = !clicked;
		}

			

		for (int i = 0; i < params::circleAmount; i++)
		{
			squareVector[i]->currentIndex = i;
		}

		
	}

	
	
}

