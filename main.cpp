
#include "spec/spec.h"
#include <cstddef>

int main(int argc, char *argv[]) 
{

    std::string subset("");
    if(argc > 1) subset=argv[1];

    bool success = specific::SpecRunner::getInstance().run(subset);

    specific::SpecRunner::getInstance().printSummary();

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
