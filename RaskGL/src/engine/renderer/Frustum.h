#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp> // optional
#include <array>
#include <iostream>

struct Plane {
	glm::vec3 normal;
	float d;
};
struct Frustum {
	Plane planes[6]; // left, right, bottom, top, near, far
};

inline Plane MakePlaneFromPoints(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& frustumCenter)
{
	Plane p;
	glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));
	if (glm::dot(n, frustumCenter - a) < 0.0f) n = -n;
	p.normal = n;
	p.d = -glm::dot(n, a);
	return p;
}

inline Frustum ExtractFrustumFromVP(const glm::mat4& vp)
{
	glm::mat4 inv = glm::inverse(vp);
	std::array<glm::vec4, 8> ndc = {
		glm::vec4(-1, -1, -1, 1), // 0 near bottom-left
		glm::vec4(1, -1, -1, 1), // 1 near bottom-right
		glm::vec4(1,  1, -1, 1), // 2 near top-right
		glm::vec4(-1,  1, -1, 1), // 3 near top-left
		glm::vec4(-1, -1,  1, 1), // 4 far  bottom-left
		glm::vec4(1, -1,  1, 1), // 5 far  bottom-right
		glm::vec4(1,  1,  1, 1), // 6 far  top-right
		glm::vec4(-1,  1,  1, 1)  // 7 far  top-left
	};

	std::array<glm::vec3, 8> p;
	for (int i = 0; i < 8; ++i) {
		glm::vec4 w = inv * ndc[i];
		p[i] = glm::vec3(w) / w.w; // perspective divide
	}

	// Compute frustum center (for plane orientation)
	glm::vec3 center(0.0f);
	for (int i = 0; i < 8; ++i) center += p[i];
	center /= 8.0f;

	Frustum f;
	// Choose three points on each face
	f.planes[0] = MakePlaneFromPoints(p[3], p[0], p[4], center); // Left  (near top-left, near bottom-left, far bottom-left)
	f.planes[1] = MakePlaneFromPoints(p[1], p[2], p[6], center); // Right (near bottom-right, near top-right, far top-right)

	f.planes[2] = MakePlaneFromPoints(p[0], p[1], p[5], center); // Bottom (near bl, near br, far br)
	f.planes[3] = MakePlaneFromPoints(p[2], p[3], p[7], center); // Top    (near tr, near tl, far tl)

	f.planes[4] = MakePlaneFromPoints(p[0], p[3], p[2], center); // Near   (near bl, near tl, near tr)
	f.planes[5] = MakePlaneFromPoints(p[5], p[6], p[7], center); // Far    (far br, far tr, far tl)

	return f;
}

inline std::array<glm::vec3, 8> ExtractFrustumCorners(const glm::mat4& vp) {
	glm::mat4 invVP = glm::inverse(vp);
	std::array<glm::vec3, 8> corners;

	int i = 0;
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			for (int z = -1; z <= 1; z += 2) {
				glm::vec4 pt = invVP * glm::vec4((float)x, (float)y, (float)z, 1.0f);
				pt /= pt.w;
				corners[i++] = glm::vec3(pt);
			}
		}
	}
	return corners;
}

inline bool SphereInFrustum(const Frustum& frustum, const glm::vec3& center, float radius)
{
	for (int i = 0; i < 6; ++i) {
		float dist = glm::dot(frustum.planes[i].normal, center) + frustum.planes[i].d;
		if (dist < -radius) return false;
	}
	return true;
}
