//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/debug.h
// Website:       http://vo.midilib.com
// Syntax:        C++11
#ifndef VOMIDI_debug
#define VOMIDI_debug
namespace vomidi{
    #define VOMIDIDebug(fmt, ...) \
            printf("[%s:%d] %s() " fmt "\n" , __FILE__ , __LINE__ , __FUNCTION__ , ##__VA_ARGS__);
}
#endif
