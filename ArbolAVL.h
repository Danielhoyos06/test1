#pragma once
#include <cstddef>
#include <algorithm>
#include <list>

class ArbolAVL {
public:
    ArbolAVL(): raiz_(nullptr), n_(0) {}
    ~ArbolAVL() { clear(); }

    void insertar(int k) { raiz_ = insert(raiz_, k); }
    void eliminar(int k) { raiz_ = remove(raiz_, k); }
    std::size_t size() const { return n_; }
    void inorder(std::list<int>& out) const { inorderWalk(raiz_, out); }

private:
    struct Nodo {
        int key, h;
        Nodo* izq;
        Nodo* der;
        explicit Nodo(int k): key(k), h(1), izq(nullptr), der(nullptr) {}
    };
    Nodo* raiz_;
    std::size_t n_;

    static int height(Nodo* x){ return x? x->h : 0; }
    static void update(Nodo* x){ if(x) x->h = 1 + std::max(height(x->izq), height(x->der)); }
    static int balance(Nodo* x){ return x? height(x->izq) - height(x->der) : 0; }

    static Nodo* rotR(Nodo* y){ Nodo* x=y->izq; Nodo* t=x->der; x->der=y; y->izq=t; update(y); update(x); return x; }
    static Nodo* rotL(Nodo* x){ Nodo* y=x->der; Nodo* t=y->izq; y->izq=x; x->der=t; update(x); update(y); return y; }

    static Nodo* insert(Nodo* n, int k){
        if(!n){ return new Nodo(k); }
        if(k<n->key) n->izq = insert(n->izq, k);
        else if(k>n->key) n->der = insert(n->der, k);
        update(n);
        int b = balance(n);
        if(b>1 && k<n->izq->key) return rotR(n);                 // LL
        if(b<-1 && k>n->der->key) return rotL(n);                 // RR
        if(b>1 && k>n->izq->key){ n->izq=rotL(n->izq); return rotR(n);} // LR
        if(b<-1 && k<n->der->key){ n->der=rotR(n->der); return rotL(n);} // RL
        return n;
    }

    static Nodo* minNode(Nodo* n){ while(n && n->izq) n=n->izq; return n; }

    static Nodo* remove(Nodo* n, int k){
        if(!n) return nullptr;
        if(k<n->key) n->izq = remove(n->izq, k);
        else if(k>n->key) n->der = remove(n->der, k);
        else{
            if(!n->izq || !n->der){
                Nodo* t = n->izq ? n->izq : n->der;
                if(!t){ delete n; n=nullptr; }
                else { *n = *t; delete t; }
            }else{
                Nodo* s = minNode(n->der);
                n->key = s->key;
                n->der = remove(n->der, s->key);
            }
        }
        if(!n) return nullptr;
        update(n);
        int b = balance(n);
        if(b>1 && balance(n->izq)>=0) return rotR(n);
        if(b>1 && balance(n->izq)<0){ n->izq=rotL(n->izq); return rotR(n); }
        if(b<-1 && balance(n->der)<=0) return rotL(n);
        if(b<-1 && balance(n->der)>0){ n->der=rotR(n->der); return rotL(n); }
        return n;
    }

    static void inorderWalk(Nodo* n, std::list<int>& out){
        if(!n) return;
        inorderWalk(n->izq, out);
        out.push_back(n->key);
        inorderWalk(n->der, out);
    }

    void freeNode(Nodo* n){
        if(!n) return;
        freeNode(n->izq); freeNode(n->der); delete n;
    }

    void clear(){ freeNode(raiz_); raiz_=nullptr; n_=0; }
};
