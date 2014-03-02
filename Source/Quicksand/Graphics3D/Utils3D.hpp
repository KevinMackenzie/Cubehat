#ifndef UTILS3D_HPP
#define UTILS3D_HPP

//include glm
#include "../3rdParty/glm-0.9.5.2/glm/glm.hpp"

//get the std::stack
#include <stack>



//a series of utility classes for 3D graphics
//NOTE: all subsystesms MUST BE USING COUNTER CLOCKWISE VERTEX SETS

#define NumPlanes 6
#define PI 3.14159265358979

namespace Quicksand
{
	//we already did this, but just to localize it
	using glm::vec3;
	using glm::vec4;
	using glm::quat;
	using glm::mat3;
	using glm::mat4;

	//first we need a class Cto represent a plane
	class CPlane
	{
	public:
		GLfloat m_fA, m_fB, m_fC, m_fD;

		CPlane(void);
		CPlane(vec3 p0, vec3 p1, vec3 p2);
		CPlane(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
		~CPlane(void);

		void Normalize(void);


		void Init(vec3 p0, vec3 p1, vec3 p2);
		void Init(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
		bool Inside(const vec3 &point, const float radius) const;
		bool Inside(const vec3 &point) const;

		GLfloat Distance(const vec3& point) const;
	};


	//this is a frustum class Crepresented as six different planes all facing in
	class CFrustum
	{
	public:

		enum Side{ Near, Far, Top, Right, Bottom, Left };

		//the planes of the frustum object
		CPlane m_Planes[NumPlanes];

		//the vertices of the near clip plane in camera space
		vec3 m_NearClip[4];
		vec3 m_FarClip[4];

		//the field of view (in radians of cource since they have mathmatical significance)
		GLfloat m_FOV;

		// the aspect radtio of the display ( width / height)
		GLfloat m_Aspect;

		//the distance to the near clipping plane
		GLfloat m_Near;
		GLfloat m_Far;

	public:
		CFrustum(void);

		bool Inside(const vec3& point) const;
		bool Inside(const vec3& point, const GLfloat radius) const;
		const CPlane& Get(Side side) const { return m_Planes[side]; }
		void SetFOV(GLfloat fov);
		void SetAspect(GLfloat aspect);
		void SetNear(GLfloat nearClip);
		void SetFar(GLfloat farClip);
		void Init(const GLfloat fov, const GLfloat aspect, const GLfloat nearClipDistance, const GLfloat farClipDistance);

		void Render(void);

		//this reinitializes the class Cwith the data already in it
		void ReInit(void);

	};


	typedef std::stack<mat4> MatrixStack4;

	//this is a matrix stack to help with hierarchal scenes
	//this replaces the openGL deprecated matrix stack
	//this is a simple data structure that i proudly say is all my work
	class CGLMatrixStack
	{
		MatrixStack4 m_MatrixStack;
		
		//this turns out to be very useful, this is the element of the stack that is one from the top
		mat4 *m_OneFromTop;

	public:

		CGLMatrixStack(void);
		~CGLMatrixStack(void);

		void Push(void);//adds to the stack
		void Pop(void);//pops the stack
		void MultMatrix(mat4 matrix);//multiplies the top matrix by the parameter
		void LoadMatrix(mat4 matrix);//overwrites the top matrix with this matrix
		void LoadIdentity(void);//makes the top of the stack the the second-to-top level of the stack
		const mat4& GetTop(void) const;//returns the top of the stack (full concatinated toworld matrix)


		//NOTE:
		//the top level matrix is the resault of multiplying the input matrix by the 
		//matrix in the level below it.  The bottom is ALWAYS the identity, and 
		//i make sure of that.  This creates a system that is very fast,
		//and versatile.  it only does the costly multiplication once

	};

}

#endif