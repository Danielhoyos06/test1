#pragma once
#include <cstddef>
#include <list>

class ArbolBinario {
public:
    ArbolBinario(): raiz_(nullptr), n_(0) {}
    ~ArbolBinario() { clear(); }

    void insertar(int k) { raiz_ = insert(raiz_, k); }
    void eliminar(int k) { raiz_ = remove(raiz_, k); }
    std::size_t size() const { return n_; }
    void inorder(std::list<int>& out) const { inorderWalk(raiz_, out); }

private:
    struct Nodo {
        int key;
        Nodo* izq;
        Nodo* der;
        explicit Nodo(int k): key(k), izq(nullptr), der(nullptr) {}
    };
    Nodo* raiz_;
    std::size_t n_;

    static Nodo* insert(Nodo* node, int k) {
        if (!node) { return new Nodo(k); }
        if (k < node->key) node->izq = insert(node->izq, k);
        else if (k > node->key) node->der = insert(node->der, k);
        return node;
    }

    static Nodo* minNode(Nodo* node) {
        while (node && node->izq) node = node->izq;
        return node;
    }

    static Nodo* remove(Nodo* node, int k) {
        if (!node) return nullptr;
        if (k < node->key) node->izq = remove(node->izq, k);
        else if (k > node->key) node->der = remove(node->der, k);
        else {
            if (!node->izq) { Nodo* r = node->der; delete node; return r; }
            if (!node->der) { Nodo* l = node->izq; delete node; return l; }
            Nodo* suc = minNode(node->der);
            node->key = suc->key;
            node->der = remove(node->der, suc->key);
        }
        return node;
    }

    static void inorderWalk(Nodo* node, std::list<int>& out) {
        if (!node) return;
        inorderWalk(node->izq, out);
        out.push_back(node->key);
        inorderWalk(node->der, out);
    }

    void freeNode(Nodo* node) {
        if (!node) return;
        freeNode(node->izq);
        freeNode(node->der);
        delete node;
    }

    void clear() { freeNode(raiz_); raiz_ = nullptr; n_ = 0; }
};
