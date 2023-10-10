

//-------------------------------------------------------
//	user interface
//-------------------------------------------------------

namespace scene
{
	class Mesh;

	Mesh *createShipMesh();
	void destroyMesh( Mesh *mesh );
	void placeMesh( Mesh *mesh, float x, float y, float angle );

	void screenToWorld( float *x, float *y );

}


//-------------------------------------------------------
//	engine only interface
//-------------------------------------------------------

namespace scene
{
	void update( float dt );
	void draw();
}
