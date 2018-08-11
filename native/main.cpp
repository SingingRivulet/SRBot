#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "predictor.hpp"
#include "keywords.hpp"
#include "cppjieba/Jieba.hpp"

cppjieba::Jieba * jieba=NULL;
predictor P;
keywords K;
bool trainMode;
void getList(const string & str , list<string> & words){
    if(jieba==NULL)
        return;
    vector<string> buf;
    jieba->CutForSearch(str, buf);
    words.clear();
    for(auto it:buf){
        words.push_back(it);
    }
}

extern "C"{
    void load(){
        const char* const DICT_PATH = "../dict/jieba.dict.utf8";
        const char* const HMM_PATH = "../dict/hmm_model.utf8";
        const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
        const char* const IDF_PATH = "../dict/idf.utf8";
        const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";
        P.load("predictor");
        K.loadKeywordList("keywords.txt");
        jieba=new cppjieba::Jieba(
            DICT_PATH,
            HMM_PATH,
            USER_DICT_PATH,
            IDF_PATH,
            STOP_WORD_PATH);
        if((access("training",F_OK))!=-1)
            trainMode=true;
        else
            trainMode=false;
    }
    void quit(){
        if(trainMode)
            P.save("predictor");
        delete jieba;
    }
    int parser_isTrue(const char * s){
        string str=s;
        list<string> words;
        getList(str,words);
        if(trainMode){
            printf("%s ",str.c_str());
            char m=0;
            scanf("%c",&m); 
            printf("\n");
            bool res=(m=='1');
            P.train(words,res);
            return res ? 1: 0;
        }else{
            return P.predict(words) ? 1 : 0;
        }
    }
    vector<string> buffer_words;
    string buffer_str;
    bool   buffer_status;
    string buffer_id;
    
    #define updatebuffer \
        string s=str; \
        if(s!=buffer_str){ \
            if(jieba==NULL){ \
                printf("object jieba no found\n"); \
                return -1; \
            } \
            jieba->CutForSearch(s,buffer_words); \
            buffer_str=s; \
            buffer_status=K.getSelectionIdFromWord(buffer_words[0],buffer_id); \
        }
    
    int parser_isSelection(const char * str){
        updatebuffer;
        return buffer_status ? 1 : 0;
    }
    int parser_getSelectionId(const char * str,char * out,int len){
        updatebuffer;
        if(buffer_status){
            snprintf(out,len,"%s",buffer_id.c_str());
        }
        return 0;
    }
    
    #undef updatebuffer
}
