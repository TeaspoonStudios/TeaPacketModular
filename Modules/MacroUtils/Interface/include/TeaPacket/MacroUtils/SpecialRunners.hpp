#pragma once

#define TP_RunBeforeMain(Function, StepName) \
    class StepName##BeforeMain {    \
        public: \
        StepName##BeforeMain() \
        { \
            Function();\
        }\
    }; \
    static StepName##BeforeMain StepName##PreMainInstance;