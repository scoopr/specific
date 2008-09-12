#include "spec.h"

#include <iostream>

namespace specific {




    class StdoutWriter : public SpecWriter {
    public:
        std::vector<SpecResult> mResults;
        void startGroup(std::string /*group*/, std::string /*description*/) {}
        void addSpecResult(SpecResult r) {
            switch(r.type) {
                case SpecResult::PASSED:
                    std::cout << ".";
                    break;
                case SpecResult::FAILED:
                    std::cout << "F";
                    break;
                case SpecResult::ERRORED:
                    std::cout << "E";
                    break;
            }
            mResults.push_back( r );
        }
        void start() {}
        void stop() {
            std::cout << std::endl;
            for(std::vector<SpecResult>::iterator i=mResults.begin(); i != mResults.end(); ++i)
            {
                if( i->type != SpecResult::PASSED ) {
                    std::cout << "Failed assertation: " << i->test << std::endl;
                }
            }
        }
    };


    class SpecdocWriter : public SpecWriter {
    public:
        void startGroup(std::string group, std::string description) {
            std::cout << "* " << group << ": " << description << std::endl;            
        }
        void addSpecResult(SpecResult r) {
            std::cout << " - " << r.test;
            switch(r.type) {
                case SpecResult::PASSED:
                    std::cout << " [OK]";
                    break;
                case SpecResult::FAILED:
                    std::cout << " [FAILED]";
                    break;
                case SpecResult::ERRORED:
                    std::cout << " [ERROR]";
                    break;
            }
            std::cout << std::endl;
        }
        void start() {}
        void stop() {}
    };







    SpecBase::SpecBase() : mWriter(NULL), mName(NULL),
        mFailed(false), mLastFailed(false), mError(false), mExecutionPoint(0), mContinuePoint(0), mNumTests(0), mNumFailures(0) 
    {
        SpecRunner::getInstance().add(this);
    }


    SpecBase::~SpecBase() {
        
    }


    bool SpecBase::startSpec(const char* name) 
    {
        endSpec();

        mExecutionPoint++;
        if(mExecutionPoint <= mContinuePoint) return false;
        mContinuePoint++;

//        std::cout << " - " << name ;
        mName = name;
        return true;
    }


    void SpecBase::endSpec() 
    {
        if(!mName) return;
/*        if(mLastFailed) {
            std::cout << "  [FAILED]" << std::endl;
        } else {
            std::cout << "  [OK]" << std::endl;
        }*/

        SpecResult r;
        r.group = getGroup();
        r.description = getDescription();
        r.type = SpecResult::PASSED;
        if(mLastFailed) r.type = SpecResult::FAILED;
        if(mError) r.type = SpecResult::ERRORED;
        if(mFile) {
            r.file = mFile;
            r.line = mLine;
            mFile = NULL;            
        }
        r.test = mName;
        mWriter->addSpecResult( r );
        
        mName = NULL; 
    }


    void SpecBase::should_test(bool value, const char* message, const char* file, int line) {
        mLastFailed=false;
        mNumTests+=1;
        if(!value) {
/*            std::cout << std::endl << "  Failed assertation at " << file << ":" << line << ": " << 
                std::endl <<  "    " << message;*/
            mLastFailed = mFailed = true;
            mNumFailures+=1;
        }
    }

    
    void SpecBase::error(std::string msg) {
        mNumFailures+=1;
        mLastFailed = true;
        mFailed = true;
        mError = true;
    }

    bool SpecBase::done() {
        if( mError ) {
            mError = false;
            return false;
        }
        return true;
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
        StdoutWriter writer;
        
        writer.start();
        std::vector<SpecBase*>::iterator i = mSpecs.begin();
        for(; i != mSpecs.end(); ++i) {
            SpecBase *b = *i;
            if( b->getGroup().find(subset, 0) == std::string::npos ) continue;
            b->mContinuePoint = 0;
            b->setWriter(&writer);
            writer.startGroup( b->getGroup(), b->getDescription() );
            do {
                b->mExecutionPoint = 0;
                try {
                    b->specify();
                } catch( std::exception& e) {
                    b->error(e.what());
                } catch( ... ) {
                    b->error("unknown exception");
                }
                b->endSpec();
                
            } while( !b->done() );
            
            success = success && b->isSuccessful();
            mNumTotalTests += b->mNumTests;
            mNumTotalFailures += b->mNumFailures;

        }
        writer.stop();

        return success;
    }

    void SpecRunner::printSummary() {
        std::cout << "Result: " << mNumTotalFailures << " failures in "
                                << mNumTotalTests << " tests in " 
                                << mSpecs.size() << " specs." << std::endl;
    }

}




