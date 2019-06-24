#include <array>

#include <Eigen/Core>


namespace editor4d {
    const size_t NB_ROTATIONS_PLANES = 6;
    const size_t X = 0;
    const size_t Y = 1;
    const size_t Z = 2;
    const size_t W = 3;

    const size_t FIRST_PLANE[NB_ROTATIONS_PLANES]  = {X, X, X, Y, Y, Z};
    const size_t SECOND_PLANE[NB_ROTATIONS_PLANES] = {Y, Z, W, Z, W, W};
    // 4d rotation's axis planes ids

    float ConvertDegreeToRadians(float degree);

    Eigen::Matrix<float, 4, 4> Perspective(float fovy, float aspect, float zNear, float zFar);

    Eigen::Matrix<float, 4, 4> LookAt(const Eigen::Vector3f &eye,
                                      const Eigen::Vector3f &center,
                                      const Eigen::Vector3f &up);

    // Create 4d Rotation matrix (4 dimension + 1 extra homogeneous dimension).
    // Arguments are angles representing rotations in axis planes (in radians).
    Eigen::Matrix<float, 5, 5> Rotate4d(std::array<float, NB_ROTATIONS_PLANES> angles);
} // namespace editor4d

