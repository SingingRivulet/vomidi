//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2019.1.1
// Filename:      vomidi/src/generator.cpp
// Website:       http://vo.midilib.com
// Syntax:        C++11
#include "generator.h"
#include <memory.h>
namespace vomidi{

void generator::noteid2str(int note , char * str ,int len){
    static const char * keys[]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
    int n=note%12;
    int h=note/12;
    snprintf(str , len , "%s%d" , keys[n] , h);
}

bool generator::pushAtTime(int note,int vol,double begin,double second){
    if(begin < this->time){
        return false;
    }else
    if(begin > this->time){
        this->pushEmpty(begin - this->time);
        this->time=begin;
    }
    this->pushVoice(note , vol ,second);
    this->time+=second;
    return true;
}

void generator::pushVoice(int note,int vol,double second){
    if(this->fp){
        char buf[8];
        this->noteid2str(note , buf , sizeof(buf));
        
        fprintf(this->fp , "%s %s %lf 1 %lf\n" , buf , this->word.c_str() , ((double)vol)/128.0d , second);
    }
}

void generator::pushEmpty(double second){
    if(this->fp){
        fprintf(this->fp , "0 0 0 0 %lf\n" , second);
    }
}

generator::generator(const char * path){
    this->fp=fopen(path , "w");
    fprintf(fp , "#note word velocity tempo delay\n");
    this->word="null";
}

generator::~generator(){
    if(this->fp)
        fclose(this->fp);
}


generators::generators(const char * output , FILE * config){
    this->pathPre=output;
    this->chnum=0;
    this->cfp =config;
}

generators::~generators(){
    for(auto it : this->gens){
        delete it;
    }
}

void generators::pushNotation(int note,int vol,double begin,double second){
    auto it=this->gens.begin();
    char buf[256];
    while(1){
        if(it==this->gens.end()){
            //create channel
            bzero(buf , sizeof(buf));
            snprintf(buf , sizeof(buf) , "%s-%d.notation" , this->pathPre.c_str() , (this->chnum++));
            if(this->cfp)
                fprintf(this->cfp , "notation:%s\n",buf);
            auto ptr=new generator(buf);
            ptr->pushAtTime(note , vol , begin , second);
            this->gens.push_back(ptr);
            return;
        }else{
            if((*it)->pushAtTime(note , vol , begin , second)){
                //push success
                return;
            }else{
                it++;
            }
        }
    }
}

bool midiLoader(const char * infile , const char * outfile){
    char buf[1024];
    char confpath[1024];
    snprintf(confpath , sizeof(confpath) , "%s.vomake" , outfile);
    auto cfp=fopen(confpath , "w");
    
    smf::MidiFile midifile;
    midifile.read(infile);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    int tracks = midifile.getTrackCount();
    
    for (int track=0; track<tracks; track++) {
        
        bzero(buf , sizeof(buf));
        snprintf(buf , sizeof(buf) , "%s-%d" , outfile , track);
        auto gen=new generators(buf , cfp);
        
        for (int event=0; event<midifile[track].size(); event++) {
            if (
              midifile[track][event].isNoteOn() && 
              midifile[track][event].size()>=3){
              
                //void generators::pushNotation(int note,int vol,double begin,double second)
                gen->pushNotation(
                    midifile[track][event][1],
                    midifile[track][event][2],
                    midifile[track][event].seconds,
                    midifile[track][event].getDurationInSeconds()
                );
            }
        }
        
        delete gen;
    }
    fclose(cfp);
    return true;
}

}//namespace vomidi

