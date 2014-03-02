#include "stdafx.h"
#include "Utils3D.hpp"
#include <cmath>


namespace Quicksand
{
	CPlane::CPlane(vec3 p0, vec3 p1, vec3 p2)
	{
		Init(p0, p1, p2);
	}

	CPlane::CPlane(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
	{
		Init(a, b, c, d);
	}

	CPlane::CPlane(void)
	{
		m_fA = 0.0f;
		m_fB = 0.0f;
		m_fC = 0.0f;
		m_fD = 0.0f;
	}

	CPlane::~CPlane(void)
	{

	}

	void CPlane::Init(vec3 p0, vec3 p1, vec3 p2)
	{
		vec3 v, u, n;


		//get the vector ABCD, we get the point in the middle of this plane with the given points
		v = p1 - p0;
		u = p2 - p0;

		//get the final vector
		n = glm::cross(v, u);

		//to get the D value, take the dot product of the opposite of the 
		//plane vector, and  a point on the plane
		Init(n.x, n.y, n.z, glm::dot(-n, p0));

	}

	void CPlane::Init(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
	{
		m_fA = a;
		m_fB = b;
		m_fC = c;
		m_fD = d;
	}

	void CPlane::Normalize(void)
	{
		float mag;
		mag = sqrt(m_fA*m_fA + m_fB*m_fB + m_fC*m_fC);
		m_fA /= mag;
		m_fB /= mag;
		m_fC /= mag;
	}

	bool CPlane::Inside(const vec3& point, const float radius) const
	{
		//if Distance < -radius, we are outside

		GLfloat fDistance;

		fDistance = Distance(point);

		return (fDistance >= -radius);
	}

	bool CPlane::Inside(const vec3& point) const
	{
		//the point is inside the plane if it is being faced by the normal of the plane
		//NOTE: if it is flush with the plane, it is inside
		return (Distance(point) >= 0.0f);
	}

	GLfloat CPlane::Distance(const vec3& point) const
	{
		return glm::dot(vec3(m_fA, m_fB, m_fC), point) + m_fD;
	}


	CFrustum::CFrustum(void)
	{
		m_FOV = PI / 4.0f;		//default FOV is pi/4
		m_Aspect = 1;			//default is square
		m_Near = 1.0f;			//default is one unit away from 'eye'
		m_Far = 1000.0f;		//default is one thousand units away from 'eye'

	}

	void CFrustum::ReInit(void)
	{
		Init(m_FOV, m_Aspect, m_Near, m_Far);
	}

	bool CFrustum::Inside(const vec3& point) const
	{
		for (int i = 0; i < NumPlanes; i++)
		{
			if (!m_Planes[i].Inside(point))
				return false;
		}
		
		//if they all report indside, return true
		return true;
	}

	bool CFrustum::Inside(const vec3& point, const GLfloat radius) const
	{
		for (int i = 0; i < NumPlanes; i++)
		{
			if (!m_Planes[i].Inside(point, radius))
				return false;
		}

		//if they all report indside, return true
		return true;
	}

	void CFrustum::SetFOV(GLfloat fov)
	{
		m_FOV = fov;
		ReInit();
	}

	void CFrustum::SetAspect(GLfloat aspect)
	{
		m_Aspect = aspect;
		ReInit();
	}

	void CFrustum::SetNear(GLfloat nearClip)
	{
		m_Near = nearClip;
		ReInit();
	}

	void CFrustum::SetFar(GLfloat farClip)
	{
		m_Far = farClip;
		ReInit();
	}

	void CFrustum::Init(const GLfloat fov, const GLfloat aspect, const GLfloat nearClipDistance, const GLfloat farClipDistance)
	{
		m_FOV = fov;
		m_Aspect = aspect;
		m_Near = nearClipDistance;
		m_Far = farClipDistance;


		//get a set of points
		GLfloat tanFovOver2 = tan(m_FOV / 2.0f);
		vec3 nearRight = (m_Near * tanFovOver2) * m_Aspect * g_Right;
		vec3 farRight = (m_Far * tanFovOver2) * m_Aspect * g_Right;
		vec3 nearUp = (m_Near * tanFovOver2) * g_Up;
		vec3 farUp = (m_Far * tanFovOver2) * g_Up;


		//points start in the upper right and go around CCW
		m_NearClip[0] = (m_Near * g_Forward) - nearRight + nearUp;
		m_NearClip[1] = (m_Near * g_Forward) + nearRight + nearUp;
		m_NearClip[2] = (m_Near * g_Forward) + nearRight - nearUp;
		m_NearClip[3] = (m_Near * g_Forward) - nearRight - nearUp;

		m_FarClip[0] = (m_Far * g_Forward) - farRight + farUp;
		m_FarClip[1] = (m_Far * g_Forward) + farRight + farUp;
		m_FarClip[2] = (m_Far * g_Forward) + farRight - farUp;
		m_FarClip[3] = (m_Far * g_Forward) - farRight - farUp;


		//after calculating all eight points for the frustum, 
		//we can now construct the six planes

		vec3 origin;
		m_Planes[Near]	.Init(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
		m_Planes[Far]	.Init(m_FarClip[0],  m_FarClip[1],  m_FarClip[2]);
		m_Planes[Right]	.Init(m_FarClip[2],  m_FarClip[1],  origin);
		m_Planes[Top]	.Init(m_FarClip[1],  m_FarClip[0],  origin);
		m_Planes[Left]	.Init(m_FarClip[0],  m_FarClip[3],  origin);
		m_Planes[Bottom].Init(m_FarClip[3],  m_FarClip[2],  origin);
	}


	void CFrustum::Render(void)
	{

	}


	//CGLMatrixStack
	//////////////////////////////////////////////

	CGLMatrixStack::CGLMatrixStack(void)
	{
		//add one to the stack to begin with to have fewer issues
		m_MatrixStack.push(mat4());
	}

	CGLMatrixStack::~CGLMatrixStack(void)
	{
		while (m_MatrixStack.size() != 0)
		{
			m_MatrixStack.pop();
		}
	}


	void CGLMatrixStack::Push(void)
	{
		
		m_OneFromTop = &m_MatrixStack.top();
		m_MatrixStack.push(m_MatrixStack.top());
		
	}

	void CGLMatrixStack::Pop(void)
	{
		//make sure we leave that one top level identity matrix
		if (m_MatrixStack.size() == 1)
			return;

		//to keep the m_OneFromTop working, we have to do something a little fancy
		
		//first pop the top
		m_MatrixStack.pop();

		//next get the top level matrix
		mat4 tmp = m_MatrixStack.top();

		//pop the top again
		m_MatrixStack.pop();

		//get a reference to the top
		m_OneFromTop = &m_MatrixStack.top();

		//now put the tmp matrix back on
		m_MatrixStack.push(tmp);
	}

	void CGLMatrixStack::LoadMatrix(mat4 matrix)
	{
		m_MatrixStack.top() = matrix * (*m_OneFromTop);
	}

	void CGLMatrixStack::LoadIdentity(void)
	{
		m_MatrixStack.top() = *m_OneFromTop;
	}

	void CGLMatrixStack::MultMatrix(mat4 matrix)
	{
		m_MatrixStack.top() *= matrix;
	}

	const mat4& CGLMatrixStack::GetTop(void) const
	{
		return m_MatrixStack.top();
	}


}