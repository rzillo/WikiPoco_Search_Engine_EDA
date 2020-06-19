#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <chrono>
#include <iomanip>
#include "code.hpp"

using namespace std;

int main () {
  ios::sync_with_stdio(false); //para deixar os iostreams mais rápidos.  
  Trie trie;
  fstream serial ("serial_trie");
  auto t0 = chrono::high_resolution_clock::now(); 
  trie.deserialize(serial);
  serial.close();
  chrono::duration<double> dt = chrono::high_resolution_clock::now() - t0;
  cout << "feito \n\n";
  cout << "A trie foi desserializada em " << dt.count() << " segundos. \n\n";

  
  while (true) {
    string s;
    cout << "Bem vindo ao WikiPoco! O que deseja buscar?\n";
    getline(cin, s); 
    vector<int> pags;
    t0 = chrono::high_resolution_clock::now();
    pags = trie.query(s);
    chrono::duration<double> dt = chrono::high_resolution_clock::now() - t0;
    cout << fixed << setprecision(11) <<"... Um total de " << pags.size() << " páginas (cerca de "
	      << dt.count() << " segundos) \n\n";
    //pags contém os índices das páginas que tem a busca.
    string user_r;
    int j = 0;
    
    while (user_r != "n") {
      
      for (int i = 0; i < 20; i++){
	if (i + 20*j < pags.size())
	  cout << "[" << i+1 << "]" << get_title(pags[i + 20*j]) << "\n";
      }
      
      cout << "Deseja abrir algum artigo?\n"
		<< "[indicador do artigo na tela para ler; 'm' para mostrar outros 20 resultados"
		<< " diferentes, 'v' para voltar para os 20 resultados anteriores"
		<< " e 'n' para realizar uma nova busca]\n";
      getline(cin, user_r);
      if (user_r == "m" || user_r == "M") {
	j++;
      } else if (user_r == "v" || user_r == "V") {
	if (j>0)
	  j--;
      } else if (user_r == "n" || user_r == "N") {
	cout << "ok \n\n";
      } else {
	int ent = 0;
	for (string::iterator it = user_r.begin(); it != user_r.end(); it++) {
	  if (*it < '0' || *it > '9') { //se a entrada não for opção acima
	    user_r = "n"; //e não for número
	    break; //refaz a busca.
	  }
	  ent = (*it - '0') + 10*ent;
	}
	  if (ent > 20 || ent < 0) {
	    user_r = "n";
	  } else {
	    cout << "\n\n";
	    cout << "------------------------------------------------------------------------------------------------\n";
	    trie.display(pags[ent - 1 + 20*j]);
	    cout << "------------------------------------------------------------------------------------------------\n";
	    cout << "\n\n";
	    user_r = "n";
	  }
      }
    }
  }
}
