#include <array>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "util.h"


namespace editor4d {
    float ConvertDegreeToRadians(float degree) {
        return float(degree * M_PI / 180.0);
    }

    // implementation close to glm,
    // taken from here http://spointeau.blogspot.com/2013/12/hello-i-am-looking-at-opengl-3.html
    Eigen::Matrix<float, 4, 4> Perspective(float fovy, float aspect, float zNear, float zFar) {
        float radians = ConvertDegreeToRadians(fovy);
	float tan_half_fovy = tan(radians / 2.0f);

        Eigen::Matrix4f result = Eigen::Matrix4f::Zero();
	result(0, 0) = 1.0f / (aspect * tan_half_fovy);
	result(1, 1) = 1.0f / (tan_half_fovy);
	result(2, 2) = -(zFar + zNear) / (zFar - zNear);
	result(3, 2) = -1.0f;
	result(2, 3) = -(2.0f * zFar * zNear) / (zFar - zNear);

	return result; 
    }

    // implementation close to glm,
    // taken from here http://spointeau.blogspot.com/2013/12/hello-i-am-looking-at-opengl-3.html
    Eigen::Matrix<float, 4, 4> LookAt(const Eigen::Vector3f &eye,
                                      const Eigen::Vector3f &center,
                                      const Eigen::Vector3f &up) {
        Eigen::Vector3f f = (center - eye).normalized();
        Eigen::Vector3f u = up.normalized();
        Eigen::Vector3f s = f.cross(u).normalized();
        u = s.cross(f);

        Eigen::Matrix4f result;
        result <<  s.x(),  s.y(),  s.z(), -s.dot(eye),
                   u.x(),  u.y(),  u.z(), -u.dot(eye),
                  -f.x(), -f.y(), -f.z(),  f.dot(eye),
                    0.0f,   0.0f,   0.0f,  1.0f;

        return result;    
    }

    Eigen::Matrix<float, 5, 5> Rotate4d(std::array<float, NB_ROTATIONS_PLANES> angles) {
        Eigen::Matrix<float, 5, 5> result = Eigen::Matrix<float, 5, 5>::Identity();

        for (size_t idx = 0; idx < NB_ROTATIONS_PLANES; ++idx) {
            auto i = FIRST_PLANE[idx];
            auto j = SECOND_PLANE[idx];

            Eigen::Matrix<float, 5, 5> rotation = Eigen::Matrix<float, 5, 5>::Identity();
            rotation(i, i) = cos(angles[idx]);
            rotation(j, j) = cos(angles[idx]);
            rotation(i, j) = sin(angles[idx]);
            rotation(j, i) = -sin(angles[idx]);

            result = rotation * result;
        }

        return result;
    }

} // namespace editor4d

