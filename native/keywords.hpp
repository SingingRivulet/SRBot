#ifndef SRBOY_keywords
#define SRBOY_keywords
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<sstream>
#include<list>
using namespace std;
class keywords{
    public:
        list<string> keywordlist;
        keywords(){}
        ~keywords(){}
        bool getSelectionIdFromWord(const string & str,string & out){
            const char * s=str.c_str();
            int err;
            for(auto it1:keywordlist){
                if(strstr(s , it1.c_str())){
                    out=it1;
                    return true;
                }
            }
            return false;
        }
        void loadKeywordList(const char * path){
            char buf[4096];
            int i,l;
            FILE * fp=fopen(path,"r");
            if(fp==NULL)
                return;
            while(!feof(fp)){
                bzero(buf,4096);
                fgets(buf,4096,fp);
                if((l=strlen(buf))==0)
                    continue;
                for(i=0;i<l;i++){
                    if(buf[i]=='\n' || buf[i]=='\r'){
                        buf[i]='\0';
                        break;
                    }
                }
                keywordlist.push_back(buf);
            }
            fclose(fp);
        }
};
#endif

