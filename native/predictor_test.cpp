#include "predictor.hpp"
std::list<std::string> arr1,arr2,arr3,arr4;
void init(){
    printf("init\n");
    
    arr1.push_back("hello");
    arr1.push_back("world");
    
    arr2.push_back("hello");
    arr2.push_back("machineLearning");
    
    arr3.push_back("world");
    arr4.push_back("machineLearning");
}
void test1(){
    predictor B;
    
    printf("training...\n");
    B.train(arr1,true);
    B.train(arr2,false);
    
    printf("predict1:%d\n",B.predict(arr3)?1:0);
    printf("predict2:%d\n",B.predict(arr4)?1:0);
    
    printf("save model\n");
    B.save("test.pdr");
}
void test2(){
    predictor B;
    
    printf("load model\n");
    B.load("test.pdr");
    
    printf("predict1:%d\n",B.predict(arr3));
    printf("predict2:%d\n",B.predict(arr4));
}
int main(){
    init();
    test1();
    test2();
    return 0;
}
