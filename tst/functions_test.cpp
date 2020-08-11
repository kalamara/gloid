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
//integer vectors are compared as digits Z Y X  by order of significance

    Point3i a(1,2,3);
    Point3i b(1,2,3);
    CHECK(a == b);
    CHECK(a >= b);
    CHECK(a <= b);

    b.X = 4;
    CHECK(b > a);
    CHECK(b >= a);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK(a != b);

    b.Y = 0;
    CHECK(b < a);

    a.Z = -1;
    CHECK(b > a);

    const Point3i c(2,1,0);
    const Point3i d(3,1,0);
    CHECK(d > c);
}

TEST(FunctionsTestGroup, distance_test){
    Point3f z = Point3f(ZERO, ZERO, ZERO);
    CHECK(z.dist(z) >= ZERO);
    CHECK(z.dist(z) <= FLOAT_PRECISION);

    Point3f ones = Point3f(ONE, ZERO, ZERO);

    CHECK(ones.dist(ones) >= ZERO);
    CHECK(abs(ones.dist(ones)) <= FLOAT_PRECISION);

    CHECK(z.dist(ones) >= ZERO);
    CHECK(abs(z.dist(ones) - ONE) <= FLOAT_PRECISION);

    ones.y = ONE;
    CHECK(abs(z.dist(ones) - ROOT2) <= FLOAT_PRECISION);

    ones.z = ONE;
    CHECK(abs(z.dist(ones) - ROOT3) <= FLOAT_PRECISION);

    ones.z = ONE;
    CHECK(abs(ones.dist(z) - ROOT3) <= FLOAT_PRECISION);

    ones = Point3f(-ONE, -ONE, -ONE);
    CHECK(abs(ones.dist(z) - ROOT3) <= FLOAT_PRECISION);

    Point3f twos = Point3f(2.0f, 2.0f, 2.0f);
    CHECK(abs(ones.dist(twos) - (3.0f * ROOT3)) <= FLOAT_PRECISION);

}

TEST(FunctionsTestGroup, raycast_test){
    auto start = Point3f(0,0,0);
    auto speed = Point3f(0,0,0);
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
    CHECK(collision.eq(Point3f(ONE,ONE,-ONE)));
    collision = start.raycast(speed, AXIS_Z, 2*ONE);
    CHECK(collision.eq(Point3f(-2*ONE,-2*ONE,2*ONE)));
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

TEST(FunctionsTestGroup, prefix_test){
    Point3f speed(0,0,0);

    CHECK(speed.signs().eq(Point3i(1,1,1)));

    speed.x = 1;
    speed.y = -1;
    speed.z = -12345;

    CHECK(speed.signs().eq(Point3i(1,-1,-1)));
}

TEST(FunctionsTestGroup, scale_test){

    CHECK(Point3f(0,0,0).scale3f(Point3f(0,0,0))
          .eq(Point3f(0,0,0)));

    CHECK(Point3f(1,2,3).scale3f(Point3f(0,0,0))
          .eq(Point3f(1,2,3)));

    CHECK(Point3f(1,1,0).scale3f(Point3f(0,0,-1))
          .eq(Point3f(1,1,0)));

    CHECK(Point3f(1,2,3).scale3f(Point3f(1,1,1))
          .eq(Point3f(1,2,3)));

    CHECK(Point3f(1,2,3).scale3f(Point3f(2,2,2))
          .eq(Point3f(1,2,3)));

    CHECK(Point3f(1,2,3).scale3f(Point3f(2,-2,2))
          .eq(Point3f(1,-2,3)));

    CHECK(Point3f(1,2,2).scale3f(Point3f(3,0,0))
          .eq(Point3f(3,0,0)));
}

TEST(FunctionsTestGroup, projection_test){
    //zeroes should give zeroes
    DOUBLES_EQUAL(ZERO,Point3f(0,0,0).proj3f(Point3f(0,0,0)), FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO,Point3f(0,0,0).proj3f(Point3f(1,2,3)), FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO,Point3f(1,2,3).proj3f(Point3f(0,0,0)), FLOAT_PRECISION);
    //orthogonals should give zeroes
    DOUBLES_EQUAL(ZERO,Point3f(1,0,0).proj3f(Point3f(0,1,0)), FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO,Point3f(0,1,0).proj3f(Point3f(0,0,1)), FLOAT_PRECISION);
    DOUBLES_EQUAL(ZERO,Point3f(0,0,1).proj3f(Point3f(1,0,0)), FLOAT_PRECISION);
    //projection of parallels should equal ratio of norms
    DOUBLES_EQUAL(0.5,Point3f(1,2,3).proj3f(Point3f(2,4,6)), FLOAT_PRECISION);
    DOUBLES_EQUAL(ONE,Point3f(0,1,0).proj3f(Point3f(0,1,0)), FLOAT_PRECISION);
    //projection of opposites should equal minus ratio of norms
    DOUBLES_EQUAL(-0.5,Point3f(-1,-2,-3).proj3f(Point3f(2,4,6)), FLOAT_PRECISION);
    DOUBLES_EQUAL(-ONE,Point3f(0,-1,0).proj3f(Point3f(0,1,0)), FLOAT_PRECISION);

}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
