#pragma once

#include "Plane.h"

struct Frustum {
	Plane planes[6]; // left, right, bottom, top, near, far
};

inline Frustum ExtractFrustum(const glm::mat4& vp) {
	Frustum frustum;

	// Left
	frustum.planes[0].normal.x = vp[0][3] + vp[0][0];
	frustum.planes[0].normal.y = vp[1][3] + vp[1][0];
	frustum.planes[0].normal.z = vp[2][3] + vp[2][0];
	frustum.planes[0].d = vp[3][3] + vp[3][0];

	// Right
	frustum.planes[1].normal.x = vp[0][3] - vp[0][0];
	frustum.planes[1].normal.y = vp[1][3] - vp[1][0];
	frustum.planes[1].normal.z = vp[2][3] - vp[2][0];
	frustum.planes[1].d = vp[3][3] - vp[3][0];

	// Bottom
	frustum.planes[2].normal.x = vp[0][3] + vp[0][1];
	frustum.planes[2].normal.y = vp[1][3] + vp[1][1];
	frustum.planes[2].normal.z = vp[2][3] + vp[2][1];
	frustum.planes[2].d = vp[3][3] + vp[3][1];

	// Top
	frustum.planes[3].normal.x = vp[0][3] - vp[0][1];
	frustum.planes[3].normal.y = vp[1][3] - vp[1][1];
	frustum.planes[3].normal.z = vp[2][3] - vp[2][1];
	frustum.planes[3].d = vp[3][3] - vp[3][1];

	// Near
	frustum.planes[4].normal.x = vp[0][3] + vp[0][2];
	frustum.planes[4].normal.y = vp[1][3] + vp[1][2];
	frustum.planes[4].normal.z = vp[2][3] + vp[2][2];
	frustum.planes[4].d = vp[3][3] + vp[3][2];

	// Far
	frustum.planes[5].normal.x = vp[0][3] - vp[0][2];
	frustum.planes[5].normal.y = vp[1][3] - vp[1][2];
	frustum.planes[5].normal.z = vp[2][3] - vp[2][2];
	frustum.planes[5].d = vp[3][3] - vp[3][2];

	// Normalize planes
	for (int i = 0; i < 6; i++) {
		float length = glm::length(frustum.planes[i].normal);
		frustum.planes[i].normal /= length;
		frustum.planes[i].d /= length;
	}

	return frustum;
}

inline bool SphereInFrustum(const Frustum& frustum, const glm::vec3& center, float radius) {
	for (int i = 0; i < 6; i++) {
		float distance = glm::dot(frustum.planes[i].normal, center) + frustum.planes[i].d;
		if (distance < -radius) {
			return false; // outside
		}
	}
	return true; // inside or intersecting
}