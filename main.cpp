#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <map>
#include <set>
#include <stdio.h>
#include <glib.h>
#include <vector>

using namespace std;

int keyRead(const char* wrd) {
    int s=0;
    unsigned char z;
    while (*wrd!='\0') {
        //int x = (int)*wrd;
        z = (unsigned char)*wrd++;
        s+=z-65;
    }
    return s;
}

int maxDiv(int n) {
    for (int i=(n/2)+1;i>=1;i--)
        if ( (n%i)==0) return i;
    return 1;
}

void printMaps(ofstream &where, map<int,set<string>> &words) {
    for (map<int,set<string> >::iterator it=words.begin(); it!=words.end(); it++) {

        where << it->first << ":  " <<endl;
        for (set<string>::iterator vit=(it->second).begin();vit!=(it->second).end();vit++) {
            where << " - " << *vit << endl;
        }

    }

}


vector<string> getWordsOfText(string name) {
    ifstream inp(name);
    string arr;
    vector<string> v;
    while (!inp.eof()) {
        getline(inp,arr,' ');
        v.push_back(arr);
        }
    inp.close();
    cout << v.size();
    for (int i=30;i<100;i++)
    cout << v[i] << " - ";
    return v;
}



int main()

{
    vector<string> res = getWordsOfText("hovory.txt");
    //for(int i=0;i<res.size();i++) cout << res[i]
/*

    map<int,set<string>> words;
    map<int,set<string> >::iterator it;
    pair<int,set<string> > intVec;

    string arr = "kápě";
    int del;
    cout << keyRead(arr.c_str()) << "... ahoj " <<endl;
    ifstream inp("ceskapodstjm.txt");
    //ifstream inp("data.txt");
    int i=1;
    while (!inp.eof()) {
        getline(inp,arr);
        arr.resize(arr.size()-1);
        //kod = keyRead(arr.c_str());
        //del = maxDiv(kod);
        del = i++ % 5000 + 1;
        //cout << arr << " ... " << kod << " -> " << del << endl;
        if ( (it =words.find(del) )==words.end()) {
            set<string> el;
            el.insert(arr);
            words.insert(pair<int,set<string> >(del,el));
        } else {
            it->second.insert(arr);
        }
    }
    inp.close();

    ofstream o("result.txt");
    printMaps(o,words);
    o.close();


    */

    //cin >> arr;
    return 0;
}
