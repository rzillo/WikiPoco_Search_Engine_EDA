

using namespace std;
 int valor(char c) {
    /*     
     N�o parece ter solu��o t�o trivial pra trabalhar com caracteres que usam
     mais de um byte e n�o s�o UTF8 (a maior parte das solu��es simples exigem
     algum recurso externo). Resolvemos modificar os caracteres acentuados, letras 
     mai�sculas e alfabetos distintos para suas letras min�sculas correspondentes.
     Por exemplo com a letra 'e', a quarta letra partindo do zero (a):    
    '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�' -> 4;    
     Os demais caracteres que n�o s�o nem letras e nem n�meros recebem o valor -1.
    */
    int x = c;
    if ((x <= 122) && (x >= 97)) // Letras min�sculas
      return (x - 97);
    else if ((x <= 90) && (x >= 65)) // Letras mai�sculas
      return (x - 65);
    else if ((x <= 57) && (x >= 48)) // N�meros
      return (x - 22);
    else if (x == -59 || x == -61) // O pr�ximo s�mbolo talvez seja lido
      return x;
    else // demais s�mbolos.
      return -1;
  }
  
  int valor(int x, char c2) {
    /**************************
    Os que vem do ISO 8859-1 s�o da forma x = -61 e c2 = n;
    Do ISO 8859-15 s�o x = -59 e c2 = n;
    **************************/
    int y = c2;
    
    if (x == -61) {
      if ((y >= -96 && y <= -91) || (y >= -128 && y <= -123)) // A's acentuados
	return 0;
      else if (y == -89 || y == -121) //�
	return 2;
      else if (y == -112 || y == -80) //�
	return 3;
      else if ((y >= -88 && y <= -85) || (y >= -120 && y <= -117)|| // E's acentuados
	       (y == -90) || (y == -122)) //� e �
	return 4;
      else if ((y >= -84 && y <= -81) || (y >= -116 && y <= -113)) // I's acentuados
	return 8;
      else if (y == -79 || y == -111) //�
	return 13;
      else if ((y >= -78 && y <= -74) || (y >= -110 && y <= -106)||
	       (y==-72) || (y == -104))// O's acentuados
	return 14;
      else if ((y >= -71  && y <= -68) || (y >= -103 && y <= -100)) // U's acentuados
	return 20;
      else if ((y == -67) || (y == -65) || (y == -99) || (y == -72)) // Y's acentuados
	return 24;
      else // demais s�mbolos
	return -1;
    } else { //x = -59 e demais casos
   	  if (y == 108 || y == 109 || y == 107 || y == 115 || y == 44 || y == 0) //�
	return 0;
	  else if (y == 102) //�
	return 14;    
      else if (y == -95 || y == -96) //�
	return 18;      
      else if (y == -72) //�
	return 24;
	  else if (y == -66 || y == -67) //�
	return 25;      
      else
	return -1;
    }  
  }






int get_number (string tag) {
  //Pega o n�mero do �ndice da p�gina do wiki-corpus.
  
  string::iterator it = tag.end();
  while (*it != ' ')
    --it;
  --it;
  --it;
  int x = 0;
  int i = 1;
  while (*it != '"'){
    x += (*it - 48) * i; // os d�gitos v�o de 48 a 57 em ASCII
    i *= 10;
    --it;
  }
  return x;
}

int get_len (string tag) {
  //Pega o tamanho da p�gina do wiki-corpus  
  
  string::iterator it = tag.end();
  while (*it != '"')
    --it;
  --it;
  int x = 0;
  int i = 1;
  while (*it != '"'){
    x += (*it - 48) * i; // os d�gitos v�o de 48 a 57 em ASCII
    i *= 10;
    --it;
  }
  return x;
}

string get_title (int index) {
  //Dado um �ndice, acha o t�tulo do documento com esse �ndice.  
  fstream file;
  stringstream temp;
  temp << "wiki_files/englishText_";
  temp << index - (index%10000) << "_" << index - (index%10000) + 10000  << "(conv)";
  file.open(temp.str());  
  string s;
  int ind;
  string title;
  while (getline(file,s)) { //s estar� sempre em uma tag seguindo a condi��o do while
        ind = get_number(s); 
    int lines = get_len(s);
    if (ind != index) {
      for (int i = 0; i < lines; i++)
	getline(file,s); 
    } else { // Achou a tag, que est� armazenada em s.
      string::iterator it = s.begin();
      while (*it != ' ') 
	it++; 
      it++;
      while (*it != ' ') 
	it++;
      advance(it, 8); 
      while (*it != '"') {
	title.push_back(*it);
	it++;
      }
      return title;
    }
  }
  return "";
}
  

class Node {
public:
  Node* pChilds[36];
  vector<int> indexes;
  Node() {
    for (int i=0; i<36; i++)
      pChilds[i] = nullptr;
  }
};



class Trie {
protected:
  Node* pRoot;
    
public:
  Trie () {
    pRoot = new Node();
  }


  vector<int> intersection(vector<int> docs1, vector<int> docs2) {
    //Para que o algoritmo funcione de maneira adequada constru�mos
    //a �rvore com os �ndices dos vetores ordenados.    
    vector<int>::iterator it1 = docs1.begin();
    vector<int>::iterator it2 = docs2.begin();
    vector<int> vec;
    while (it1 != docs1.end() && it2 != docs2.end()) {
      if (*it1 > *it2) {
	it2++;
      } else if (*it2 > *it1) {
	it1++;
      } else {
	vec.push_back(*it1);
	it1++;
	it2++;
      }
    }
    return vec;
  }
  
  void insert(string word, int index) {
    Node* pNode;
    pNode = pRoot;
    int x;
    for (string::iterator it = word.begin(); it != word.end(); ++it) {       
      x = valor(*it);
      if (x >= 0) {
	if (pNode->pChilds[x] == nullptr)
	  pNode->pChilds[x] = new Node();
	pNode = pNode->pChilds[x];
      } else if (x != -1) { 	
	++it;
	int y = *it;	
	y = valor(x, *it);	
	if (y >= 0) {
	  if (pNode->pChilds[y] == nullptr)
	    pNode->pChilds[y] = new Node();
	  pNode = pNode->pChilds[y];
	}
      }	
    }
    //pNode se torna ponteiro ao n� da palavra a ser adicionada
    //quando o for termina.
    
    //Verifica��o do �ndice para ver se est� no n� ou se o n� � o root. 
    if (pNode == pRoot)
      return;    
    if (!(pNode->indexes).empty())
      if ((pNode->indexes).back() == index){	
	return;
      }
    (pNode->indexes).push_back(index);
    (pNode->indexes).reserve((pNode->indexes).size());
  }
    
  vector<int> query_oneword (string word) {
    // A inten��o com a query � retornar um vetor vazio caso o n� n�o seja encontrado.
    //query para uma palavra 
    Node* pNode = pRoot;
    int x;
    for (string::iterator it = word.begin(); it != word.end(); ++it) {
      x = valor(*it);
      if (x >= 0) {
	if (!(pNode->pChilds[x])) {
	  cout << "Essa palavra n�o foi encontrada em nenhum artigo. Por favor, insira 'n' e tente novamente \n";
	  vector<int> z;
	  return z;
	}
	pNode = pNode->pChilds[x];
      } else if (x != -1) {
	int y;
	++it;
	y = valor(x, *it); // s� pode ser >=0 ou -1
	if (y >= 0) {
	  if (!(pNode->pChilds[y])) {
	    cout << "Essa palavra n�o foi encontrada em nenhum artigo. Por favor, insira 'n' e tente novamente \n";
	    vector<int> z;
	    return z;
	  }
	  pNode = pNode->pChilds[y]; 
	} else {
	  cout << "Essa palavra n�o foi encontrada em nenhum artigo. Por favor, insira 'n' e tente novamente \n";
	  vector<int> z;
	  return z;
	}
      } else { //x == -1
	cout << "Essa palavra n�o foi encontrada em nenhum artigo. Por favor, insira 'n' e tente novamente \n";
	vector<int> z;
	return z;
      }
    }
    return pNode->indexes;
  }

  vector<int> query (string s) {
    string word;
    vector<int> vec;
    bool multi_w = false;
    for (string::iterator it = s.begin(); it != s.end(); ++it) {
      if (*it == ' ' ||*it == '-' ||*it == '.'||*it == ',' || *it == ';'|| *it == ':'||*it == '?'|| *it == '!') {
	multi_w = true; // para strings com mais de uma palavra devemos nos preocupar com as separa��es;
	if (vec.empty())
	  vec = query_oneword(word); // fazemos as interse��es com os �ndices das palavras.
	else
	  vec = intersection(vec, query_oneword(word));
	word.clear();
      } else {
	word += *it;
      }
    }
    if (!(multi_w)) // a busca pela palavra � feita no caso onde a string s� tem um �nica palavra.
      vec = query_oneword(word);
    else
      vec = intersection(vec, query_oneword(word));
    return vec;
  }

  void serial_rec (Node* pNode, fstream& file , char fonte) {
    if (pNode) {
      file << fonte << "(";
      for (vector<int>::iterator it = pNode->indexes.begin(); it != pNode->indexes.end(); it++) 
	file << *it << ",";
      file << ")";
      for (int i = 0; i < 36; i++){
	//char c = "abcdefghijklmnopqrstuvwxyz0123456789"[i];
	serial_rec(pNode->pChilds[i], file, "abcdefghijklmnopqrstuvwxyz0123456789"[i]);
      }
      file << "$";
    }
  }
  
  void serialize (fstream& file) {
    Node* pNode;
    for (int i = 0; i < 36; i++){
      pNode = pRoot->pChilds[i];
      char c = "abcdefghijklmnopqrstuvwxyz0123456789"[i];
      serial_rec(pNode, file, c);
    }
  }

  void deserialize (fstream& file) {
    Node* pNode;
    string arq;
    string word;    
    int index = 0;
    getline(file, arq);
    for (string::iterator it = arq.begin(); it != arq.end(); it++) {
      if (*it != '(' && *it != '$') {
	word += *it;
      } else if (*it == '(') {
	it++;
	while (*it != ')') {
	  if (*it != ',') {
	    index = (*it - 48) + 10*index;
	  } else { 
	    insert(word, index);
	    index = 0;
	  }
	  it++;
	}
      } else { 
	word.pop_back();
      }
    }
  }

  void display (int index) {
    fstream file;
    stringstream temp;
    temp << "wiki_files/englishText_";
    temp << index - (index%10000) << "_" << index - (index%10000) + 10000  << "(conv)";
    file.open(temp.str());    
    string s;
    int ind;
    while (getline(file,s)) { // 's' estar� sempre em uma tag.
      ind = get_number(s);
      int lines = get_len(s);
      if (ind != index) {
	for (int i = 0; i < lines; i++) // getline chega </doc> no fim do loop.
	  getline(file,s);
      } else {
	for (int i = 0; i < (lines-2); i++) {
	  getline(file,s);
	  cout << s << "\n";
	}
	file.close();
	return;
      }
    }
  }
}; 
