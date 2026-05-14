#pragma once

#ifndef ENABLE_DEBUG
    #define ENABLE_DEBUG false
#else
    #undef ENABLE_DEBUG
    #define ENABLE_DEBUG true
#endif

// https://stackoverflow.com/a/11826787
#define DEBUG_STREAM \
    if (ENABLE_DEBUG == false) {} \
    else std::cerr

#define DEBUG DEBUG_STREAM << "[" << __FILE__ << ":" << __LINE__ << "]: "
