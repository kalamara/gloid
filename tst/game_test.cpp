#include "GLoid.h"
//#include <functional>
//#include <optional>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "Point.h"
#include "Engine.h"
#include "World.h"
#include "Game.h"

TEST_GROUP(GameTestGroup){
    void teardown(){
            mock().clear();
    }
};

TEST(GameTestGroup, ctor_dtor_test){
    Game game();
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
