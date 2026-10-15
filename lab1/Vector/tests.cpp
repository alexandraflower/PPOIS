#include <UnitTest++/UnitTest++.h>
#include "Vector3D.hpp"
#include <sstream>
#include <stdexcept>

SUITE(Vector3DTestSuite) {

    TEST(01_DefaultConstructor_ZerosAllCoordinates) {
        Vector3D v;
        CHECK_CLOSE(0.0, v.getX1(), 1e-9);
        CHECK_CLOSE(0.0, v.getY1(), 1e-9);
        CHECK_CLOSE(0.0, v.getZ1(), 1e-9);
        CHECK_CLOSE(0.0, v.getX2(), 1e-9);
        CHECK_CLOSE(0.0, v.getY2(), 1e-9);
        CHECK_CLOSE(0.0, v.getZ2(), 1e-9);
    }

    TEST(02_ParameterizedConstructor_PositiveCoordinates) {
        Vector3D v(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        CHECK_CLOSE(1.0, v.getX1(), 1e-9);
        CHECK_CLOSE(2.0, v.getY1(), 1e-9);
        CHECK_CLOSE(3.0, v.getZ1(), 1e-9);
        CHECK_CLOSE(4.0, v.getX2(), 1e-9);
        CHECK_CLOSE(5.0, v.getY2(), 1e-9);
        CHECK_CLOSE(6.0, v.getZ2(), 1e-9);
    }

    TEST(03_ParameterizedConstructor_NegativeCoordinates) {
        Vector3D v(-5.0, -10.0, -15.0, -2.0, -4.0, -6.0);
        CHECK_CLOSE(-5.0, v.getX1(), 1e-9);
        CHECK_CLOSE(-10.0, v.getY1(), 1e-9);
        CHECK_CLOSE(-15.0, v.getZ1(), 1e-9);
        CHECK_CLOSE(-2.0, v.getX2(), 1e-9);
        CHECK_CLOSE(-4.0, v.getY2(), 1e-9);
        CHECK_CLOSE(-6.0, v.getZ2(), 1e-9);
    }

    TEST(04_CopyConstructor) {
        Vector3D v1(1.5, 2.5, 3.5, 4.5, 5.5, 6.5);
        Vector3D v2 = v1;
        CHECK_CLOSE(v1.getX1(), v2.getX1(), 1e-9);
        CHECK_CLOSE(v1.getY1(), v2.getY1(), 1e-9);
        CHECK_CLOSE(v1.getZ1(), v2.getZ1(), 1e-9);
        CHECK_CLOSE(v1.getX2(), v2.getX2(), 1e-9);
        CHECK_CLOSE(v1.getY2(), v2.getY2(), 1e-9);
        CHECK_CLOSE(v1.getZ2(), v2.getZ2(), 1e-9);
    }

    TEST(05_AssignmentOperator_ValuesAreIdentical) {
        Vector3D v1(2.0, 4.0, 6.0, 8.0, 10.0, 12.0);
        Vector3D v2;
        v2 = v1;
        CHECK_CLOSE(2.0, v2.getX1(), 1e-9);
        CHECK_CLOSE(4.0, v2.getY1(), 1e-9);
        CHECK_CLOSE(6.0, v2.getZ1(), 1e-9);
        CHECK_CLOSE(8.0, v2.getX2(), 1e-9);
        CHECK_CLOSE(10.0, v2.getY2(), 1e-9);
        CHECK_CLOSE(12.0, v2.getZ2(), 1e-9);
    }

    TEST(06_Projections_dx_dy_dz) {
        Vector3D v(1.0, 2.0, 3.0, 5.0, 8.0, 2.0);
        CHECK_CLOSE(4.0, v.dx(), 1e-9); 
        CHECK_CLOSE(6.0, v.dy(), 1e-9);  
        CHECK_CLOSE(-1.0, v.dz(), 1e-9); 
    }

    TEST(07_Length_ZeroVectorIsZero) {
        Vector3D v(2.0, 3.0, 4.0, 2.0, 3.0, 4.0);
        CHECK_CLOSE(0.0, v.length(), 1e-9);
    }

    TEST(08_Length_StandardPositiveValues) {
        Vector3D v(0.0, 0.0, 0.0, 3.0, 4.0, 0.0);
        CHECK_CLOSE(5.0, v.length(), 1e-9);
    }

    TEST(09_Length_WithNegativeComponents) {
        Vector3D v(0.0, 0.0, 0.0, -2.0, -3.0, -6.0);
        CHECK_CLOSE(7.0, v.length(), 1e-9);
    }

    TEST(10_OperatorPlus_ReturnsCorrectSum) {
        Vector3D v1(1.0, 1.0, 1.0, 3.0, 4.0, 5.0); 
        Vector3D v2(0.0, 0.0, 0.0, 1.0, 2.0, 1.0); 
        Vector3D res = v1 + v2;

        CHECK_CLOSE(1.0, res.getX1(), 1e-9);
        CHECK_CLOSE(1.0, res.getY1(), 1e-9);
        CHECK_CLOSE(1.0, res.getZ1(), 1e-9);
        CHECK_CLOSE(3.0, res.dx(), 1e-9);
        CHECK_CLOSE(5.0, res.dy(), 1e-9);
        CHECK_CLOSE(5.0, res.dz(), 1e-9);
    }

    TEST(11_OperatorPlusEqual_ModifiesLhs) {
        Vector3D v1(0.0, 0.0, 0.0, 2.0, 2.0, 2.0); 
        Vector3D v2(5.0, 5.0, 5.0, 6.0, 7.0, 8.0); 
        v1 += v2;

        CHECK_CLOSE(3.0, v1.dx(), 1e-9);
        CHECK_CLOSE(4.0, v1.dy(), 1e-9);
        CHECK_CLOSE(5.0, v1.dz(), 1e-9);
    }

    TEST(12_OperatorMinus_ReturnsCorrectDifference) {
        Vector3D v1(0.0, 0.0, 0.0, 10.0, 12.0, 14.0); 
        Vector3D v2(0.0, 0.0, 0.0, 3.0, 5.0, 7.0);    
        Vector3D res = v1 - v2;

        CHECK_CLOSE(7.0, res.dx(), 1e-9);
        CHECK_CLOSE(7.0, res.dy(), 1e-9);
        CHECK_CLOSE(7.0, res.dz(), 1e-9);
    }

    TEST(13_OperatorMinusEqual_ModifiesLhs) {
        Vector3D v1(1.0, 1.0, 1.0, 5.0, 5.0, 5.0); 
        Vector3D v2(0.0, 0.0, 0.0, 1.0, 2.0, 3.0); 
        v1 -= v2;

        CHECK_CLOSE(3.0, v1.dx(), 1e-9);
        CHECK_CLOSE(2.0, v1.dy(), 1e-9);
        CHECK_CLOSE(1.0, v1.dz(), 1e-9);
    }

    TEST(14_ScalarProduct_StandardVectors) {
        Vector3D v1(0.0, 0.0, 0.0, 1.0, 2.0, 3.0);
        Vector3D v2(0.0, 0.0, 0.0, 4.0, 5.0, 6.0);
        CHECK_CLOSE(32.0, v1 * v2, 1e-9);
    }

    TEST(15_ScalarProduct_OrthogonalVectors_IsZero) {
        Vector3D v1(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        CHECK_CLOSE(0.0, v1 * v2, 1e-9);
    }

    TEST(16_CaretOperator_CollinearVectors_CosineIsOne) {
        Vector3D v1(0.0, 0.0, 0.0, 2.0, 0.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, 5.0, 0.0, 0.0);
        CHECK_CLOSE(1.0, v1 ^ v2, 1e-9);
    }

    TEST(17_CaretOperator_OppositeVectors_CosineIsMinusOne) {
        Vector3D v1(0.0, 0.0, 0.0, 3.0, 0.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, -4.0, 0.0, 0.0);
        CHECK_CLOSE(-1.0, v1 ^ v2, 1e-9);
    }

    TEST(18_CaretOperator_ThrowsOnZeroVector) {
        Vector3D v1(0.0, 0.0, 0.0, 1.0, 2.0, 3.0);
        Vector3D zeroVec;
        CHECK_THROW(v1 ^ zeroVec, std::invalid_argument);
        CHECK_THROW(zeroVec ^ v1, std::invalid_argument);
    }

    TEST(19_MultiplyByPositiveScalar) {
        Vector3D v(1.0, 1.0, 1.0, 3.0, 4.0, 5.0); 
        Vector3D res = v * 3.0;

        CHECK_CLOSE(6.0, res.dx(), 1e-9);
        CHECK_CLOSE(9.0, res.dy(), 1e-9);
        CHECK_CLOSE(12.0, res.dz(), 1e-9);
    }

    TEST(20_MultiplyByNegativeScalar) {
        Vector3D v(0.0, 0.0, 0.0, 2.0, -3.0, 4.0);
        Vector3D res = v * (-2.0);

        CHECK_CLOSE(-4.0, res.dx(), 1e-9);
        CHECK_CLOSE(6.0, res.dy(), 1e-9);
        CHECK_CLOSE(-8.0, res.dz(), 1e-9);
    }

    TEST(21_MultiplyByZeroScalar) {
        Vector3D v(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        Vector3D res = v * 0.0;

        CHECK_CLOSE(0.0, res.dx(), 1e-9);
        CHECK_CLOSE(0.0, res.dy(), 1e-9);
        CHECK_CLOSE(0.0, res.dz(), 1e-9);
        CHECK_CLOSE(0.0, res.length(), 1e-9);
    }

    TEST(22_ScalarMultiplication_CommutativeOperator) {
        Vector3D v(0.0, 0.0, 0.0, 2.5, 3.5, 4.5);
        Vector3D res1 = v * 4.0;
        Vector3D res2 = 4.0 * v;

        CHECK_CLOSE(res1.dx(), res2.dx(), 1e-9);
        CHECK_CLOSE(res1.dy(), res2.dy(), 1e-9);
        CHECK_CLOSE(res1.dz(), res2.dz(), 1e-9);
    }

    TEST(23_OperatorMultiplyEqual_ModifiesLhs) {
        Vector3D v(2.0, 2.0, 2.0, 4.0, 5.0, 6.0); 
        v *= 2.5;

        CHECK_CLOSE(5.0, v.dx(), 1e-9);
        CHECK_CLOSE(7.5, v.dy(), 1e-9);
        CHECK_CLOSE(10.0, v.dz(), 1e-9);
    }

    TEST(24_ComponentDivision_ValidValues) {
        Vector3D v1(0.0, 0.0, 0.0, 10.0, 12.0, 14.0);
        Vector3D v2(0.0, 0.0, 0.0, 2.0, 4.0, 7.0);
        Vector3D res = v1 / v2;

        CHECK_CLOSE(5.0, res.dx(), 1e-9);
        CHECK_CLOSE(3.0, res.dy(), 1e-9);
        CHECK_CLOSE(2.0, res.dz(), 1e-9);
    }

    TEST(25_OperatorDivideEqual_ModifiesLhs) {
        Vector3D v1(1.0, 1.0, 1.0, 7.0, 10.0, 13.0); 
        Vector3D v2(0.0, 0.0, 0.0, 3.0, 3.0, 4.0);
        v1 /= v2;

        CHECK_CLOSE(2.0, v1.dx(), 1e-9);
        CHECK_CLOSE(3.0, v1.dy(), 1e-9);
        CHECK_CLOSE(3.0, v1.dz(), 1e-9);
    }

    TEST(26_ComponentDivision_ThrowsWhenRhsDxIsZero) {
        Vector3D v1(0.0, 0.0, 0.0, 4.0, 4.0, 4.0);
        Vector3D v2(1.0, 0.0, 0.0, 1.0, 2.0, 2.0);
        CHECK_THROW(v1 / v2, std::invalid_argument);
    }

    TEST(27_ComponentDivision_ThrowsWhenRhsDzIsZero) {
        Vector3D v1(0.0, 0.0, 0.0, 4.0, 4.0, 4.0);
        Vector3D v2(0.0, 0.0, 5.0, 2.0, 2.0, 5.0); // dz = 0
        CHECK_THROW(v1 /= v2, std::invalid_argument);
    }

    TEST(28_Comparison_EqualVectors_SameLength) {
        Vector3D v1(0.0, 0.0, 0.0, 3.0, 4.0, 0.0); 
        Vector3D v2(1.0, 1.0, 1.0, 1.0, 4.0, 5.0);
        CHECK(v1 == v2);
    }

    TEST(29_Comparison_NotEqual) {
        Vector3D v1(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, 2.0, 0.0, 0.0);
        CHECK(v1 != v2);
    }

    TEST(30_Comparison_LessThan) {
        Vector3D shortVec(0.0, 0.0, 0.0, 1.0, 1.0, 1.0); 
        Vector3D longVec(0.0, 0.0, 0.0, 2.0, 2.0, 2.0);  
        CHECK(shortVec < longVec);
        CHECK(!(longVec < shortVec));
    }

    TEST(31_Comparison_LessOrEqual) {
        Vector3D v1(0.0, 0.0, 0.0, 0.0, 0.0, 5.0); 
        Vector3D v2(0.0, 0.0, 0.0, 5.0, 0.0, 0.0); 
        Vector3D v3(0.0, 0.0, 0.0, 6.0, 0.0, 0.0); 
        CHECK(v1 <= v2);
        CHECK(v1 <= v3);
    }

    TEST(32_Comparison_GreaterThan) {
        Vector3D v1(0.0, 0.0, 0.0, 10.0, 0.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, 2.0, 3.0, 4.0);
        CHECK(v1 > v2);
        CHECK(!(v2 > v1));
    }

    TEST(33_Comparison_GreaterOrEqual) {
        Vector3D v1(0.0, 0.0, 0.0, 0.0, 10.0, 0.0);
        Vector3D v2(0.0, 0.0, 0.0, 10.0, 0.0, 0.0);
        Vector3D v3(0.0, 0.0, 0.0, 0.0, 5.0, 0.0);
        CHECK(v1 >= v2);
        CHECK(v1 >= v3);
    }

    TEST(34_StreamOutput_CorrectFormat) {
        Vector3D v(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        std::ostringstream out;
        out << v;
        CHECK_EQUAL("(1, 2, 3) (4, 5, 6)", out.str());
    }

    TEST(35_StreamInput_AllSixCoordinates) {
        Vector3D v;
        std::istringstream in("-1.5 0.5 2.0 10.2 8.4 -3.1");
        in >> v;

        CHECK_CLOSE(-1.5, v.getX1(), 1e-9);
        CHECK_CLOSE(0.5, v.getY1(), 1e-9);
        CHECK_CLOSE(2.0, v.getZ1(), 1e-9);
        CHECK_CLOSE(10.2, v.getX2(), 1e-9);
        CHECK_CLOSE(8.4, v.getY2(), 1e-9);
        CHECK_CLOSE(-3.1, v.getZ2(), 1e-9);
    }
}

int main() {
    return UnitTest::RunAllTests();
}