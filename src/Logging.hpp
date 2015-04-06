#ifndef AMPUSH_LOGGING_HPP
#define AMPUSH_LOGGING_HPP

#define _ELPP_THREAD_SAFE
#define _ELPP_STACKTRACE_ON_CRASH
#define _ELPP_STL_LOGGING
#define _ELPP_DEFAULT_LOG_FILE "ampush.log"

#include <build-config.h>
#if defined(AMPUSH_LOGGING_ENABLED)
# include <easylogging++.h>
# define AMPUSH_LOGGING_INIT _INITIALIZE_EASYLOGGINGPP
#else
# define AMPUSH_LOGGING_INIT static const int __void=0
# include <iosfwd>
struct __NullStream {};
template <typename T>
__NullStream & operator<<(__NullStream & s, T const &) {return s;}
static __NullStream & operator<<(__NullStream & s, std::ostream &(std::ostream&)) {return s;}
static __NullStream __nullstream;
# define LOG(...) if(0) __nullstream
#endif

namespace Amplify
{

} /* namespace Amplify */

#endif /* AMPUSH_LOGGING_HPP */
