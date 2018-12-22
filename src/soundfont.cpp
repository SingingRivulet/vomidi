#include "soundfont.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
namespace vomidi{

soundword::soundword(const char * path){
    try{
        this->wavin = new WavInFile(path);
    }catch(...){
        this->wavin = NULL;//wavin can't been used
    }
}

soundword::~soundword(){
    if(this->wavin)
        delete this->wavin;
}

soundfont::soundfont(const char * path){
    char bufs[2048];
    char buftmp[128];
    
    this->rate=0;
    this->channel=0;
    
    snprintf(bufs , sizeof(bufs) , "%s/config" , path);
    FILE * fp=fopen(bufs , "r");
    if(fp==NULL)
        return;
    std::string s1,s2;
    while(!feof(fp)){
        fgets(bufs , sizeof(bufs) , fp);
        if(bufs[0]=='\0')
            continue;
        
        if(bufs[0]=='@'){
            
            std::istringstream iss(bufs+1);
            iss>>s1;
            if(s1=="setRate"){
                if(this->rate==0)
                    iss>>(this->rate);
            }else
            if(s1=="setChannel"){
                if(this->channel==0)
                    iss>>(this->channel);
            }
            
        }else{
            
            std::istringstream iss(bufs);
            iss>>s1;//name
            iss>>s2;//file name
            if(s1.empty() || s2.empty())
                continue;
            
            if(this->fonts.find(s1)!=this->fonts.end()){
                //name has been existed
                continue;
            }else{
                snprintf(bufs , sizeof(bufs) , "%s/%S" , path , s2.c_str());
                auto wp=new soundword(bufs);
                
                if(wp->wavin==NULL){
                    //object isn't useable
                    delete wp;
                    continue;
                }
                
                //check sample rate
                auto nrate=wp->getSampleRate();
                if(this->rate==0){
                    this->rate=nrate;
                }else{
                    if(this->rate!=nrate){
                        delete wp;
                        continue;
                    }
                }
                
                //ckeck channel
                auto nchannel=wp->getNumChannels();
                if(this->channel==0){
                    this->channel=nchannel;
                }else{
                    if(this->channel!=nchannel){
                        delete wp;
                        continue;
                    }
                }
                
                iss>>(wp->kTone);
                
                this->fonts[s1]=wp;
            }
            
        }
    }
    fclose(fp);
}

soundfont::~soundfont(){
    for(auto it : this->fonts){
        if(it.second)
            delete it.second;
    }
}

soundword * soundfont::seek(const std::string & name){
    auto it=this->fonts.find(name);
    if(it==this->fonts.end())
        return NULL;
    else
        return it->second;
}

}//namespace vomidi
