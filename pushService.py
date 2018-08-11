# -*- coding:utf-8 -*-
import postParser

class pusher(postParser.parser):
    def __init__(self,baseUrl,seeLZ):
        postParser.parser.__init__(self,baseUrl,seeLZ)
        self.children=None
        self.postId=None
        self.title=""
        self.mainContent=""
        self.questionMode=False
        self.answering=None
    
    def getPostTitle(self,t):
        self.title=t
    
    ############################
    #sendpost begin
    
    #创建根贴，返回id
    def createRoot(self,title,content):
        pass
    
    #创建分支，返回分支id
    def createFork(self,id,title,content,jump):
        pass
    
    #sendpost end
    ############################
    
    #表示获取到了新的帖子
    def getNewPost(self):
        self.answering=None
        self.questionMode=False
        if self.postId==None:
            if len(self.mainContent)!=0 :
                rid=self.createRoot(self.title,self.mainContent)
                self.children=None
                self.postId=rid
                self.mainContent=""
        
    
    #内容，行号
    def contentAppend(self,raw,num):
        if self.answering==None:
            self.mainContent+=raw
        else:
            if self.children!=None :
                if (type(self.children[self.answering]).__name__ == 'dict') :
                    self.children[self.answering]["content"]+="\n"+raw
                else:
                    self.mainContent+=raw
            else:
                self.mainContent+=raw
    
    def finished(self):
        #在此贴中已经执行过提交
        if self.questionMode :
            return
        if self.children :
            rid=0
            tmpid=0
            for item in self.children:
                if item["resuming"] :
                    if rid==0 :
                        rid=self.createFork(self.postId , item["title"] , item["content"]+"\n"+self.mainContent , None)
                    else:
                        self.createFork(self.postId , item["title"] , item["content"] , rid)
                else:
                    tmpid=self.createFork(self.postId , item["title"] , item["content"] , None)
            self.mainContent=""
            
            if rid!=0 :
                self.postId=rid
            elif tmpid!=0:
                self.postId=tmpid
            #如果两个都为0，说明出错了
            
            #清空children
            self.children={}
    
    #内容，行号
    def questionAppend(self,raw,num):
        self.finished()
        self.questionMode=True
        self.children[num]={"title":raw , "content":raw , "resuming":False}
    
    #内容，行号，回答行号，是否继续
    def answerAppend(self,raw,num,ansnum,resuming):
        #修改状态机
        self.answering=ansnum
        if self.children!=None :
            if (type(self.children[ansnum]).__name__ == 'dict') :
                self.children[ansnum]["content"]+="\n"+raw
                self.children[ansnum]["resuming"]=resuming
