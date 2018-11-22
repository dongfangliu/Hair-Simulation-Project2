#pragma once
#include "common.h"

glm::vec3 static evalBeizer(const std::vector<glm::vec3>& P, const float & t) {
	if (P.size()==2) {
			return (1 - t)*P[0] + t*P[1];
		
	}else {
		std::vector<glm::vec3> out_Points(P.size() - 1);
		glm::vec3 P1, P2;
		for (int i = 0; i <out_Points.size(); i++) {
			P1 = P[i]; P2 = P[i + 1];
			out_Points[i] = (1 - t)*P1 + t*P2;
		}
		return evalBeizer(out_Points, t);
	}
}
glm::vec3 static spline(const std::vector<glm::vec3>& cp, float t)
{
	//// indices of the relevant control points
	//int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	//int i1 = glm::clamp<int>(t, 0, cp.size() - 1);
	//int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	//int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	//// parameter on the local curve interval
	//float local_t = (t - i0) / (i3 - i0);//glm::fract(t);

	////return glm::hermite(cp[i0], cp[i1], cp[i2], cp[i3], local_t);
	//return cp[i0] * (1 - local_t)* (1 - local_t)* (1 - local_t) + cp[i1] * 3.0f*local_t* (1 - local_t)* (1 - local_t) + cp[i2] * 3.0f*local_t*local_t*(1 - local_t) + cp[i3] * local_t* local_t* local_t;
	return evalBeizer(cp, (t) / (cp.size() - 1));
}

