/* Created by Akshat Bhardwaj UFID - 43527794  */


#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <queue>
#include <fstream>
#include <istream>
#include <string>

using namespace std;

/* Node Declaration */

struct fNode{
    string key;	//To store the key as string
    int value;	//To store the hashtag value
    int fDegree;	//To store the degree of the heap
    fNode* fParent;	//Node pointing to the parent
    fNode* fChild;	//Node pointing to the child
    fNode* fLeft;	//Node pointing to the left sibling
    fNode* fRight;	//Node pointing to the right sibling
    bool cut;	//To store the information if it has lost a child after becoming the child of its current parent
};


/* Class Declaration */

class fHeap {
private:
    int nH;
    fNode* fH;//Pointer to the head node
public:
    fNode* initializeFHeap();	  //Function to initialize the heap
    fNode* createFNode(int);	  //Function to create a node with the frequency value
    void insertFNode(fNode *,string, int, bool);  //Function to insert a node in the initialized heap
    fNode* removeMax(fNode *);	  //Function to remove the Maximum element from the heap
    fNode* increaseKey(fNode *, int);	  //Function to increment the frequency of a key
    int nodeCut(fNode *, fNode *, fNode *);	  //Function to remove a node from its parent
    int cascadeNodeCut(fNode *, fNode *);  //Function to remove a node from its parent till root unless a node with cut value 'false' is found
    int Consolidate();	  //Function to consolidate the trees so that no two roots have the same rank
    int fConnect(fNode* , fNode* );      //Function to perform linking operation so that smaller root is the child of larger root
    int Display(fNode *);
    fHeap()
    {
        fH = initializeFHeap();                  //Constructor
    }
};

fHeap fh;	 //Instance of the class
fNode * H = fh.initializeFHeap();//Pointer to the initialized heap

/* Heap Initialization */

fNode* fHeap::initializeFHeap()
{
    fNode* p;
    p = NULL;
    return p;
}

/* Node Creation */

fNode* fHeap::createFNode(int v)
{
    fNode* n = new fNode;
    n->value = v;
    return n;
}

/* Node Insertion */

void fHeap::insertFNode(fNode* n,string key,int value,bool flag)
{
    if(flag){
        n->key=key;
        n->value=value;
        n->fDegree = 0;
        n->fParent = NULL;
        n->fChild = NULL;
        n->fLeft = n;
        n->fRight = n;
        n->cut = false;
        if(H != NULL)
        {
            (H->fLeft)->fRight = n;
            n->fRight = H;
            n->fLeft = H->fLeft;
            H->fLeft = n;
            if(n->value > H->value){
                H = n;
            }
        }
        else{
            H = n;
        }
        nH = nH + 1;	//Degree of the parent node is increased
        
    }
    else {
        if(H){
            (H->fLeft)->fRight=n;
            n->fRight=H;
            n->fLeft=H->fLeft;
            H->fLeft=n;
            if(n->value > H->value)
                H=n;
        }
        else {
            H=n;
        }
        
    }
}

/* Incrementing the value */

fNode* fHeap::increaseKey(fNode* h, int l)
{
    if(!h)
    {
        return h;
    }
    h->value += l;	//Frequency of the hash tag is increased
    fNode* p = h->fParent;
    if(p != NULL && h->value > p->value)
    {
        nodeCut(H, h, p);
        cascadeNodeCut(H, p);
    }
    if(h->value > H->value)
    {
        H = h;
    }
    return H;
}

/*Remove max element from the Fibonacci Heap */

fNode* fHeap::removeMax(fNode* x)
{
    fNode ** cList;
    fNode *p, *q, *next;
    p = H;
    if ( p != NULL )
    {
        q = p->fChild;
        if ( q != nullptr )
        {
            cList = new fNode*[p->fDegree];
            next = q;
            for ( int j = 0; j < (int)p->fDegree; j++ )
            {
                cList[j] = next;
                next = next->fRight;
            }
            for ( int j = 0; j < (int)p->fDegree; j++ )
            {
                q = cList[j];
                H->fLeft->fRight = q;
                q->fLeft = H->fLeft;
                H->fLeft = q;
                q->fRight = H;
                q->fParent = nullptr;
            }
          //  delete [] cList;
        }
        p->fChild=nullptr;
        p->fDegree=0;
        p->fLeft->fRight = p->fRight;
        p->fRight->fLeft = p->fLeft;
        if ( p == p->fRight )
        {
            H = NULL;
        }
        else
        {
            H = p->fRight;
            Consolidate();
        }
        nH--;
    }
    return p;
}

/* Consolidation of the tree */

int fHeap::Consolidate()
{
    fNode* w, * next, * x, * y, * temp;
    fNode** Array, ** rList;
    int d, rSize;
    int max_degree =  floor(log((double)(nH))/log((double)(1 + sqrt((double)(5)))/2));//Max degree is less than or equal to log base golden ratio of n
    Array = new fNode*[max_degree+2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
    fill_n(Array, max_degree+2, nullptr);//used to assign null value to the elements of A
    w = H;
    rSize = 0; //Size of the root list is initially zero
    next = w;
    do
    {
        rSize++;
        next = next->fRight;
    } while ( next != w );
    rList = new fNode*[rSize];
    for ( int i = 0; i < rSize; i++ )
    {
        rList[i] = next;
        next = next->fRight;
    }
    for ( int i = 0; i < rSize; i++ )
    {
        w = rList[i];
        x = w;
        d = x->fDegree;
        while ( Array[d] != nullptr )
        {
            y = Array[d];
            if ( x->value < y->value )
            {
                temp = x;
                x = y;
                y = temp;
            }
            fConnect(y,x);
            Array[d] = nullptr;
            d++;
        }
        Array[d] = x;
    }
    delete [] rList;
    H = nullptr;
    for ( int i = 0; i < max_degree+2; i++ )
    {
        if ( Array[i] != nullptr )
        {
            if ( H == nullptr )
            {
                H = Array[i]->fLeft = Array[i]->fRight = Array[i];
            }
            else
            {
                H->fLeft->fRight = Array[i];
                Array[i]->fLeft = H->fLeft;
                H->fLeft = Array[i];
                Array[i]->fRight = H;
                if ( Array[i]->value > H->value )
                {
                    H = Array[i];
                }
            }
        }
    }
    delete [] Array;
    return 0;
}


/* Connecting nodes in the Heap */

int fHeap::fConnect(fNode* y, fNode* x)
{
    
    y->fLeft->fRight = y->fRight;
    y->fRight->fLeft = y->fLeft;
    if ( x->fChild != nullptr )
    {
        x->fChild->fLeft->fRight = y;
        y->fLeft = x->fChild->fLeft;
        x->fChild->fLeft = y;
        y->fRight = x->fChild;
    }
    else
    {
        x->fChild = y;
        y->fRight = y;
        y->fLeft = y;
    }
    y->fParent = x;
    x->fDegree++;
    y->cut = false;
    return 0;
}

/* Node Cut in the Fibonacci Heap */

int fHeap::nodeCut(fNode* z, fNode* x, fNode* y)
{
    if(x == x->fRight)
    {
        y->fChild = NULL;
    }
    (x->fLeft)->fRight = x->fRight;
    (x->fRight)->fLeft = x->fLeft;
    if(x == y->fChild)
    {
        y->fChild = x->fRight;
    }
    y->fDegree = y->fDegree - 1;
    x->fRight = x;
    x->fLeft = x;
    (H->fLeft)->fRight = x;
    x->fRight = H;
    x->fLeft = H->fLeft;
    H->fLeft = x;
    x->fParent = NULL;
    x->cut = false;
    return 0;
}


/* Cascading Cut in the Fibonacci Heap */

int fHeap::cascadeNodeCut(fNode* t, fNode* y)
{
    fNode* z = y->fParent;
    if(z != NULL)
    {
        if(y->cut == false)
        {
            y->cut = true;
        }
        else
        {
            nodeCut(H, y, z);
            cascadeNodeCut(H, z);
        }
    }
    return 0;
}



int main(int argc, char *argv[]){
    
    int value;
    string key;
    map<string, fNode*> hm;//Hash Map to store the input string as the key and node pointer as the value.
    fNode* p;
    fNode* h;
    ifstream inFile(argv[1]);
    ofstream outFile("output_file.txt");
    while(inFile >> key)
    {
        if(key[0]=='#')
        {
            inFile >> value;
            if(hm.find(key) != hm.end())
            {
                h = hm[key];
                fNode* H1 = fh.increaseKey(h, value);//If the key is found, increment the value
            }
            else
            {
                p = fh.createFNode(value);
                fh.insertFNode(p,key,value,true); //If the key is not found, insert the node
                hm[key]=p;
            }
        }
        else if(key=="STOP"|| key=="stop")//If the key asks to stop the input values
        {
            inFile.close();
            outFile.close();
            return 0;
        }
        else
        {
            
            queue<fNode * > Q;
            value=stoi(key);
            if(value>0)
            {
                fNode* y = fh.removeMax(H);
                Q.push(y);                  //Max removed values are pushed on to the queue
                string result=y->key;
                result.erase(0,1);
                outFile << result << "";  //outFile << result << "" << y->value << "";
                while(--value)
                {
                    fNode * test = H;
                    fNode* x = fh.removeMax(H);
                    Q.push(x);
                    string result=x->key;
                    result.erase(0,1);
                    outFile <<","<<result<<""; //outFile << ","<<result<<""<<x->value<<"";
                }
                while(!Q.empty())
                {
                    fNode * temp = Q.front();
                    Q.pop();
                    fh.insertFNode(temp,key,value,false);
                }
                outFile << endl;
                
            }
        }
    }
    return 0;
}
