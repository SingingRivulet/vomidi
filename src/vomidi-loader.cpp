//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/vomidi-loader.cpp
// Website:       http://vo.midilib.com
// Syntax:        C++11
#include "generator.h"
#include <stdio.h>
int main(int argc , const char ** argv){
    if(argc<3){
        printf("%s [project path] [midi path]\n" , argv[0]);
        return 1;
    }
    vomidi::midiLoader(argv[2] , argv[1]);
    return 0;
}
