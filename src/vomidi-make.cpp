//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.31
// Last Modified: 2019.1.9
// Filename:      vomidi/src/vomidi-make.cpp
// Website:       http://vo.midilib.com
// Syntax:        C++11
#include <list>
#include <string>
#include <sstream>
#include <memory.h>
#include "mixer.h"
#include "parser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

using namespace vomidi;

long getModifyTime(const char * path){    
    FILE * fp;
    int fd;
    struct stat buf;
    fp=fopen(path,"r");
    
    if(fp==NULL)
        return -1;
    
    fd=fileno(fp);
    fstat(fd, &buf);
    long modify_time=buf.st_mtime;
    fclose(fp);
    return modify_time;
}

class makeParser{
    public:
        typedef std::pair<std::string,double> vpath;
        std::list<vpath> notationList;
        std::list<vpath> mixList;
        std::string sfpath;
        inline void include(double v , const std::string & path){
            this->parse(v,path);
        }
        
        inline void notation(double v , const std::string & path){
            notationList.push_back(vpath(path , v));
            mixList.push_back(vpath(path+".wav" , v));
        }
        
        inline void mix(double v , const std::string & path){
            mixList.push_back(vpath(path , v));
        }
        
        void vomake(double v , const std::string & path){
            auto fp=fopen(path.c_str() , "r");
            
            if(fp==NULL){
                VOMIDIDebug("load %s fail" , path.c_str());
                return;
            }
            
            char bufs[4096];
            while(!feof(fp)){
                bzero(bufs , sizeof(bufs));
                fgets(bufs , sizeof(bufs) , fp);
                if(bufs[0]=='#' || bufs[0]=='\0'){
                    continue;
                }else{
                    char * cp=bufs;
                    char * pp=NULL;
                    while(1){
                        if(*cp=='\0')
                            break;
                        else
                        if(*cp==':'){
                            pp=cp+1;
                            break;
                        }
                        ++cp;
                    }
                    if(pp){
                        this->notation(v , pp);
                    }
                }
            }
            
            fclose(fp);
        }
        
        void parse(double v , const std::string & path){
            std::string conf=path+"/voMakeList";
            std::string name , npath;
            double vol;
            auto fp=fopen(conf.c_str() , "r");
            
            if(fp==NULL){
                VOMIDIDebug("load %s fail" , conf.c_str());
                return;
            }
            
            char bufs[4096];
            
            while(!feof(fp)){
                bzero(bufs , sizeof(bufs));
                fgets(bufs , sizeof(bufs) , fp);
                if(bufs[0]=='#' || bufs[0]=='\0'){
                    continue;
                }else
                if(bufs[0]=='!'){
                    this->sfpath=bufs+1;
                }else{
                    std::istringstream iss(bufs);
                    iss>>name;
                    iss>>vol;
                    iss>>npath;
                    if(name=="include"){
                        this->include(vol*v , path+npath);
                    }else
                    if(name=="notation"){
                        this->notation(vol*v , path+npath);
                    }else
                    if(name=="mix"){
                        this->mix(vol*v , path+npath);
                    }else
                    if(name=="vomake"){
                        this->vomake(vol*v , path+npath);
                    }
                }
            }
            
            fclose(fp);
        }
        
        bool compile(const std::string & path){
            auto output=path+".wav";
            long nt=getModifyTime(path.c_str());
            long wt=getModifyTime(output.c_str());
            if(nt==-1)
                return false;
            if(wt!=-1){
                if(wt>nt)
                    return true;
            }
            //compile file
            parser      ps;
            soundfont   sf(this->sfpath.c_str());
            synth       syn(output.c_str() , &sf , 16);
            if(ps.load(path.c_str())){
                ps.exportSynth(&syn);
                return true;
            }else{
                return false;
            }
        }
        
        bool mixAll(const std::string & path){
            wavMixer wm;
            for(auto it:mixList){
                if(!wm.add(it.first.c_str(),it.second)){
                    VOMIDIDebug("mix fail:%s" , it.first.c_str());
                    return false;
                }
            }
            if(!wm.setOutput(path.c_str())){
                VOMIDIDebug("setOutput fail:%s" , path.c_str());
                return false;
            }
            return wm.mixAll();
        }
        
        bool build(){
            for(auto it:notationList){
                if(!this->compile(it.first)){
                    VOMIDIDebug("compile %s fail" , it.first.c_str());
                    return false;
                }
            }
            return this->mixAll("vomidi-out.wav");
        }
};
int main(int argc , const char ** argv){
    makeParser mp;
    mp.parse(1.0d , "./");
    if(mp.build()){
        return 0;
    }else{
        printf("compilation terminated.\n");
        return 1;
    }
}
