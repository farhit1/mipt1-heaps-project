#include "base.h"
#include <climits>
#include <vector>

class BinomialHeap : virtual IHeap {
private:
    struct Node {
        int key;
        std::vector<Node*> children;

        Node() { *this = NULL; }
        Node(int key) :
                key(key),
                children(std::vector<Node*>(0)) {};

        ~Node() {
            for (Node* i : children)
                delete i;
        }
    };

    struct Heap {
        std::vector<Node*> roots;

        Heap() = default;
        Heap(const std::vector<Node*>& list) {
            roots = list;
        }

        Heap(int key) :
                roots(std::vector<Node*>{new Node(key)}) {}

        ~Heap() {
            for (Node* i : roots)
                delete i;
        }
    };

    Node* _nxt(Node* l, Node* r) {
        if (l->key < r->key) {
            l->children.push_back(r);
            return l;
        } else {
            r->children.push_back(l);
            return r;
        }
    }

    Heap* _meld(Heap* l, Heap* r) {
        int maxsize = std::max(l->roots.size(), r->roots.size());
        while (l->roots.size() < maxsize)
            l->roots.push_back(NULL);
        while (r->roots.size() < maxsize)
            r->roots.push_back(NULL);

        while (maxsize > 0 && l->roots.back() == NULL && r->roots.back() == NULL) {
            maxsize--;
            l->roots.pop_back();
            r->roots.pop_back();
        }

        Node* remainder = NULL;
        for (int i = 0; i < maxsize; i++) {
            int notnull = 0;
            if (remainder != NULL)
                ++notnull;
            if (l->roots[i] != NULL)
                ++notnull;
            if (r->roots[i] != NULL)
                ++notnull;

            if (notnull == 0)
                continue;
            if (notnull == 1) {
                if (r->roots[i] != NULL)
                    l->roots[i] = r->roots[i];
                if (remainder != NULL) {
                    l->roots[i] = remainder;
                    remainder = NULL;
                }
                continue;
            }
            if (notnull == 2) {
                Node *n1, *n2;
                if (l->roots[i] == NULL) {
                    n1 = r->roots[i];
                    n2 = remainder;
                }
                if (r->roots[i] == NULL) {
                    n1 = l->roots[i];
                    n2 = remainder;
                }
                if (remainder == NULL) {
                    n1 = l->roots[i];
                    n2 = r->roots[i];
                }
                l->roots[i] = NULL;
                remainder = _nxt(n1, n2);
            }
            if (notnull == 3) {
                remainder = _nxt(r->roots[i], remainder);
            }
        }
        if (remainder != NULL)
            l->roots.push_back(remainder);

        return l;
    }

    Heap* _heap;

public:

    BinomialHeap() :
        _heap(new Heap) {}

    void Insert(int key) {
        _heap = _meld(_heap, new Heap(key));
    }

    int GetMin() {
        int ret = INT_MAX;
        for (auto i : _heap->roots)
            if (i != NULL)
                ret = std::min(ret, i->key);
        return ret;
    }

    void ExtractMin() {
        int ret = INT_MAX;
        size_t where;
        for (int i = 0; i < _heap->roots.size(); i++)
            if (_heap->roots[i] != NULL && ret > _heap->roots[i]->key) {
                ret = _heap->roots[i]->key;
                where = i;
            }

        Heap* tmp = new Heap(_heap->roots[where]->children);
        _heap->roots[where] = NULL;
        _heap = _meld(_heap, tmp);
    }

    void Meld(BinomialHeap& other) {
        _heap = _meld(_heap, other._heap);
        other._heap = new Heap();
    }

    ~BinomialHeap() {
        delete _heap;
    }
};