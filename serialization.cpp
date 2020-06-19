#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include <string>
#include "code.hpp"


using namespace std;

int main () {
  
  Trie teste;
  ifstream names("files");
  ifstream file;
  string s;
  string s1;
  string word;
  int index;
  int docs = 0;
  
  auto t0 = chrono::high_resolution_clock::now();
  while (getline(names, s)) {
    file.open("C:/Users/renan/Desktop/final_test/wiki_files/" + s +"(conv)");
    cout << "lendo arquivo " << s << "\n";
    while (getline(file, s1)) {
      if (s1.substr(0,4) == "<doc") {
	docs++;
	index = get_number(s1);
      } else {
	if (s1.substr(0,5) != "</doc" && s1.substr(0,5) != "ENDOF"){
	  for (string::iterator it = s1.begin(); it != s1.end(); ++it) {
	    if (*it == ' ' ||*it == '-' ||*it == '.'||*it == ',' || *it == ';'|| *it == ':'||*it == '?'|| *it == '!') {	      
	      teste.insert(word, index);
	      word.clear();
	    } else {
	      word += *it;
	    }
	  }
	}
      }
    }
    file.close();
  }
  word.clear();
  chrono::duration<double> dt = chrono::high_resolution_clock::now() - t0;
  cout << "feito! \n";
  cout << "Foram indexados " << docs << " artigos em " << dt.count() <<" segundos\n\n";
  cout << "Serializando ...\n";
  t0 = chrono::high_resolution_clock::now();
  fstream file2;
  file2.open("serial_trie");
  teste.serialize(file2);
  dt = chrono::high_resolution_clock::now() - t0;
  cout << "O processo levou " << dt.count() << " segundos.\n";
  return 0;
} 
