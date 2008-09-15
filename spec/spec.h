#ifndef SPECIFIC_SPEC_H
#define SPECIFIC_SPEC_H

#include <string>
#include <vector>
#include <stdexcept>


namespace specific {


    class SpecResult {
    public:
        typedef enum {
            PASSED,
            FAILED,
            ERRORED
        } Type;
      
        Type type;
        
        std::string group;
        std::string description;
        std::string test;
    };


    class SpecFailure {
    public:
        SpecFailure(std::string amsg, const char* afile, int aline)
            : msg(amsg), file(afile), line(aline) { }
        std::string msg;
        const char* file;
        int line;
    };


    class SpecWriter {
    public:
        std::vector<SpecResult> mResults;
        std::vector<SpecFailure> mFailures;
        SpecWriter() {}
        virtual ~SpecWriter() {}
        virtual void startGroup(std::string group, std::string description) = 0;
        virtual void addFailedAssertation(std::string msg, const char *file, int line) {
            mFailures.push_back( SpecFailure(msg,file,line) );
        }
        virtual void addSpecResult(SpecResult r) {
            mResults.push_back( r );
        }
        virtual void start() = 0;
        virtual void stop() = 0;
    };


    class ProgressWriter : public SpecWriter {
    public:
        void startGroup(std::string /*group*/, std::string /*description*/);
        void addSpecResult(SpecResult r);
        void start();
        void stop();
    };



    class SpecdocWriter : public SpecWriter {
    public:
        void startGroup(std::string group, std::string description);
        void addFailedAssertation(std::string msg, const char *file, int line);
        void addSpecResult(SpecResult r);
        void start();
        void stop();
    };




    class SpecBase {
    public:
        SpecBase();
        virtual ~SpecBase();

        virtual void specify() = 0;

        void setWriter(SpecWriter* w) { mWriter = w; }

        bool startSpec(const char* name);
        void endSpec();

        void should_test(bool value, const char* message, const char* file, int line);

        virtual std::string getGroup() = 0;
        virtual std::string getDescription() = 0;

        bool isSuccessful() { return !mFailed; }
        
        bool done();
        
        void error(std::string msg);

        SpecWriter* mWriter;
        const char* mName;
        bool mFailed;
        bool mLastFailed;
        bool mError;
        int mExecutionPoint;
        int mContinuePoint;
        int mNumTests;
        int mNumFailures;
        char *mFile;
        std::string mErrorMessage;
        int mLine;
    };


    class SpecRunner {
    public:
        static SpecRunner& getInstance();
        void add(SpecBase* spec) { mSpecs.push_back( spec ); }
        bool run(SpecWriter& writer, const std::string subset = "");
    private:

        std::vector<SpecBase*> mSpecs;
        int mNumTotalTests;
        int mNumTotalFailures;

        SpecRunner();
        ~SpecRunner();
    };

    #define SPEC_UNIQUE_NAME3(x,y) x##y
    #define SPEC_UNIQUE_NAME2(x,y) SPEC_UNIQUE_NAME3(x,y)

    #define SPEC_NAME(x) SPEC_UNIQUE_NAME2(SPEC_##x, SPEC_UNIQUE_NAME2(_startingOnLine, __LINE__) )


    #define describe(group, description)                                    \
    class SPEC_NAME(group) : public specific::SpecBase                         \
    {                                                                       \
    public:                                                                 \
        void specify();                                                     \
        std::string getGroup() { return #group; }                           \
        std::string getDescription() { return description; }                \
    };                                                                      \
    static SPEC_NAME(group) SPEC_UNIQUE_NAME2(SPEC_NAME(group), _instance); \
    void SPEC_NAME(group)::specify()
    

    #define it(description) if(startSpec(description))


    // Matchers
    #define should_be_true(a) should_test(a, #a, __FILE__, __LINE__)
    #define should_be_false(a) should_be_true( !a )
    #define should_equal(a, b) should_be_true( (a) == (b) )

    #define should_not_equal(a, b) should_be_true( (a) != (b) )

    #define should_throw(code, what) \
    do {                             \
        bool _thrown = false;        \
        try {                        \
          code ;                     \
        } catch(what& e) {           \
            _thrown = true;          \
        }                            \
        should_test(_thrown, "should throw exception " #what, __FILE__, __LINE__); \
    } while(0)



}



#endif /* Include guard */

