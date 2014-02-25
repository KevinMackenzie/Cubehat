#include "stdafx.h"

namespace Quicksand
{


	const vec3& GetTransMatPosition(const mat4& matrix)
	{
		return vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}

	void SetTransMatPosition(mat4* matrix, vec3 position)
	{
		(*matrix)[3][0] = position.x;
		(*matrix)[3][1] = position.y;
		(*matrix)[3][2] = position.z;
	}

	const vec3& GetTransMatDirection(const mat4& matrix)
	{
		mat4 justRot = matrix;
		SetTransMatPosition(&justRot, vec3());
		vec3 forward = vec3((vec4(g_Forward, 0.0f) * justRot));
		return forward;
	}
}