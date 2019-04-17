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

TEST(FunctionsTestGroup, comparison_test){
    Point3i a(1,2,3);
    Point3i b(1,2,3);
    CHECK(a == b);
    CHECK(a >= b);
    CHECK(a <= b);

    b.Z = 4;
    CHECK(b > a);
    CHECK(b >= a);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK(a != b);

    b.Y = 0;
    CHECK(b < a);

    a.X = -1;
    CHECK(b > a);

    const Point3i c(0,1,2);
    const Point3i d(0,1,3);
    CHECK(d > c)

}

TEST(FunctionsTestGroup, raycast_test){
    Point3f start; //(0,0,0)
    Point3f speed;
    //invalid axis should give equal to start
    Point3f collision = start.raycast(speed, 0, ONE);
    CHECK(collision.eq(start));

    //at zero speed raycast should equal start on every axis
    collision = start.raycast(speed, AXIS_X, ONE);
    CHECK(collision.eq(start));
    collision = start.raycast(speed, AXIS_Y, ZERO);
    CHECK(collision.eq(start));
    collision = start.raycast(speed, AXIS_Z, -ONE);
    CHECK(collision.eq(start));

    //at speed(u,u,u), on any plane p, collision point is (p,p,p)
    speed.x = ONE;
    speed.y = ONE;
    speed.z = ONE;
    collision = start.raycast(speed, AXIS_X, ZERO);
    CHECK(collision.eq(Point3f(ZERO,ZERO,ZERO)));
    collision = start.raycast(speed, AXIS_X, ONE);
    CHECK(collision.eq(Point3f(ONE,ONE,ONE)));
    collision = start.raycast(speed, AXIS_Z, 2*ONE);
    CHECK(collision.eq(Point3f(2*ONE,2*ONE,2*ONE)));
    //at speed(u,0,0), on (x=p), collision point is (p,0,0)
    speed.x = ONE;
    speed.y = ZERO;
    speed.z = ZERO;
    collision = start.raycast(speed, AXIS_X, 2*ONE);
    CHECK(collision.eq(Point3f(2*ONE,ZERO,ZERO)));
    //on other planes collision is equal to start (0,0,0)
    collision = start.raycast(speed, AXIS_Y, 2*ONE);
    CHECK(collision.eq(start));
    collision = start.raycast(speed, AXIS_Z, 2*ONE);
    CHECK(collision.eq(start));

    //at speed(u,u,0), on (x=p), collision point is (p,p,0)
    speed.x = -ONE;
    speed.y = -ONE;
    speed.z = ZERO;
    collision = start.raycast(speed, AXIS_X, 2*ONE);
    CHECK(collision.eq(Point3f(-2*ONE,-2*ONE,ZERO)));
    collision = start.raycast(speed, AXIS_Y, 2*ONE);
    CHECK(collision.eq(Point3f(-2*ONE,-2*ONE,ZERO)));
     //on (z=p) =>(0,0,0)
    collision = start.raycast(speed, AXIS_Z, 2*ONE);
    CHECK(collision.eq(start));

}


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
