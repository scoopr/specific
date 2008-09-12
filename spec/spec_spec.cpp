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
