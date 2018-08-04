# -*- coding:utf-8 -*-
import spider

class parser(spider.BDTB):
    def __init__(self,baseUrl,seeLZ):
        spider.BDTB.__init__(self,baseUrl,seeLZ)
        #行号
        self.rawid=1
        #上一选项贴标记
        self.lastSelection=None
    
    #########################
    #use AI begin
    #是正确答案
    def isTrue(self,raw):
        pass
    #是选项
    def isSelection(self,raw):
        pass
    #获取行号
    def getSelectionId(self,raw):
        pass
    #use AI end
    #########################

    #############################
    #IO begin
    #内容，行号
    def contentAppend(self,raw,num):
        pass
    #内容，行号
    def questionAppend(self,raw,num):
        pass
    #内容，行号，回答行号，是否继续
    def answerAppend(self,raw,num,ansnum,resuming):
        pass
    #IO end
    #############################
    
    def canResume(self,raw):
        return self.isTrue(raw)

    def getPost(self,post):
        #为空
        if post == None:
            return
        #按行读取
        raws=post.splitlines()
        
        #之前不存在选项
        #即此贴可能是问题
        if self.lastSelection==None:
            for raw in raws:
                #如果是选项
                if self.isSelection(raw):
                    id=self.getSelectionId(raw)
                    #如果记录为空，就初始化为字典
                    if self.lastSelection==None:
                        self.lastSelection={}
                    #记录当前位置
                    self.lastSelection[id]=self.rawid
                    #加入
                    self.questionAppend(raw,self.rawid)
                else:
                    self.contentAppend(raw,self.rawid)
                self.rawid=self.rawid+1
        #之前存在选项，此贴是答案
        else:
            selectFlag=False
            selectText=""
            selectAnsId=None
            for raw in raws:
                #如果是选项
                if self.isSelection(raw):
                    if selectFlag:
                        self.answerAppend(self , selectText , self.rawid , self.lastSelection[selectAnsId] , self.canResume(selectText))
                        selectText=""
                        selectAnsId=None
                    selectFlag=True
                    selectAnsId=self.getSelectionId(raw)
                    selectText=raw
                else:
                    if selectFlag:
                        selectText+="<br>"
                        selectText+=raw
                    else:
                        self.contentAppend(raw,self.rawid)
                        self.rawid=self.rawid+1
            if selectFlag:
                self.answerAppend(self , selectText , self.rawid , self.lastSelection[selectAnsId] , self.canResume(selectText))
                self.rawid=self.rawid+1
                #清空选项
                self.lastSelection=None
