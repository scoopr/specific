
#include "spec/spec.h"
#include <cstddef>

int main(int argc, char *argv[]) 
{

    std::string subset("");
    if(argc > 1) subset=argv[1];

    //specific::ProgressWriter writer;
    specific::SpecdocWriter writer;

    bool success = specific::SpecRunner::getInstance().run(writer, subset);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
