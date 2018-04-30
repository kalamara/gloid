#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "Point.h"

TEST_GROUP(FunctionsTestGroup){
    void teardown(){
            mock().clear();
    }
};

int rand() {
    return mock().actualCall("rand")
                 .returnIntValue();

}

TEST(FunctionsTestGroup, ctor_dtor_test){
    Point3f p = Point3f();
    DOUBLES_EQUAL(ZERO, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, p.z, FLOAT_PRECISION);

    p = Point3f(ONE, ONE, ONE);
    DOUBLES_EQUAL(ONE, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ONE, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ONE, p.z, FLOAT_PRECISION);

    mock().expectNCalls(3, "rand")
            .andReturnValue(RAND_MAX/2);
//M = 0
    p = Point3f(2*ONE);
    DOUBLES_EQUAL(ZERO, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, p.z, FLOAT_PRECISION);
    mock().expectNCalls(3, "rand")
            .andReturnValue(RAND_MAX);
    p = Point3f(2*ONE);
//M = V3/2 * RAND_MAX
    DOUBLES_EQUAL(2.0f/ROOT3, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f/ROOT3, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f/ROOT3, p.z, FLOAT_PRECISION);
    Point3f p2 = Point3f(p);
    DOUBLES_EQUAL(2.0f/ROOT3, p2.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f/ROOT3, p2.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(2.0f/ROOT3, p2.z, FLOAT_PRECISION);

    Point3i i = Point3i();
    CHECK(i.X == 0);
    CHECK(i.Y == 0);
    CHECK(i.Z == 0);

    i = Point3i(1,2,3);
    CHECK(i.X == 1);
    CHECK(i.Y == 2);
    CHECK(i.Z == 3);

    Point3i i2 = Point3i(i);
    CHECK(i2.X == 1);
    CHECK(i2.Y == 2);
    CHECK(i2.Z == 3);

    p = Point3f(i);
    //5x - 15
    DOUBLES_EQUAL(-10.0f, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-5.0f, p.y, FLOAT_PRECISION);
    //-2.5x -1.25
    DOUBLES_EQUAL(-8.75f, p.z, FLOAT_PRECISION);
//0,0,0 should be the center of the brick in (0,0,0)
//    => (-15, -15, -1.25)
// (half brick up left and half brick away from player)
    p = Point3f(Point3i(0,0,0));
    DOUBLES_EQUAL(-15.0f, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(-15.0f, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-1.25f, p.z, FLOAT_PRECISION);

    i2 = Point3i(p);
    CHECK_EQUAL(0, i2.X);
    CHECK_EQUAL(0, i2.Y);
    CHECK_EQUAL(0, i2.Z);

    p2 = p2.deepcopy(p);
    p = Point3f(Point3i(p2));

    CHECK(p.eq(p2));

    p = Point3f(Point3i(6,6,12));
    DOUBLES_EQUAL(15.0f, p.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(15.0f, p.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(-31.25f, p.z, FLOAT_PRECISION);

    //CHECK(p.eq(Point3f(Point3i(p))));
}

TEST(FunctionsTestGroup, chase_test){
    //speed is 0 => (0,0,0)
    Point3f start = Point3f();
    Point3f end = Point3f(ONE, ONE, ONE);

    Point3f speed = start.chase(end, ZERO);
    DOUBLES_EQUAL(ZERO, speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO, speed.z, FLOAT_PRECISION);
//speed is V3 => all coeffs equal to 1
    speed = start.chase(end, ROOT3);
    DOUBLES_EQUAL(ONE, speed.x, FLOAT_PRECISION);
    DOUBLES_EQUAL(ONE, speed.y, FLOAT_PRECISION);
    DOUBLES_EQUAL(ONE, speed.z, FLOAT_PRECISION);

}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
