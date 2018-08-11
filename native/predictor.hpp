#ifndef srbot_predictor
#define srbot_predictor
#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <exception>
#include <unordered_map>
using namespace std;
class predictor{
    public:
        class FloatError:public std::exception{};
        struct reslog{
            int Tnum,Fnum;
            reslog(){
                Tnum=0;
                Fnum=0;
            }
        };
    private:
        unordered_map<string,reslog> history;
    public:
        void train(const list<string> & arr,bool res){
            if(res){
                for(auto it:arr)
                    history[it].Tnum++;
            }else{
                for(auto it:arr)
                    history[it].Fnum++;
            }
        }
        bool predict(const list<string> & arr){
            int Tnum=0,
                Fnum=0;
            for(auto it:arr){
                auto it2=history.find(it);
                if(it2==history.end())
                    continue;
                Tnum+=it2->second.Tnum;
                Fnum+=it2->second.Fnum;
            }
            return Tnum>=Fnum;
        }
        bool save(const char * path){
            FILE * fp=fopen(path,"w");
            if(fp==NULL)
                return false;
            
            for(auto it : history){
                fprintf(fp,"%s %d %d\n" ,
                    it.first.c_str(),
                    it.second.Tnum,
                    it.second.Fnum
                );
            }
            fclose(fp);
        }
        bool load(const char * path){
            char buf[4096];
            FILE * fp=fopen(path,"r");
            if(fp==NULL)return false;
            while(!feof(fp)){
                bzero(buf,4096);
                fgets(buf,4096,fp);
                istringstream iss(buf);
                string m;
                int k1 , k2;
                iss>>m;
                iss>>k1;
                iss>>k2;
                history[m].Tnum=k1;
                history[m].Fnum=k2;
            }
            fclose(fp);
        }
};
#endif
