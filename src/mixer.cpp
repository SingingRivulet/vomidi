//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2019.1.1
// Last Modified: 2019.1.1
// Filename:      vomidi/src/mixer.cpp
// Website:       http://vo.midilib.com
// Syntax:        C++11
#include "mixer.h"
namespace vomidi{

void mixer::mix(const std::vector<std::pair<soundtouch::SAMPLETYPE,double> > & res , soundtouch::SAMPLETYPE & out){
    #ifdef SOUNDTOUCH_INTEGER_SAMPLES
        static const soundtouch::SAMPLETYPE MAX=32767;
        static const soundtouch::SAMPLETYPE MIN=-32768;
    #else
        static const soundtouch::SAMPLETYPE MAX=1.0d;
        static const soundtouch::SAMPLETYPE MIN=-1.0d;
    #endif
    
    soundtouch::SAMPLETYPE sum=0;
    for(auto it:res){
        sum+=it.first*it.second;
    }
    
    auto output=(soundtouch::SAMPLETYPE)sum;
    
    if (output > MAX){
        f = (double)MAX / (double)(output);
        output = MAX;
    }
    
    if (output < MIN){
        f = (double)MIN / (double)(output);
        output = MIN;
    }
    
    if (f < 1){
        f += ((double)1 - f) / (double)32;
    }
    
    out=(soundtouch::SAMPLETYPE)output;
}

mixer::mixer(){
    f=1;
    this->wavout=NULL;
}

bool mixer::mixAll(){
    f=1;
    int i,j;
    auto count=resource.size();
    int rate=-1;
    int nch=-1;
    for(i=0;i<count;i++){
        if(rate==-1){
            rate=resource[i].first->getSampleRate();
        }else
        if(rate!=resource[i].first->getSampleRate()){
            throw rateError();
            return false;
        }
        
        if(nch==-1){
            nch=(int)resource[i].first->getNumChannels();
        }else
        if(nch!=(int)resource[i].first->getNumChannels()){
            throw channelsError();
            return false;
        }
    }
    
    auto gbuffer=new soundtouch::SAMPLETYPE[4096];
    std::vector<std::pair<soundtouch::SAMPLETYPE* , int> > buffer(count);
    for(i=0;i<count;i++){
        buffer[i].first=new soundtouch::SAMPLETYPE[4096];
        buffer[i].second=0;
    }
    
    std::vector<std::pair<soundtouch::SAMPLETYPE,double> > res(count);
    
    int useable , useable2;
    while(1){
        useable=0;
        for(i=0;i<count;i++){
            if(resource[i].first->eof()){
                buffer[i].second = -1;
                continue;
            }else{
                ++useable;
                //read buffer
                buffer[i].second = resource[i].first->read(buffer[i].first,4096);
            }
        }
        
        
        for(j=0;j<4096;j++){
            useable2=0;
            for(i=0;i<count;i++){
                //mix 
                if(buffer[i].second < j){
                    res[i].first=0;
                    res[i].second=resource[i].second;
                    continue;
                }else{
                    ++useable2;
                    res[i].first=buffer[i].first[j];
                    res[i].second=resource[i].second;
                }
            }
            soundtouch::SAMPLETYPE tmp;
            this->mix(res , tmp);
            gbuffer[j]=tmp;
            if(useable2==0){
                ++j;
                break;
            }
        }
        
        if(j>0){
            this->writeOut(gbuffer , j);
        }
        
        if(useable<=0)
            break;
    }
    
    for(i=0;i<count;i++)
        delete [] buffer[i].first;
    return true;
}

void mixer::writeOut(const soundtouch::SAMPLETYPE * out , int len){
    if(this->wavout)
        this->wavout->write(out , len);
}

}//namespace vomidi
