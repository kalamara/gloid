
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

TEST(FunctionsTestGroup, lol){

}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
