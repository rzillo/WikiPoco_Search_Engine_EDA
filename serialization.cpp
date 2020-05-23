#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

struct Node{
    public:
    string name; 
    bool end;    
    map<string, Node *> child; 
    Node(string n): name(n), end(0){   }
    Node(): end(0){ } 
};
class Tree{
    private:
    Node * pRoot;    
    public:    
    Tree(){
        pRoot = new Node();
    }

    void insert(string full_name){
        stringstream split; 
        split << full_name; 
        string cur_name;
        Node ** pNode = &pRoot; 
        Node * pNew; 
      
        while(split >> cur_name){
            if(!(*pNode)->child[cur_name]){ 
                pNew = new Node(cur_name); 
                (*pNode)->child[cur_name] = pNew;  
            }
            pNode = &((*pNode)->child[cur_name]); 
        }
    }

    void search(string full_name){
        cout << "Procurando por: " << full_name << endl;
        stringstream split;
        split << full_name; 
        string cur_name;
        Node * pNode = pRoot; 
        while(split >> cur_name){
            if(pNode->child[cur_name]){ 
                pNode = pNode->child[cur_name];
            }else{
                cout << "Nome nao encontrado." << endl; 
                return;
            }
        }
        cout << "Nome encontrado." << endl; 
        return;
    }

    void serialize(string file_name){ 
        ofstream file;
        file.open(file_name);
        Node * pNode = pRoot;
        run_serialize(pNode, file); 
    }

    void run_serialize(Node * pCur, ofstream & file){
        file << pCur->name << " "; 
        for(auto const& item : pCur->child){             
            cout << item.first << endl;
            run_serialize(item.second, file); 
        }
        
        file << ") "; 
    }
    
    void diserialize(string file_name){
        ifstream file; 
        string line; 
        file.open(file_name); 
        getline(file, line);
        Node ** pNode = &pRoot; 
        stringstream split; 
        split << line; 
        run_pRoot_diserialize(pNode, split);
    }

    void run_pRoot_diserialize(Node ** pNode, stringstream & split){
        string cur_name; 
        while(split >> cur_name){
        
            if(run_diserialize(pNode, cur_name, split)) break;
        }
    }

    bool run_diserialize(Node ** pNode, string cur_name, stringstream  & split){
        if(cur_name == ")") return 1;
        Node * pNew = new Node(cur_name); 
        (*pNode)->child[cur_name] = pNew; 
        pNode = &(*pNode)->child[cur_name]; 
        while(split >> cur_name){ 
            if(run_diserialize(pNode, cur_name, split)) break;
        }
        return 0;
    }

};
int main(){
    Tree nomes;
    //nomes.insert("giovani valdrighi");
    //nomes.insert("giovani souza");
    //nomes.insert("rener oliveira");
    //nomes.serialize("second.txt");
    nomes.diserialize("second.txt");  
    return 0;
}
