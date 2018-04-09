
#include <list>
#include "GLoid.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "Functions.h"

using namespace Functions;

TEST_GROUP(FunctionsTestGroup){
    void teardown(){
            mock().clear();
    }
};

int rand() {
    return mock().actualCall("rand")
                 .returnIntValue();
}

TEST(FunctionsTestGroup, RandomListMember){
    std::list<point3f> li ;
    //empty list should not crash
    std::list<point3f>::iterator r = randItem<point3f>(li.begin(), li.end());
    CHECK(li.end() == r);

    //otherwise return random value
    mock().expectOneCall("rand")
          .andReturnValue(1500001);

    li = {RED, GREEN, BLUE};
    r = randItem<point3f>(li.begin(), li.end());
    point3f green = GREEN;
    CHECK(green.eq(*r));
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
