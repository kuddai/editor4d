#include <iostream>
#include <cmath>

#include <catch.hpp>
#include <Eigen/Core>

#include "util.h"


TEST_CASE("Rotate4d", "geometry") {
    auto result = editor4d::Rotate4d({float(M_PI), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
    Eigen::Matrix<float, 5, 5> expected;

    expected << -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f,  0.0f, 0.0f, 0.0f, 1.0f;

    REQUIRE(result.isApprox(expected, 1.0e-07));
}
