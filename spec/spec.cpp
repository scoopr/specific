#include "spec.h"

#include <iostream>

namespace specific {



    void SpecWriter::startGroup(std::string group, std::string description) {
    }
    void SpecWriter::addFailedAssertation(std::string msg, const char *file, int line) {
        mFailures.push_back( SpecFailure(msg,file,line) );
    }
    void SpecWriter::addSpecResult(SpecResult r) {
        mResults.push_back( r );
    }
    void SpecWriter::start() {}
    void SpecWriter::stop() {
        std::cout << std::endl;
        size_t nth = 0;
        for(std::vector<SpecFailure>::iterator i=mFailures.begin(); i != mFailures.end(); ++i, ++nth)
        {
            std::cout << std::endl;
            std::cout << (nth+1) << ") Failed assertation at " << i->file << ":"
            << i->line << ":" << std::endl << "  " << i->msg << std::endl;
        }
        std::cout << std::endl << mResults.size() << " examples, " << mFailures.size() << " failures" << std::endl;
 
    }
 


    void ProgressWriter::addSpecResult(SpecResult r) {
        SpecWriter::addSpecResult(r);
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
        std::cout << std::flush;
    }



    void SpecdocWriter::startGroup(std::string group, std::string description) {
        std::cout << group << ": " << description << std::endl;            
    }


    void SpecdocWriter::addSpecResult(SpecResult r) {
        SpecWriter::addSpecResult(r);
        size_t nth = mFailures.size();
        std::cout << "- " << r.test;
        switch(r.type) {
            case SpecResult::PASSED:
                std::cout << " [OK]";
                break;
            case SpecResult::FAILED:
                std::cout << " [FAILED - " << nth << "]";
                break;
            case SpecResult::ERRORED:
                std::cout << " [ERROR - "<< nth <<"]";
                break;
        }
        std::cout << std::endl;
    }








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

        mName = name;
        return true;
    }


    void SpecBase::endSpec() 
    {
        if(!mName) return;

        SpecResult r;
        r.group = getGroup();
        r.description = getDescription();
        r.type = SpecResult::PASSED;
        if(mLastFailed) r.type = SpecResult::FAILED;
        if(mError) r.type = SpecResult::ERRORED;
        r.test = mName;
        mWriter->addSpecResult( r );
        
        mName = NULL; 
    }


    void SpecBase::should_test(bool value, const char* message, const char* file, int line) {
        mLastFailed=false;
        mNumTests+=1;
        if(!value) {
            mWriter->addFailedAssertation(message, file, line);
            mLastFailed = mFailed = true;
            mNumFailures+=1;
        }
    }

    
    void SpecBase::error(std::string msg) {
        mWriter->addFailedAssertation(msg, "exception", 0);
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


    bool SpecRunner::run(SpecWriter& writer, const std::string subset) {
        bool success = true;
        
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


}




