#include <iostream>
#include "keywords.hpp"
keywords K;
int main(){
    K.loadKeywordList("1.txt");
    string s,r;
    cin >> s;
    K.getSelectionIdFromWord(s,r);
    cout<<"\n";
    cout<<r;
    return 0;
}
