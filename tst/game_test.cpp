#include "GLoid.h"
//#include <functional>
//#include <optional>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "Point.h"
#include "Game.h"


TEST_GROUP(GameTestGroup){
    void teardown(){
            mock().clear();
    }
};


TEST(GameTestGroup, lol){

}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
