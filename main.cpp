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
#include <time.h>
#include <stdlib.h>
#include <sstream>

#define DEBUG

using namespace std;

char separators[] = { ' ', '\n', ',', ';', '\r', '\t' };
const int MAXLENGTH = 100;
const int NUMGRP = 5; //number of numeric groups
const int GRP1 = -1;
const int GRP2 = -2;
const int GRP3 = -3;
const int GRP4 = -4;
const int GRPSUM = -5;
const int GRPPROD = -6;
const int GRPPOINT = -7;


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
            num = i++ % NUMGRP + 1;

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

inline bool isSeparator(char ch) {
    for (unsigned int j=0;j<sizeof(separators);j++) {
        if (ch==separators[j]) {
            return true;
        }
    }
    return false;
}


vector<string> divSentence(string sent) {
    vector<string> words;
    unsigned int u=0, i;
    while (isSeparator(sent[u])) u++;
    for (i=u;i<sent.size();i++) {

        if (isSeparator(sent[i]) ) {
            if (i>u) words.push_back(sent.substr(u,i-u));
            u=i+1;
        }
    }
    if (u<i) words.push_back(sent.substr(u));
    return words;
}




//divide back-file into words
vector<string> loadBackFile(string fname) {
    ifstream inp(fname);
    vector<string> wds,temp;
    string lin;
    while (!inp.eof()) {
        getline(inp,lin);
        temp = divSentence(lin);
        wds.insert(wds.end(), temp.begin(), temp.end());
    }
    return wds;
}

int nEqualChar(const char *str1, const char *str2) {
    int s=0;
    while ((*str1)&&(*str2)) {
        if ((*str1++)==(*str2++)) {
            s++;
        } else {
            return s;
        }
    }
    return s;
}

int isLike(const char *str1, const char *str2) {
    int n=0,mx=0;
    int lng1 = strlen(str1);
    int lng2 = strlen(str2);
    while (*str2) {
        if ((n=nEqualChar(str1,str2))>mx) {
            mx=n;
            if (mx==lng1) return mx+MAXLENGTH; //entire word found
        }
        str2++;lng2--;
        if (mx>=lng2) return mx; //no more
    }
    return mx;
}

int findBestWord (string wrdForCoding, vector<string> & backwords, int &index) {
    int n=0, mx = 0;
    for (unsigned int i=0;i<backwords.size();i++) {
            if ((n=isLike(wrdForCoding.c_str(),backwords[i].c_str()))>mx) {
                mx=n;index=i;
                if (mx>MAXLENGTH) return mx;
            }
    }
    return mx;
}

string getRandomElement(set<string> &from) {
    set<string>::iterator it = from.begin();
    advance(it,rand() % from.size());
    return *it;

}

string encryptNumber(int num, map<int,set<string> > &codeConf) {
    stringstream res;
    int prd = num/NUMGRP;
    int rest = num % NUMGRP;
    if (prd>0) {
        res << getRandomElement(codeConf[prd])     << " "
            << getRandomElement(codeConf[GRPPROD]) << " "
            << getRandomElement(codeConf[NUMGRP]);
        if (rest>0) {
            res << " " << getRandomElement(codeConf[GRPSUM]) << " ";
        }
    }
    if (rest>0) res << getRandomElement(codeConf[rest]);
    return res.str();
}

string encryptSpecialChar(char ch, map<int,set<string> > &codeConf ) {
    stringstream res;
    res << getRandomElement(codeConf[GRP4]) << " "
        << getRandomElement(codeConf[ch]);
    return res.str();
}


string encryptWord(string wrdForCoding, vector<string> &backwords, map<int,set<string> > &codeConf) {

    stringstream res;
    string s1;
    int n,i;
    {
        n=findBestWord(wrdForCoding,backwords,i);
        if (n==0) {
                res << encryptSpecialChar(wrdForCoding[0], codeConf) << " ";
                if (wrdForCoding.size()==1) wrdForCoding=""; else wrdForCoding = wrdForCoding.substr(1);
        } else {
                res << encryptNumber(i, codeConf)  << " "; //position
                if (n>MAXLENGTH) {
                    s1 = wrdForCoding;
                    wrdForCoding = "";
                } else {
                    s1 = wrdForCoding.substr(0,n-1);
                    wrdForCoding = wrdForCoding.substr(n);
                }

                if (s1!=backwords[i]) { //only part from backword
                    size_t found = backwords[i].find(s1);
                    #ifdef DEBUG
                        if (found==string::npos) {cout << "problem in encryptWord";exit(1);}
                    #endif // DEBUG
                    res << encryptNumber(found, codeConf)                       << " "
                        << encryptNumber(s1.size()+found,codeConf)   << " ";
                }
                res << getRandomElement(codeConf[GRP1]);
        }
    } while (wrdForCoding.size()>0);
    return res.str();
}


int main()

{

    srand (time(NULL)); //random choose from set of strings...
    map<int,set<string> > codeConf;
    if (existsFile("conf.txt")) {
        codeConf = openCodedConfiguration("conf.txt");
    } else {
        if (existsFile("ceskaslova.txt"))
            codeConf = createCodedConfiguration("ceskaslova2.txt", "conf.txt");
        else {
            cout << "Can't find conf.txt or ceskaslova.txt" << endl;exit(0);
        }
    }
    vector<string> backwords = loadBackFile("dopis2.txt");
    string slovo("hotoka");
    string x = encryptWord(slovo,backwords,codeConf);// << endl;
    //cout << encryptNumber(x,codeConf) << endl;


    return 0;

}
