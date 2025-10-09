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
#include "RNSetTree.h"



static void imprimir_inorden(const std::list<int>& lst, const std::string& nombre) {
    std::cout << nombre << " (inorden): ";
    if (lst.empty()) {
        std::cout << "(vacío)";
    } else {
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            std::cout << *it;
            if (std::next(it) != lst.end()) std::cout << ", ";
        }
    }
    std::cout << "\n";
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

        // -----------------------------------------------------------------
        // BST
        ArbolBinario bst;
        long long ops_bst = 0;
        {
            std::ifstream f1(file);
            if(!f1){
                std::cerr << "No se pudo abrir: " << file << "\n";
                continue;
            }
            std::string op; int val;
            std::clock_t t0 = std::clock();
            while (f1 >> op >> val) {
                if (op == "A" || op == "a") bst.insertar(val);
                else if (op == "E" || op == "e") bst.eliminar(val);
                ++ops_bst;
            }
            std::clock_t t1 = std::clock();
            double tbst = (t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

        }

    
        double t_bst_ms = 0.0;
        {
            ArbolBinario bst_tmp; // para volver a medir limpiamente
            std::ifstream f1b(file);
            std::string op; int val;
            std::clock_t t0 = std::clock();
            while (f1b >> op >> val) {
                if (op == "A" || op == "a") bst_tmp.insertar(val);
                else if (op == "E" || op == "e") bst_tmp.eliminar(val);
            }
            std::clock_t t1 = std::clock();
            t_bst_ms = (t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
         
        }

        // -----------------------------------------------------------------
        // AVL
        ArbolAVL avl;
        long long ops_avl = 0;
        double t_avl_ms = 0.0;
        {
            std::ifstream f2(file);
            if(!f2){
                std::cerr << "No se pudo abrir: " << file << "\n";
                continue;
            }
            std::string op; int val;
            std::clock_t t0 = std::clock();
            while (f2 >> op >> val) {
                if (op == "A" || op == "a") avl.insertar(val);
                else if (op == "E" || op == "e") avl.eliminar(val);
                ++ops_avl;
            }
            std::clock_t t1 = std::clock();
            t_avl_ms = (t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
        }

        // -----------------------------------------------------------------
        // RN (set) usando RNSetTree
        RNSetTree<> rn;
        long long ops_rn = 0;
        double t_rn_ms = 0.0;
        {
            std::ifstream f3(file);
            if(!f3){
                std::cerr << "No se pudo abrir: " << file << "\n";
                continue;
            }
            std::string op; int val;
            std::clock_t t0 = std::clock();
            while (f3 >> op >> val) {
                if (op == "A" || op == "a") rn.insert(val);
                else if (op == "E" || op == "e") rn.erase(val);
                ++ops_rn;
            }
            std::clock_t t1 = std::clock();
            t_rn_ms = (t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
        }

        // -----------------------------------------------------------------
        // Comparación
        std::list<int> in_bst, in_avl, in_rn;
        bst.inorder(in_bst);
        avl.inorder(in_avl);
        rn.inorder(in_rn);

        bool bst_ok = (in_bst == in_rn);
        bool avl_ok = (in_avl == in_rn);

        // Salida en consola
        std::cout << std::left << std::setw(12) << "BST"      << "  "
                  << std::setw(12) << ops_bst                 << "  "
                  << std::setw(10) << bst.size()              << "  "
                  << std::setw(12) << t_bst_ms                << "  "
                  << (bst_ok ? "true" : "false") << "\n";

        std::cout << std::left << std::setw(12) << "AVL"      << "  "
                  << std::setw(12) << ops_avl                 << "  "
                  << std::setw(10) << avl.size()              << "  "
                  << std::setw(12) << t_avl_ms                << "  "
                  << (avl_ok ? "true" : "false") << "\n";

        std::cout << std::left << std::setw(12) << "RN (set)" << "  "
                  << std::setw(12) << ops_rn                  << "  "
                  << std::setw(10) << rn.size()               << "  "
                  << std::setw(12) << t_rn_ms                 << "  "
                  << "NA" << "\n";

        std::cout << "---------------------------------------------------------------\n\n";

            std::cout << "\nRecorridos inorden:\n";
        imprimir_inorden(in_bst, "BST");
        imprimir_inorden(in_avl, "AVL");
        imprimir_inorden(in_rn,  "RN");
        

        // Salida CSV
        csv << file << ",BST,"     << ops_bst << "," << bst.size() << "," << t_bst_ms << "," << (bst_ok ? "true":"false") << "\n";
        csv << file << ",AVL,"     << ops_avl << "," << avl.size() << "," << t_avl_ms << "," << (avl_ok ? "true":"false") << "\n";
        csv << file << ",RN,"      << ops_rn  << "," << rn.size()  << "," << t_rn_ms  << ",NA\n";


   
    }

    
    csv.close();
    return 0;
}
