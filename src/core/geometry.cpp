#include <vector>
#include <stdexcept>

#include "geometry.h"


namespace editor4d {
    Geometry::Geometry(const std::vector<float> &vertices, const std::vector<int> &indices)
    :vertices_(vertices), indices_(indices) {
        Validate();
    }

    Geometry::Geometry(std::vector<float> &&vertices, std::vector<int> &&indices)
    :vertices_(std::move(vertices)), indices_(std::move(indices)) {
        Validate();
    }

    void Geometry::Validate() const {
        if (vertices_.size() % 4 != 0) {
            throw std::runtime_error("vertices vector is not multiple of 4");
        }

        if (indices_.size() % 3 != 0) {
            throw std::runtime_error("indices vector is not multiple of 3");
        }
    }

    std::vector<float>& Geometry::GetVertices() {
        return vertices_;
    }

    std::vector<int>& Geometry::GetIndices() {
        return indices_;
    }

    std::vector<float> Geometry::Project(const Eigen::Matrix<float, 5, 5> &transform) const {
        auto nb_vertices = vertices_.size() / 4;

        // Matrix of column vectors
        // (4 dimension + 1 extra homogeneous dimension)
        Eigen::MatrixXf vertices = Eigen::MatrixXf::Identity(5, nb_vertices);
        for (size_t i = 0; i < vertices_.size(); ++i) {
            vertices(i % 4, i / 4) = vertices_[i];
        }

        auto transformed_vertices = transform * vertices;

        // now doing projection to #D (simply discard 4th dimension)
        std::vector<float> result(nb_vertices * 3, 0);  // 3 is for 3D

        for (size_t i = 0; i < nb_vertices; ++i) {
            result[i * 3 + 0] = transformed_vertices(0, i);
            result[i * 3 + 1] = transformed_vertices(1, i);
            result[i * 3 + 2] = transformed_vertices(2, i);
        }

        return result;
    }

} // namespace editor4d

