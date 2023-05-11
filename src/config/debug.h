#pragma once


//Choose a test mode, only one can be enabled at a time, none should be for production
//#define TEST_DEBUG //for debugging without the mara
//#define LIVE_DEBUG //for debugging on the mara itself

#if defined TEST_DEBUG && defined LIVE_DEBUG
    #error "Cannot have both test and live debug enabled, please fix in debug.h"
#endif