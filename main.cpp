#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <ctime>
#include <iomanip>
#include <string>
#include <algorithm>
#include "ArbolBinario.h"
#include "ArbolAVL.h"

static bool equals_list_set(const std::list<int>& l, const std::set<int>& s){
    if(l.size()!=s.size()) return false;
    auto itl=l.begin(); auto its=s.begin();
    for(; itl!=l.end(); ++itl, ++its) if(*itl != *its) return false;
    return true;
}

int main(int argc, char* argv[]){
    if(argc<2){
        std::cout << "Uso: ./app archivo1.txt [archivo2.txt ...]\n";
        return 1;
    }

    std::ofstream csv("resultados.csv");
    csv << "file,structure,ops,nodes,time_ms,equal_to_rn\n";

    for(int i=1;i<argc;i++){
        std::string file = argv[i];
        std::cout << "Archivo: " << file << "\n";
        std::cout << "---------------------------------------------------------------\n";
        std::cout << std::left << std::setw(12) << "Estructura" << "  "
                  << std::setw(12) << "Operaciones" << "  "
                  << std::setw(10) << "Nodos"       << "  "
                  << std::setw(12) << "Tiempo(ms)"  << "  "
                  << "Igual a RN\n";
        std::cout << "---------------------------------------------------------------\n";

        // BST
        ArbolBinario bst;
        std::ifstream f1(file);
        long long ops=0; std::string op; int val;
        std::clock_t t0=std::clock();
        while(f1>>op>>val){ if(op=="A") bst.insertar(val); else bst.eliminar(val); ops++; }
        std::clock_t t1=std::clock();
        double tbst=(t1-t0)*1000.0/CLOCKS_PER_SEC;

        // AVL
        ArbolAVL avl;
        std::ifstream f2(file); ops=0;
        t0=std::clock();
        while(f2>>op>>val){ if(op=="A") avl.insertar(val); else avl.eliminar(val); ops++; }
        t1=std::clock();
        double tavl=(t1-t0)*1000.0/CLOCKS_PER_SEC;

        // RN (set)
        std::set<int> rn;
        std::ifstream f3(file); ops=0;
        t0=std::clock();
        while(f3>>op>>val){ if(op=="A") rn.insert(val); else rn.erase(val); ops++; }
        t1=std::clock();
        double trn=(t1-t0)*1000.0/CLOCKS_PER_SEC;

        // Comparación
        std::list<int> in_bst, in_avl;
        bst.inorder(in_bst); avl.inorder(in_avl);
        bool bst_ok = equals_list_set(in_bst, rn);
        bool avl_ok = equals_list_set(in_avl, rn);

        std::cout << std::left << std::setw(12) << "BST"     << "  " << std::setw(12) << ops << "  " << std::setw(10) << bst.size() << "  " << std::setw(12) << tbst << "  " << (bst_ok?"true":"false") << "\n";
        csv << file << ",BST," << ops << "," << bst.size() << "," << tbst << "," << (bst_ok?"true":"false") << "\n";

        std::cout << std::left << std::setw(12) << "AVL"     << "  " << std::setw(12) << ops << "  " << std::setw(10) << avl.size() << "  " << std::setw(12) << tavl << "  " << (avl_ok?"true":"false") << "\n";
        csv << file << ",AVL," << ops << "," << avl.size() << "," << tavl << "," << (avl_ok?"true":"false") << "\n";

        std::cout << std::left << std::setw(12) << "RN (set)"<< "  " << std::setw(12) << ops << "  " << std::setw(10) << rn.size() << "  " << std::setw(12) << trn << "  " << "NA" << "\n";
        csv << file << ",RN," << ops << "," << rn.size() << "," << trn << ",NA\n";

        std::cout << "---------------------------------------------------------------\n\n";
    }
    csv.close();
    return 0;
}
