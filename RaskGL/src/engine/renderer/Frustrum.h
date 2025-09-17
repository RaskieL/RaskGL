#pragma once

#include "Plane.h"

struct Frustum {
	Plane planes[6]; // left, right, bottom, top, near, far
};

inline Frustum ExtractFrustum(const glm::mat4& vp) {
	Frustum f;

	// Left
	f.planes[0].normal.x = vp[3][0] + vp[0][0];
	f.planes[0].normal.y = vp[3][1] + vp[0][1];
	f.planes[0].normal.z = vp[3][2] + vp[0][2];
	f.planes[0].d = vp[3][3] + vp[0][3];

	// Right
	f.planes[1].normal.x = vp[3][0] - vp[0][0];
	f.planes[1].normal.y = vp[3][1] - vp[0][1];
	f.planes[1].normal.z = vp[3][2] - vp[0][2];
	f.planes[1].d = vp[3][3] - vp[0][3];

	// Bottom
	f.planes[2].normal.x = vp[3][0] + vp[1][0];
	f.planes[2].normal.y = vp[3][1] + vp[1][1];
	f.planes[2].normal.z = vp[3][2] + vp[1][2];
	f.planes[2].d = vp[3][3] + vp[1][3];

	// Top
	f.planes[3].normal.x = vp[3][0] - vp[1][0];
	f.planes[3].normal.y = vp[3][1] - vp[1][1];
	f.planes[3].normal.z = vp[3][2] - vp[1][2];
	f.planes[3].d = vp[3][3] - vp[1][3];

	// Near
	f.planes[4].normal.x = vp[3][0] + vp[2][0];
	f.planes[4].normal.y = vp[3][1] + vp[2][1];
	f.planes[4].normal.z = vp[3][2] + vp[2][2];
	f.planes[4].d = vp[3][3] + vp[2][3];

	// Far
	f.planes[5].normal.x = vp[3][0] - vp[2][0];
	f.planes[5].normal.y = vp[3][1] - vp[2][1];
	f.planes[5].normal.z = vp[3][2] - vp[2][2];
	f.planes[5].d = vp[3][3] - vp[2][3];

	// Normalize planes
	for (int i = 0; i < 6; i++) {
		float len = glm::length(f.planes[i].normal);
		f.planes[i].normal /= len;
		f.planes[i].d /= len;
	}

	return f;
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