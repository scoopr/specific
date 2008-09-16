/* Specific - Minimal C++ spec framework.
 

The zlib/libpng License


Copyright (c) 2008 Mikko Lehtonen

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/


#include "spec.h"


class ClassThatEqualsAny {
public:
    template<typename T> bool operator==(T /* anything */ ) { return true; }
    template<typename T> bool operator!=(T /* anything */ ) { return false; }
};

class ClassThatEqualsNone {
public:
    template<typename T> bool operator==(T /* anything */ ) { return false; }
    template<typename T> bool operator!=(T /* anything */ ) { return true; }
};


describe(Spec, "should matchers") {
    
    it("should have should_be_true, passing on true") {       
        should_be_true( true );
    }

    it("should have should_be_false, passing on false") {       
        should_be_false( false );
    }

    it("should have should_equal, passing on when operator== yields true") {
        should_equal( ClassThatEqualsAny(), false );
    }

    it("should have should_not_equal, passing on when operator!= yields true") {
        should_not_equal( ClassThatEqualsNone(), false );
    }

    
    it("should have should_throw, passing when given exception is thrown") {
        should_throw( throw std::exception(), std::exception);
    }
    
}

describe(Spec, "should matcher failures (THESE SHOULD ALL FAIL!)") {
    
    it("should have should_be_true, failing on false") {
        should_be_true( false );
    }

    it("should have should_be_false, failing on false") {
        should_be_false( true );
    }
    

    it("should have should_equal, failing when operator== yields false") {
        should_equal( ClassThatEqualsNone(), false );
    }

    it("should have should_not_equal, failing when operator!= yields false") {
        should_not_equal( ClassThatEqualsAny(), false );
    }

    it("spec framework should fail and continue execution even if unexpected exception occurs") {
        throw 1;
    }
    
    it("should have should_throw, failing when no exception thrown") {
        should_throw( (void)0 , std::exception);
    }

    it("should have should_throw, failing when wrong exception thrown") {
        should_throw( throw std::logic_error("this should be unhandled"), std::runtime_error);
    }

    
}


describe(OtherSpec, "just testing other group") {
    it("foo test") {}
}

