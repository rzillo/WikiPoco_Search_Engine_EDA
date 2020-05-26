#include <iostream>
#include <vector> 
#include <string>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
const int ALPHABET_SIZE = 36; 

class Node {
public:
  Node* pChilds[36];
  std::vector<int> indexes;
  Node() {
    for (int i=0; i<36; i++)
      pChilds[i] = nullptr;
  }
};

class Trie {
protected:
  Node* pRoot;

  int valor(char c) {    
    int x = c;
    if ((x<=122) && (x >= 97)) // a e z
      return (x - 97);
    else if ((x <=90) && (x >= 65)) // A e Z
      return (x - 65);
    else if ((x <= 57) && (x >= 48)) //comparação de 0 a 9
      return (x-22);
    else if (x == -59 || x == -61) //o próx símbolo talvez seja lido
      return x;
    else //demais símbolos.
      return -1;
  }
  int valor(int x, char c2) {
    
       int y = c2;
    
    if (x == -61) {
      if ((y >= -96 && y <= -91) || (y >= -128 && y <= -123)) //diferentes A's
	return 0;
      else if (y == -89 || y == -121) //ç
	return 2;
      else if (y == -112 || y == -80) //ð
	return 3;
      else if ((y >= -88 && y <= -85) || (y >= -120 && y <= -117)|| //diferentes E's
	       (y == -90) || (y == -122)) //æ e Æ
	return 4;
      else if ((y >= -84 && y <= -81) || (y >= -116 && y <= -113)) //diferentes I's
	return 8;
      else if (y == -79 || y == -111) //ñ
	return 13;
      else if ((y >= -78 && y <= -74) || (y >= -110 && y <= -106)||
	       (y==-72) || (y == -104))//diferentes O's
	return 14;
      else if ((y >= -71  && y <= -68) || (y >= -103 && y <= -100)) //diferentes U's
	return 20;
      else if ((y == -67) || (y == -65) || (y == -99) || (y==-72)) //diferentes Y's
	return 24;
      else // demais símbolos
	return -1;
    } else { //x = -59 ou qualquer coisa fora que apareça ao acaso
      if (y == -95 || y == -96) //š
	return 18;
      else if (y == -66 || y == -67) //ž
	return 25;
      else if (y == -72) //Ÿ
	return 24;
      else if (y == 108 || y==109 || y==107 || y==115 || y == 44 || y==0) //Å
	return 0;
      else if (y == 102) //Ø
	return 14;
      else
	return -1;
    }  
  }
