
#include "spec/spec.h"
#include <cstddef>

int main(int argc, char *argv[]) 
{

    std::string subset("");

    specific::ProgressWriter progressWriter;
    specific::SpecdocWriter specdocWriter;
    specific::SpecWriter* writer = &progressWriter;

    for(size_t i = 1; i < size_t(argc); ++i) {
        if( std::string("-s") == argv[i] ) {
            writer = &specdocWriter;
        } else {
            subset = argv[i];
        }
    }


    bool success = specific::SpecRunner::getInstance().run(*writer, subset);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

