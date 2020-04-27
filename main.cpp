#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
using namespace std;

//#define INT_MIN -2147483648
//#define INT_MAX 2147483647

ifstream in("test.in");
ofstream out ("test.out");

const int m = SHRT_MAX;
bool bloom[m];

unsigned int hash1 ( int x){
    return (abs(x * x - INT_MAX)) % UINT32_MAX;
}
unsigned int hash2 (int x){
    return (abs((x % 1000) * ( x / 1000) - INT_MAX)) % UINT32_MAX;
}
void add( int x){
    for( int i = 0; i < 10; i++){
        unsigned int b = ( hash1(x) + i * hash2(x));
        b %= m;
        bloom[b] = true;
    }
}
bool verify( int x){
    for( int i = 0; i < 10; i++){
        unsigned int b = (hash1(x) + i * hash2(x));
        b %= m;
        if( !bloom[b])
            return false;
    }
    return true;
}

struct node{
    node * left;
    node * right;
    int key;
    int prt;
};
node* newNode( int q){
    node * n = new node;
    n -> key = q;
    n -> prt = rand() % 100;
    n -> left = nullptr;
    n -> right = nullptr;
    return n;
}

node* right_rotate( node * a){
    node * b = a -> left;
    node *aux = b -> right;
    b -> right = a;
    a -> left = aux;
    return b;
}
node* left_rotate( node * a ){
    node* b = a -> right;
    node *aux = b -> left;
    b -> left = a;
    a -> right = aux;
    return b;
}

node* insert(node* i, int q) {
    if (!i)
        return newNode(q);
    if (i->key < q) {
        i->right = insert(i->right, q);
        if ( i->right->prt > i->prt)
            i = left_rotate(i);
    } else {
        i->left = insert(i->left, q);
        if ( i->left->prt > i->prt)
            i = right_rotate(i);
    }
    return i;
}
node* discard(node* i, int q){
    if( i == nullptr)
        return i;
    if (q < i->key)
        i->left = discard(i->left, q);
    else if (q > i->key)
        i->right = discard(i->right, q);
    else if( i-> left == nullptr){
        node* aux = i->right;
        delete(i);
        i = aux;
    } else if ( i-> right == nullptr){
        node* aux = i->left;
        delete(i);
        i = aux;
    } else if( i -> left -> prt < i -> right -> prt){
        i = left_rotate(i);
        i -> left = discard(i->left, q);
    } else{
        i = right_rotate(i);
        i -> right = discard(i->right, q);
    }
    return i;
}
node * search ( node * a, int q){
    if( a == nullptr)
        return a;
    if( a -> key < q)
        return search( a-> right, q);
    else if ( a-> key > q)
        return search( a-> left, q);
    return a;
}
void in_order(node* i) {
    if( i) {
        in_order(i->left);
        out << i->key << " ";
        in_order(i->right);
    }
}

node * del ( node * tre, int &q){
    int c = 0;
    while( search( tre, q)){
        tre = discard( tre, q);
        c++;
    }
    out << c;
    return tre;
}
int predecesor( node * a, int &q){
    if( a == nullptr)
        return INT_MAX;
    int st = INT_MAX, dr = INT_MAX;
    if( a-> key <= q ){
        dr = predecesor( a-> right, q);
        if( dr <= q )
            return dr;
        return a-> key;
    }
    st = predecesor( a-> left, q);
    if( st <= q)
        return st;
    return INT_MAX;
}
int succesor( node * a, int &q){
    if( a == nullptr)
        return INT_MIN;
    int st = INT_MIN, dr = INT_MIN;
    if( a -> key >= q){
        st = succesor( a-> left, q);
        if( st >= q)
            return st;
        return a-> key;
    }
    dr = succesor( a-> right, q);
    if( dr >= q)
        return dr;
    return INT_MIN;
}
int find( node * tre, int &x ){
    if( search(tre, x) != nullptr)
        return 1;
    return 0;
}
void afisare_interval( node * tre, int &a, int &b){
    if(tre != nullptr){
        afisare_interval( tre-> left, a, b);
        if( tre-> key >= a && tre-> key <= b)
            out << tre-> key << " ";
        afisare_interval( tre-> right, a, b);
    }
}

int main() {
    node* tre = nullptr;
    int n;
    in >> n;
    while( n != 0){
        int i;
        in >> i;
        if( i == 1){
            int v;
            in >> v;
            tre = insert(tre, v);
            add(v);
        }
        if( i == 2){
            int v;
            in >> v;
            if ( verify(v)) {
                if (search(tre, v) != nullptr) {
                    out << v << " -> ";
                    tre = del(tre, v);
                    out << "\n";
                } else
                out << v << " -> 0\n";
            } else out << v << " -> 0\n";
        }
        if( i == 3){
            int v;
            in >> v;
            out << v << " : ";
            if( !verify(v) )
                out << "0\n";
            else out << find( tre, v) << "\n";
        }
        if( i == 4){
            int v;
            in >> v;
            int pred = predecesor(tre, v);
            if( pred == INT_MAX)
                out << "-1\n";
            else
                out << pred << "\n";
        }
        if( i == 5 ){
            int v;
            in >> v;
            int suc = succesor(tre, v);
            if( suc == INT_MIN)
                out << "-1\n";
            else
                out << suc << "\n";
        }
        if( i == 6){
            int a, b;
            in >> a >> b;
            afisare_interval(tre, a, b);
            out <<"\n";
        }
        n--;
    }
    while( tre ){
        tre = discard( tre, tre->key);
    }
    return 0;
}

