#include "spec.h"

#include <iostream>

namespace specific {

    SpecBase::SpecBase() : mName(NULL), 
        mFailed(false), mLastFailed(false), mNumTests(0), mNumFailures(0) 
    {
        SpecRunner::getInstance().add(this);
    }


    SpecBase::~SpecBase() {
        
    }


    bool SpecBase::startSpec(const char* name) 
    {
        endSpec();
        std::cout << " - " << name ;
        mName = name; 
        return true;
    }


    void SpecBase::endSpec() 
    {
        if(!mName) return;
        if(mLastFailed) {
            std::cout << "  [FAILED]" << std::endl;
        } else {
            std::cout << "  [OK]" << std::endl;    
        }
        mName = NULL; 
    }


    void SpecBase::should_test(bool value, const char* message, const char* file, int line) {
        mLastFailed=false;
        mNumTests+=1;
        if(!value) {
            std::cout << std::endl << "  Failed assertation at " << file << ":" << line << ": " << 
                std::endl <<  "    " << message;
            mLastFailed = mFailed = true;
            mNumFailures+=1;
        }
    }



    SpecRunner::SpecRunner() : mNumTotalTests(0), mNumTotalFailures(0) {}
    SpecRunner::~SpecRunner() { }

    SpecRunner& SpecRunner::getInstance() {
        static SpecRunner* instance = NULL;
        if( instance == NULL ) {
            instance = new SpecRunner;
        }
        return *instance;
    }


    bool SpecRunner::run(const std::string subset) {
        bool success = true;
        std::vector<SpecBase*>::iterator i = mSpecs.begin();
        for(; i != mSpecs.end(); ++i) {
            SpecBase *b = *i;
            if( b->getGroup().find(subset, 0) == std::string::npos ) continue;
            std::cout << "* " << b->getGroup() << ": " << b->getDescription() << std::endl;
            try {
                b->specify();
            } catch( std::exception& e) {
                std::cout << "\nUnhandled exception: " << e.what() << std::endl;
                b->mFailed = b->mLastFailed = true;
            } catch( ... ) {
                std::cout << "\nUnhandled unknown exception" << std::endl;
                b->mFailed = b->mLastFailed = true;
            }
            b->endSpec();
            success = success && b->isSuccessful();
            mNumTotalTests += b->mNumTests;
            mNumTotalFailures += b->mNumFailures;

        }

        return success;
    }

    void SpecRunner::printSummary() {
        std::cout << "Result: " << mNumTotalFailures << " failures in "
                                << mNumTotalTests << " tests in " 
                                << mSpecs.size() << " specs." << std::endl;
    }

}




