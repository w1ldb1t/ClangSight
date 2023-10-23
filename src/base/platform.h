#if defined(_WIN32)
#define WINDOWS
#ifdef max
#undef max
#endif
#elif !defined(_WIN32) && defined(__linux__)
#define POSIX
#endif