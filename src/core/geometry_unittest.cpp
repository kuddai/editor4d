#include <vector>
#include <stdexcept>

#include <catch.hpp>
#include <Eigen/Dense>

#include "geometry.h"


bool AreVectorsEqual(std::vector<float> first,
                     std::vector<float> second) {
    if (first.size() != second.size()) {
        return false;
    }

    auto size = first.size();
    for (size_t i = 0; i < size; ++i) {
        if (first[i] != Approx(second[i])) {
            return false;
        }
    }
    return true;
}

TEST_CASE("vertices validation", "geometry") {
    std::vector<float> vertices = {
       0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 5th component - error!
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f
    };
    std::vector<int> indices = {0, 1, 2};

    REQUIRE_THROWS(editor4d::Geometry(vertices, indices));
}

TEST_CASE("indices validation", "geometry") {
    std::vector<float> vertices = {
       0.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f
    };
    std::vector<int> indices = {0, 1};  // not enough indices to form triangle - error!

    REQUIRE_THROWS(editor4d::Geometry(vertices, indices));
}

TEST_CASE("3d projection", "geometry") {
    std::vector<float> vertices = {
       0.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f
    };
    std::vector<int> indices = {0, 1, 2};
    editor4d::Geometry triangle(vertices, indices);

    // homogeneous transform - rotation, translation, scaling,
    // but for 4d (that is why 5x5 - 4d + 1 extra homogeneous dimension)
    auto transform = Eigen::Matrix<float, 5, 5>::Identity();

    // perform transform and remove 4th dimension
    std::vector<float> result = triangle.Project(transform);
    std::vector<float> expected = {
       0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 1.0f
    };

    REQUIRE(AreVectorsEqual(result, expected));
}

