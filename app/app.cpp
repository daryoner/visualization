
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
	int circleAmount = 100;
	int frameDelay = 1;
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
	Vector2 GetPosition();
	float GetTarget();


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
	animationSpeed = 4.f;
	
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

Vector2 Square::GetPosition()
{
	return position;
}

float Square::GetTarget()
{
	return target.y;
}


enum class QsortStatus
{
	NewSortBorders,
	SetPivot,
	FindPair,
	SwapFound,
	SetBorders,
	Finish


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

	void PrintStatus();
};

void SquareSorter::PrintStatus()
{
	//std::cout << "status: ";
	switch (qSortStatus)
	{
	case QsortStatus::NewSortBorders:// 0
		//std::cout << "NewSortBorders | ";
		break;
	case QsortStatus::SetPivot: // 1
		//std::cout << "SetPivot | ";
		break;
	case QsortStatus::FindPair: // 2
		//std::cout << "FindPair | ";
		break;
	case QsortStatus::SwapFound: // 3
		//std::cout << "SwapFound | ";
		break;
	case QsortStatus::SetBorders: // 4
		//std::cout << "SetBorders | ";
		break;
	case QsortStatus::Finish:
		//std::cout << "Finish | ";
		break;




	}
}

void SquareSorter::NextStep(std::vector<Square*> &v)
{
	Square* temp = nullptr;
	PrintStatus();
	//std::cout << "subVectors to sort:" << sb.size() << std::endl;
	switch (qSortStatus)
	{
	case QsortStatus::NewSortBorders:// 0
		for (int lb = sb[0].L; lb <= sb[0].R; lb++)
		{
			v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 1.0f));
		}
		qSortStatus = QsortStatus::SetPivot;
		break;
	case QsortStatus::SetPivot: // 1
		i = sb[0].L;
		j = sb[0].R;
		mid = sb[0].L + (sb[0].R - sb[0].L) / 2;
		pivot = v[mid];
		//std::cout << "\tpivot index:" << mid << std::endl;
		pivot->setTarget(Vector2(pivot->GetPosition().x, -1.f));
		qSortStatus = QsortStatus::FindPair;
		break;
	case QsortStatus::FindPair: // 2

		
		
		if ((i < sb[0].R) || (j > sb[0].L))
		{
			while (v[i]->value < pivot->value)
			{
				i++;
			}
			while (v[j]->value > pivot->value)
			{
				j--;
			}
			//std::cout << "\tFirstToSwap:" << i << "\tSecondToSwap:" << j << std::endl;;
			
		}
		if (i <= j)
		{
			qSortStatus = QsortStatus::SwapFound;
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, -1.f));
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, -1.f));
		}
			
		else
			qSortStatus = QsortStatus::SetBorders;
		break;
	case QsortStatus::SwapFound: // 3
		temp = v[i];
		v[i] = v[j];
		v[j] = temp;
		if (v[i] == pivot)
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, -1.f));
		else
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, 1.f));
		if (v[j] == pivot)
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, 1.f));
		else
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, 1.f));
		i++;
		j--;
		qSortStatus = QsortStatus::FindPair;
		break;
	case QsortStatus::SetBorders: // 4
		
		for (int lb = sb[0].L; lb <= sb[0].R; lb++)
		{
			v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 3.0f));
		}

		pivot->setTarget(Vector2(pivot->GetPosition().x, 3.0f));
		//std::cout << '\t' << i << " " << j << " " << sb[0].R << " " << sb[0].L << std::endl;
		if (i < sb[0].R)
			sb.push_back(SortBorders(i, sb[0].R));
		if (j > sb[0].L)
			sb.push_back(SortBorders(sb[0].L, j));

		

		
		
		if (sb.size() > 1)
		{
			qSortStatus = QsortStatus::SetPivot;
			for (int lb = sb[0].L; lb <= sb[0].R; lb++)
			{
			
				v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 3.0f));
			}
		}
			
		else
			qSortStatus = QsortStatus::Finish;
		sb.erase(sb.begin());
		if (sb.size() > 0)
		{
			//std::cout << "\tnextBorders:\t" << sb[0].L << '\t' << sb[0].R << std::endl;
			for (int lb = sb[0].L; lb <= sb[0].R; lb++)
			{
				//std::cout << '\t' << "looking for error" << '\n';
				v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 1.0f));
			}
		}
			

		break;
	case QsortStatus::Finish:
		std::cout << "SORTED!!!" << std::endl;
		break;
	}
}

SquareSorter::SquareSorter()
{
	sb.push_back(SortBorders(0, params::circleAmount - 1));
	//sb[0].L = 0;
	//sb[0].R = params::circleAmount - 1;
	i = sb[0].L;
	j = sb[0].R;
	mid = i + (j - i) / 2;
	pivot = nullptr;
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
		static float delay = 0;
		for (int i = 0; i < params::circleAmount; i++)
		{
			
			squareVector[i]->setTarget(Vector2(-8. + (16. / (float)(params::circleAmount - 1)) * squareVector[i]->currentIndex, squareVector[i]->GetTarget()));
			squareVector[i]->update(dt);
		}
		if (delay > dt * params::frameDelay)
		{
			delay = 0;
			mouseClicked(0.f, 0.f, true);
		}
		else
		{
			delay += dt;
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
			//std::sort(squareVector.begin(), squareVector.end(), MyCompare);
			clicked = !clicked;
		}

			

		for (int i = 0; i < params::circleAmount; i++)
		{
			squareVector[i]->currentIndex = i;
		}

		
	}

	
	
}

