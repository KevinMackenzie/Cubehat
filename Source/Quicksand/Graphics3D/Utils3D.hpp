#ifndef UTILS3D_HPP
#define UTILS3D_HPP

//include glm
#include "../3rdParty/glm-0.9.5.2/glm/glm.hpp"

#undef near
#undef far

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

	//first we need a class to represent a plane
	class Plane
	{
	public:
		GLfloat m_fA, m_fB, m_fC, m_fD;

		Plane(void);
		Plane(vec3 p0, vec3 p1, vec3 p2);
		Plane(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
		~Plane(void);

		void Normalize(void);


		void Init(vec3 p0, vec3 p1, vec3 p2);
		void Init(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
		bool Inside(const vec3 &point, const float radius) const;
		bool Inside(const vec3 &point) const;

		GLfloat Distance(const vec3& point) const;
	};


	//this is a frustum class represented as six different planes all facing in
	class Frustum
	{
	public:

		enum Side{ Near, Far, Top, Right, Bottom, Left };

		//the planes of the frustum object
		Plane m_Planes[NumPlanes];

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
		Frustum(void);

		bool Inside(const vec3& point) const;
		bool Inside(const vec3& point, const GLfloat radius) const;
		const Plane& Get(Side side) const { return m_Planes[side]; }
		void SetFOV(GLfloat fov);
		void SetAspect(GLfloat aspect);
		void SetNear(GLfloat nearClip);
		void SetFar(GLfloat farClip);
		void Init(const GLfloat fov, const GLfloat aspect, const GLfloat near, const GLfloat far);

		void Render(void);

		//this reinitializes the class with the data already in it
		void ReInit(void);

	};


}

#endif