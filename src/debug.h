#ifndef VOMIDI_debug
#define VOMIDI_debug
namespace vomidi{
    #define VOMIDIDebug(fmt, ...) \
            printf("[%s:%d] %s() " fmt "\n" , __FILE__ , __LINE__ , __FUNCTION__ , ##__VA_ARGS__);
}
#endif
