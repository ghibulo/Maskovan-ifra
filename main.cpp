#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <map>
#include <set>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <climits>

#define DEBUG

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

inline bool existsFile (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

void printMaps(ostream &where, map<int,set<string>> &words, int cnt=INT_MAX) {
    int c=0;
    for (map<int,set<string> >::iterator it=words.begin(); it!=words.end(); it++) {

        where << it->first << ":  " <<endl;
        for (set<string>::iterator vit=(it->second).begin();vit!=(it->second).end();vit++) {
            where << " + " << *vit << endl;
        }
        if (++c > cnt) {
            cout << "and so on..."<<endl;
            return;
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


map<int,set<string>> createCodedConfiguration(string filewords, string codconf) {
    map<int,set<string>> words;
    map<int,set<string> >::iterator it;
    pair<int,set<string> > intVec;

    string lin; //line of text file = another word
    int num; //key for group of words
    int siz; //size of word

    ifstream inp(filewords);
    int i=1, j=1;
    while (!inp.eof()) {
        getline(inp,lin);
        lin.resize(lin.size()-1); //msdos format of text file -> delete '\r' character

        if ( (siz = lin.size()) <=4)
            num = -(j++ % 7 + 1); //short words for 7 special groups (product, add, decimal point, numbers, ascii...)
        else
            num = i++ % 5000 + 1;

        if ( (it =words.find(num) )==words.end()) {
            set<string> el;
            el.insert(lin);
            words.insert(pair<int,set<string> >(num,el));
        } else {
            it->second.insert(lin);
        }
    }
    inp.close();
    ofstream o(codconf);
    printMaps(o,words);
    o.close();
    return words;
}

map<int,set<string>> openCodedConfiguration(string filewords) {
    map<int,set<string>> words;
    map<int,set<string> >::iterator it;
    pair<int,set<string> > intVec;

    string lin; //line of text file = another word
    int ngr;

    ifstream inp(filewords);
    map<int,set<string> >::iterator actGroup;
    pair< map<int,set<string> >::iterator, bool> temp;
    while (!inp.eof()) {
        getline(inp,lin);
        if (lin.empty()) continue; //empty lines allowed
        if (lin[0]!=' ') {

            set<string> el;
            try {
            ngr = stoi(lin);
            temp = words.insert(pair<int,set<string> >(ngr,el));
            }
            catch(std::invalid_argument& e){
                cout << "Name of words group must be a number!" << endl;
                cout << "lin=" << lin << "ag=" << actGroup->first << endl;
                exit(1);

            }
            catch(std::out_of_range& e){
                cout << "Invalid range for number-name of groups" << endl;
                exit(2);
            }
            catch(...) {
              // everything else
              cout << "File conf.txt is corrupted." << endl;
                exit(2);
            }
            actGroup = temp.first;
        } else {
            actGroup->second.insert(lin.substr(3));
        }
    }
    inp.close();
    #ifdef DEBUG
    printMaps(cout,words,10);
    #endif
    return words;
}


int main()

{
    //vector<string> res = getWordsOfText("hovory.txt");
    //for(int i=0;i<res.size();i++) cout << res[i]
    //cout << keyRead(arr.c_str()) << "... ahoj " <<endl;
            //kod = keyRead(arr.c_str());
        //del = maxDiv(kod);

    map<int,set<string>> codeConf;
    if (existsFile("conf.txt")) {
        codeConf = openCodedConfiguration("conf.txt");
    } else {
        if (existsFile("ceskaslova.txt"))
            codeConf = createCodedConfiguration("ceskaslova.txt", "conf.txt");
        else {
            cout << "Can't find conf.txt or ceskaslova.txt" << endl;exit(0);
        }
    }


    return 0;

}
