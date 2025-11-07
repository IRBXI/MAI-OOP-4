#include <cmath>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

#include "hexagon.hpp"
#include "octagon.hpp"
#include "pentagon.hpp"
#include "regular_polygon.hpp"
#include "vec2.hpp"
#include "vector.hpp"

namespace lib::geometry {
namespace {

// Helper function to calculate distance between two points
template <typename T>
T distance(const Vec2<T>& a, const Vec2<T>& b) {
    T dx = a.x - b.x;
    T dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Test fixture for RegularPolygon tests
template <typename T>
class RegularPolygonTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Common test vectors
        zero_vec = Vec2<T>{0, 0};
        unit_x = Vec2<T>{1, 0};
        unit_y = Vec2<T>{0, 1};
        diagonal = Vec2<T>{1, 1};
        // Normalize diagonal to have length 1 for fair comparisons
        T diag_len = std::sqrt(static_cast<T>(2));
        unit_diagonal = Vec2<T>{1 / diag_len, 1 / diag_len};
    }

    Vec2<T> zero_vec;
    Vec2<T> unit_x;
    Vec2<T> unit_y;
    Vec2<T> diagonal;
    Vec2<T> unit_diagonal;
};

using FloatTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(RegularPolygonTest, FloatTypes);

// Default Constructor Tests
TYPED_TEST(RegularPolygonTest, DefaultConstructor) {
    RegularPolygon<TypeParam, 5> pentagon;
    Pentagon<TypeParam> pentagon_alias;
    Hexagon<TypeParam> hexagon;
    Octagon<TypeParam> octagon;

    // Default constructed polygons should be valid
    EXPECT_NO_THROW(pentagon.Centroid());
    EXPECT_NO_THROW(static_cast<double>(pentagon));
}

// Constructor with radius_vector as a vertex and guiding vector
TYPED_TEST(RegularPolygonTest, ConstructorWithVertexAndGuidingVector) {
    // radius_vector is one vertex, guiding vector determines orientation and
    // side length
    Vec2<TypeParam> vertex{2, 3};
    Vec2<TypeParam> guide{1, 0}; // Horizontal side, length = 1

    Pentagon<TypeParam> pentagon(vertex, guide);
    Hexagon<TypeParam> hexagon(vertex, guide);
    Octagon<TypeParam> octagon(vertex, guide);

    // The given vertex should be part of the polygon
    EXPECT_NO_THROW(pentagon.Centroid());
    EXPECT_NO_THROW(hexagon.Centroid());
    EXPECT_NO_THROW(octagon.Centroid());

    // All should have positive area
    EXPECT_GT(static_cast<double>(pentagon), 0);
    EXPECT_GT(static_cast<double>(hexagon), 0);
    EXPECT_GT(static_cast<double>(octagon), 0);
}

// Area Calculation Tests - polygons with same side length should have
// predictable area ratios
TYPED_TEST(RegularPolygonTest, AreaComparisonSameSideLength) {
    Vec2<TypeParam> vertex{0, 0};
    // Use unit vectors to ensure same side length
    Vec2<TypeParam> guide{1, 0}; // Unit side length

    Pentagon<TypeParam> pentagon(vertex, guide);
    Hexagon<TypeParam> hexagon(vertex, guide);
    Octagon<TypeParam> octagon(vertex, guide);

    double pentagon_area = static_cast<double>(pentagon);
    double hexagon_area = static_cast<double>(hexagon);
    double octagon_area = static_cast<double>(octagon);

    // All areas should be positive
    EXPECT_GT(pentagon_area, 0);
    EXPECT_GT(hexagon_area, 0);
    EXPECT_GT(octagon_area, 0);

    // For regular polygons with same side length, more sides = larger area
    // (approaching circle area as n -> infinity)
    EXPECT_LT(pentagon_area, hexagon_area);
    EXPECT_LT(hexagon_area, octagon_area);
}

// Corrected Orientation Test - account for side length differences
TYPED_TEST(RegularPolygonTest, Orientation) {
    Vec2<TypeParam> vertex{0, 0};

    // Use vectors with same length for fair comparison
    Pentagon<TypeParam> poly1(vertex, this->unit_x);        // length = 1
    Pentagon<TypeParam> poly2(vertex, this->unit_y);        // length = 1
    Pentagon<TypeParam> poly3(vertex, this->unit_diagonal); // length = 1

    // All should have same area (same side length)
    double area1 = static_cast<double>(poly1);
    double area2 = static_cast<double>(poly2);
    double area3 = static_cast<double>(poly3);

    EXPECT_NEAR(area1, area2, 1e-6);
    EXPECT_NEAR(area2, area3, 1e-6);

    // Now test with different lengths to verify area scales correctly
    Vec2<TypeParam> short_guide{0.5, 0}; // length = 0.5
    Vec2<TypeParam> long_guide{2.0, 0};  // length = 2.0

    Pentagon<TypeParam> short_poly(vertex, short_guide);
    Pentagon<TypeParam> long_poly(vertex, long_guide);

    double short_area = static_cast<double>(short_poly);
    double long_area = static_cast<double>(long_poly);

    // Area should scale with square of side length ratio
    // (2.0/0.5)^2 = 16
    EXPECT_NEAR(long_area / short_area, 16.0, 0.1);
}

// Side Length Tests - verify area scales with square of side length
TYPED_TEST(RegularPolygonTest, SideLengthScaling) {
    Vec2<TypeParam> vertex{0, 0};

    // Test multiple side length ratios
    std::vector<std::pair<Vec2<TypeParam>, TypeParam>> test_cases = {
        {Vec2<TypeParam>{0.5, 0}, 0.5}, // side length = 0.5
        {Vec2<TypeParam>{1.0, 0}, 1.0}, // side length = 1.0
        {Vec2<TypeParam>{2.0, 0}, 2.0}, // side length = 2.0
    };

    std::vector<double> areas;
    for (const auto& [guide, expected_length] : test_cases) {
        Pentagon<TypeParam> polygon(vertex, guide);
        areas.push_back(static_cast<double>(polygon));

        // Verify the guiding vector length matches expected
        EXPECT_NEAR(distance(guide, this->zero_vec), expected_length, 1e-6);
    }

    // Verify area scales with square of side length
    // area2/area1 should be (2.0/0.5)^2 = 16
    EXPECT_NEAR(areas[2] / areas[0], 16.0, 0.1);
    // area1/area0 should be (1.0/0.5)^2 = 4
    EXPECT_NEAR(areas[1] / areas[0], 4.0, 0.1);
}

// Centroid Calculation Tests
TYPED_TEST(RegularPolygonTest, CentroidLocation) {
    // For a regular polygon, centroid should be the geometric center
    Vec2<TypeParam> vertex{2, 3};
    Vec2<TypeParam> guide{1, 0};

    Pentagon<TypeParam> pentagon(vertex, guide);
    Hexagon<TypeParam> hexagon(vertex, guide);
    Octagon<TypeParam> octagon(vertex, guide);

    auto pent_centroid = pentagon.Centroid();
    auto hex_centroid = hexagon.Centroid();
    auto oct_centroid = octagon.Centroid();

    // Centroids should not be the same as the initial vertex
    EXPECT_NE(pent_centroid.x, vertex.x);
    EXPECT_NE(pent_centroid.y, vertex.y);
    EXPECT_NE(hex_centroid.x, vertex.x);
    EXPECT_NE(hex_centroid.y, vertex.y);
    EXPECT_NE(oct_centroid.x, vertex.x);
    EXPECT_NE(oct_centroid.y, vertex.y);

    // Centroids should be finite and reasonable
    EXPECT_TRUE(std::isfinite(pent_centroid.x));
    EXPECT_TRUE(std::isfinite(pent_centroid.y));
}

// Using Vector Container with Polygons
TYPED_TEST(RegularPolygonTest, WithVectorContainer) {
    vector::Vector<Pentagon<TypeParam>> pentagon_vector;
    vector::Vector<Hexagon<TypeParam>> hexagon_vector;
    vector::Vector<Octagon<TypeParam>> octagon_vector;

    // Add polygons at different positions with same side length for fair
    // comparison
    for (int i = 0; i < 3; ++i) {
        Vec2<TypeParam> vertex{static_cast<TypeParam>(i),
                               static_cast<TypeParam>(i)};
        pentagon_vector.PushBack(Pentagon<TypeParam>(vertex, this->unit_x));
        hexagon_vector.PushBack(
            Hexagon<TypeParam>(vertex, this->unit_x)); // Same guiding vector
        octagon_vector.PushBack(
            Octagon<TypeParam>(vertex, this->unit_x)); // Same guiding vector
    }

    EXPECT_EQ(pentagon_vector.Size(), 3);
    EXPECT_EQ(hexagon_vector.Size(), 3);
    EXPECT_EQ(octagon_vector.Size(), 3);

    // Test accessing and using elements
    for (std::size_t i = 0; i < pentagon_vector.Size(); ++i) {
        EXPECT_GT(static_cast<double>(pentagon_vector[i]), 0);
        EXPECT_GT(static_cast<double>(hexagon_vector[i]), 0);
        EXPECT_GT(static_cast<double>(octagon_vector[i]), 0);

        auto centroid = pentagon_vector[i].Centroid();
        EXPECT_TRUE(std::isfinite(centroid.x));
        EXPECT_TRUE(std::isfinite(centroid.y));
    }
}

// Polymorphic Behavior with Figure Base Class
TYPED_TEST(RegularPolygonTest, PolymorphicUsage) {
    vector::Vector<std::unique_ptr<Figure<TypeParam>>> figures;

    // Add different polygon types to same vector through base class
    // Use same guiding vector length for fair comparison
    figures.PushBack(
        std::make_unique<Pentagon<TypeParam>>(this->zero_vec, this->unit_x));
    figures.PushBack(
        std::make_unique<Hexagon<TypeParam>>(this->zero_vec, this->unit_x));
    figures.PushBack(
        std::make_unique<Octagon<TypeParam>>(this->zero_vec, this->unit_x));

    double total_area = 0;
    for (std::size_t i = 0; i < figures.Size(); ++i) {
        double area = static_cast<double>(*figures[i]);
        total_area += area;
        EXPECT_GT(area, 0);

        auto centroid = figures[i]->Centroid();
        EXPECT_TRUE(std::isfinite(centroid.x));
        EXPECT_TRUE(std::isfinite(centroid.y));
    }

    EXPECT_GT(total_area, 0);
}

// Test that different guiding vector lengths produce different areas
TYPED_TEST(RegularPolygonTest, DifferentGuidingVectorLengths) {
    Vec2<TypeParam> vertex{0, 0};

    // Test with guiding vectors of different lengths but same direction
    std::vector<Vec2<TypeParam>> guides = {
        Vec2<TypeParam>{0.5, 0}, // length = 0.5
        Vec2<TypeParam>{1.0, 0}, // length = 1.0
        Vec2<TypeParam>{1.5, 0}, // length = 1.5
        Vec2<TypeParam>{2.0, 0}  // length = 2.0
    };

    std::vector<double> areas;
    for (const auto& guide : guides) {
        Pentagon<TypeParam> polygon(vertex, guide);
        areas.push_back(static_cast<double>(polygon));
    }

    // Verify areas are strictly increasing with guiding vector length
    for (size_t i = 1; i < areas.size(); ++i) {
        EXPECT_GT(areas[i], areas[i - 1]);
    }

    // Verify area ratios roughly match squared length ratios
    EXPECT_NEAR(areas[1] / areas[0], 4.0, 0.1);  // (1.0/0.5)^2 = 4
    EXPECT_NEAR(areas[3] / areas[0], 16.0, 0.1); // (2.0/0.5)^2 = 16
}

// Complex Container Operations with consistent side lengths
TYPED_TEST(RegularPolygonTest, ComplexVectorOperations) {
    vector::Vector<Pentagon<TypeParam>> polygons;

    // Create various pentagons with same side length but different
    // positions/orientations
    polygons.PushBack(
        Pentagon<TypeParam>(Vec2<TypeParam>{0, 0}, Vec2<TypeParam>{1, 0}));
    polygons.PushBack(Pentagon<TypeParam>(
        Vec2<TypeParam>{1, 1},
        Vec2<TypeParam>{0, 1})); // Same length, different direction
    polygons.PushBack(Pentagon<TypeParam>(
        Vec2<TypeParam>{2, 2}, Vec2<TypeParam>{1, 0})); // Same as first

    // Test vector operations
    EXPECT_EQ(polygons.Size(), 3);

    // All pentagons should have approximately the same area (same side length)
    double area0 = static_cast<double>(polygons[0]);
    double area1 = static_cast<double>(polygons[1]);
    double area2 = static_cast<double>(polygons[2]);

    EXPECT_NEAR(area0, area1, 1e-6);
    EXPECT_NEAR(area1, area2, 1e-6);
}

// Edge Cases with attention to guiding vector length
TYPED_TEST(RegularPolygonTest, EdgeCases) {
    // Zero guiding vector (should handle gracefully)
    Pentagon<TypeParam> zero_guided(this->zero_vec, this->zero_vec);
    EXPECT_NO_THROW(zero_guided.Centroid());
    EXPECT_NO_THROW(static_cast<double>(zero_guided));

    // Very small guiding vector
    Vec2<TypeParam> tiny_vec{1e-10, 0};
    Pentagon<TypeParam> tiny_polygon(this->zero_vec, tiny_vec);
    double tiny_area = static_cast<double>(tiny_polygon);
    EXPECT_GE(tiny_area, 0);

    // Very large guiding vector
    Vec2<TypeParam> large_vec{1e10, 0};
    Pentagon<TypeParam> large_polygon(this->zero_vec, large_vec);
    double large_area = static_cast<double>(large_polygon);
    EXPECT_GT(large_area, 0);

    // Area should be much larger for the large polygon
    EXPECT_GT(large_area,
              tiny_area * 1e10); // Rough check, not exact due to floating point
}

// Copy and Move Semantics
TYPED_TEST(RegularPolygonTest, CopyMoveSemantics) {
    Pentagon<TypeParam> original(this->diagonal, this->unit_x);
    double original_area = static_cast<double>(original);
    Vec2<TypeParam> original_centroid = original.Centroid();

    // Copy constructor
    Pentagon<TypeParam> copy(original);
    EXPECT_NEAR(static_cast<double>(copy), original_area, 1e-3);
    EXPECT_NEAR(copy.Centroid().x, original_centroid.x, 1e-3);
    EXPECT_NEAR(copy.Centroid().y, original_centroid.y, 1e-3);

    // Copy assignment
    Pentagon<TypeParam> copy_assign;
    copy_assign = original;
    EXPECT_NEAR(static_cast<double>(copy_assign), original_area, 1e-3);
    EXPECT_NEAR(copy_assign.Centroid().x, original_centroid.x, 1e-3);
    EXPECT_NEAR(copy_assign.Centroid().y, original_centroid.y, 1e-3);

    // Move constructor
    Pentagon<TypeParam> moved(std::move(copy));
    EXPECT_NEAR(static_cast<double>(moved), original_area, 1e-3);
    EXPECT_NEAR(moved.Centroid().x, original_centroid.x, 1e-3);
    EXPECT_NEAR(moved.Centroid().y, original_centroid.y, 1e-3);
}

// Test Geometric Properties with proper side length consideration
TYPED_TEST(RegularPolygonTest, GeometricProperties) {
    // For regular polygons with unit side length, we can verify against known
    // formulas

    Vec2<TypeParam> vertex{0, 0};
    Vec2<TypeParam> unit_guide{1, 0}; // Unit side length

    Pentagon<TypeParam> pentagon(vertex, unit_guide);
    Hexagon<TypeParam> hexagon(vertex, unit_guide);
    Octagon<TypeParam> octagon(vertex, unit_guide);

    // Known area formulas for regular polygons with side length = 1:
    // pentagon_area = (1/4)*sqrt(5*(5+2*sqrt(5))) ≈ 1.720
    // hexagon_area = (3*sqrt(3))/2 ≈ 2.598
    // octagon_area = 2*(1+sqrt(2)) ≈ 4.828

    double pent_area = static_cast<double>(pentagon);
    double hex_area = static_cast<double>(hexagon);
    double oct_area = static_cast<double>(octagon);

    // Check approximate values with reasonable tolerance
    EXPECT_NEAR(pent_area, 1.720, 0.01);
    EXPECT_NEAR(hex_area, 2.598, 0.01);
    EXPECT_NEAR(oct_area, 4.828, 0.01);
}

// Test that direction of guiding vector affects orientation but not area when
// length is constant
TYPED_TEST(RegularPolygonTest, DirectionVsLength) {
    Vec2<TypeParam> vertex{0, 0};

    // Different directions but same length
    Vec2<TypeParam> guide1{1, 0};           // Right
    Vec2<TypeParam> guide2{0, 1};           // Up
    Vec2<TypeParam> guide3{-1, 0};          // Left
    Vec2<TypeParam> guide4{0, -1};          // Down
    Vec2<TypeParam> guide5{0.7071, 0.7071}; // 45 degrees (unit length)

    Pentagon<TypeParam> poly1(vertex, guide1);
    Pentagon<TypeParam> poly2(vertex, guide2);
    Pentagon<TypeParam> poly3(vertex, guide3);
    Pentagon<TypeParam> poly4(vertex, guide4);
    Pentagon<TypeParam> poly5(vertex, guide5);

    double area1 = static_cast<double>(poly1);
    double area2 = static_cast<double>(poly2);
    double area3 = static_cast<double>(poly3);
    double area4 = static_cast<double>(poly4);
    double area5 = static_cast<double>(poly5);

    // All should have same area (same side length)
    EXPECT_NEAR(area1, area2, 1e-3);
    EXPECT_NEAR(area2, area3, 1e-3);
    EXPECT_NEAR(area3, area4, 1e-3);
    EXPECT_NEAR(area4, area5, 1e-3);

    // But centroids should be in different positions relative to vertex
    auto centroid1 = poly1.Centroid();
    auto centroid2 = poly2.Centroid();

    EXPECT_NE(centroid1, centroid2);
}

} // namespace
} // namespace lib::geometry
