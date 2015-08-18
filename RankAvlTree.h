
//  Assuming K implement operators: =, copy c'tor, ==, >,<
//  Assuming T implement operators: =, copy c'tor

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <algorithm>
#include <assert.h>
#include <math.h>

class AvlTreeKeyAlreadyExist:std::exception{};
class AvlTreeKeyDoesNotExist:std::exception{};
class AvlTreeIsEmpty :std::exception{};
class AvlTreeInvalidInput : std::exception{};

template <typename K,typename T>
class AvlTree{
    class Node{
        K key;
        T data;
        
        int HL;
        int HR;
		int w;
        
        Node* father;
        Node* leftSon;
        Node* rightSon;
    public:
        Node(){}
		Node(const K& key, const T& data) :key(key), data(data), HL(0), HR(0), w(1),
                              father(NULL),leftSon(NULL),rightSon(NULL){}
		Node(const int h, const int r, const int w) : HL(h), HR(r), w(w), father(NULL), leftSon(NULL), rightSon(NULL){}
        Node(const Node& node):key(node.key),data(node.data),HL(node.HL),HR(node.HR),w(node.w){}
        
        void setFather(Node* node){ father=node; }
        void setLeftSon(Node* node){ leftSon = node; }
        void setRightSon(Node* node){ rightSon = node; }
        Node* getFather(){ return father; }
        Node* getLeftSon(){ return leftSon; }
        Node* getRightSon(){ return rightSon; }
        
        void setHL(int num) { HL=num; }
        void setHR(int num) { HR=num; }
		void setW(int num) { w = num; }
        int getHL() { return HL; }
        int getHR() { return HR; }
		int getW() { return w; }
        
        void incHL(){ HL+=1; }
        void decHL(){ HL-=1; }
        void incHR(){ HR+=1; }
        void decHR(){ HR-=1; }
		void incW() { w += 1; }
		void decW() { w -= 1; }

        int BF(){
            return HL-HR;
        }
		

        void setKey(K& k) { key=k; }
        void setData(T& d) { data=d; }
        K& getKey() { return key; }
        T& getData() { return data; }
    };
    
    Node* Root;
    int size;
    int recursiveIterator;
    
    bool isKeyExist(Node* root, const K& key){
        if (!root){
            return false;
        }
        if (root->getKey()==key){
            return true;
        }
        if (root->getKey()>key){
            return isKeyExist(root->getLeftSon(), key);
        } else {
            return isKeyExist(root->getRightSon(), key);
        }
    }
    
    Node* findByKey(Node* root, const K& key){
        if (root->getKey()==key){
            return root;
        }
        if (root->getKey()>key){
            return findByKey(root->getLeftSon(), key);
        } else {
            return findByKey(root->getRightSon(), key);
        }
    }
    
    void findNextBestByKey(Node* root, const K& key,Node** nextBest){
        if (!root){
            return;
        }
        if (root->getKey()>key){
            findNextBestByKey(root->getLeftSon(), key,nextBest);
            if ((root->getKey()<key) && (root->getKey()> (*nextBest)->getKey())){
                *nextBest = root;
            }
        } else {
            findNextBestByKey(root->getRightSon(), key,nextBest);
            if ((root->getKey() < key) && (root->getKey() > (*nextBest)->getKey())){
                *nextBest = root;
            }
        }
    }
    
    Node* findMax(Node* root){
		if (root == NULL){
			return NULL;
		}
        if (!root->getRightSon()){
            return root;
        }
        return findMax(root->getRightSon());
    }

    Node* findMin(Node* root){
		if (root == NULL){
			return NULL;
		}
        if (!root->getLeftSon()){
            return root;
        }
        return findMin(root->getLeftSon());
    }
    
    void rollR(Node* B){////ROLL R
        Node* A = B->getLeftSon();
        
        if (B==Root){
            Root=A;                             //Case B is the ROOT

        }
        
        B->setHL(A->getHR());
        A->setHR(B->getHR()>A->getHR() ? B->getHR() : A->getHR());
        A->incHR();                             //Fix Heigts

		int totW = B->getW();					//Fix w
		int bLeftW = (A->getRightSon()) ? (A->getRightSon())->getW() : 0;
		B->setW(1 + bLeftW + ((B->getRightSon()) ? B->getRightSon()->getW() : 0));
		A->setW(totW);
        
        B->setLeftSon(A->getRightSon());
        if (A->getRightSon()){                  //Swap Ar -> Bl
            (A->getRightSon())->setFather(B);
        }
        
        A->setRightSon(B);
        A->setFather(B->getFather());           //Swap B -> Ar
        B->setFather(A);
        
        if (!A->getFather()){
            return;
        }                                       //Fix Father's Pointer
        if  ((A->getFather())->getLeftSon() == B){
            (A->getFather())->setLeftSon(A);
        } else {
            (A->getFather())->setRightSon(A);
        }
    }
    
    void rollL(Node* A){////ROLL L
        Node* B = A->getRightSon();
        
        if (A==Root){
            Root=B;                             //Case A is the ROOT
        }
        
        A->setHR(B->getHL());
        B->setHL(B->getHL()>A->getHL() ? B->getHL() : A->getHL());
        B->incHL();                             //Fix Heigts

		int totW = A->getW();					//Fix w
		int aRightW = (B->getLeftSon()) ? (B->getLeftSon())->getW() : 0;
		A->setW(1 + aRightW + ((A->getLeftSon()) ? A->getLeftSon()->getW() : 0));
		B->setW(totW);

        A->setRightSon(B->getLeftSon());
        if (B->getLeftSon()){                   //Swap Bl -> Ar
            (B->getLeftSon())->setFather(A);
        }
        
        B->setLeftSon(A);
        B->setFather(A->getFather());           //Swap A -> Bl
        A->setFather(B);
        
        if (!B->getFather()){
            return;
        }                                       //Fix Father's Pointer
        if  ((B->getFather())->getLeftSon() == A){
            (B->getFather())->setLeftSon(B);
        } else {
            (B->getFather())->setRightSon(B);
        }
    }

    void checkAndRoll(Node* node){
        if (node->BF() == 2){
            if ((node->getLeftSon())->BF() == -1){
                rollL(node->getLeftSon());
                rollR(node);
            } else {
                rollR(node);
            }
        }
        if (node->BF() == -2){
            if ((node->getRightSon())->BF() == 1){
                rollR(node->getRightSon());
                rollL(node);
            } else {
                rollL(node);
            }
        }
    }
    
    void insertNode(Node* root, const K& key,const T& data){
        if (root->getKey()>key){
            if (!root->getLeftSon()){
                root->setLeftSon(new Node(key,data));
                (root->getLeftSon())->setFather(root);
                
                root->incHL();
//				root->incW();
                
            } else {
                insertNode(root->getLeftSon(),key,data);
                root->setHL(root->getLeftSon()->getHL() > root->getLeftSon()-> getHR()
                            ? root->getLeftSon()->getHL() : root->getLeftSon()->getHR());
                root->incHL();
//				root->incW();
                checkAndRoll(root);
            }
        } else {
            if (!root->getRightSon()){
                root->setRightSon(new Node(key,data));
                (root->getRightSon())->setFather(root);
                
                root->incHR();
//				root->incW();
                
            } else {
                insertNode(root->getRightSon(),key,data);
                root->setHR(root->getRightSon()->getHL() > root->getRightSon()-> getHR()
                            ? root->getRightSon()->getHL() : root->getRightSon()->getHR());
                root->incHR();
//				root->incW();
                checkAndRoll(root);
            }
        }
    }
    
    void removeByCase(Node* root){
        if ((!root->getLeftSon()) && (!root->getRightSon())){//No Sons
            if (root == Root){
                Root = NULL;
                delete root;
                return;
            }
            
            if ((root->getFather())->getRightSon() == root){
                root->getFather()->setRightSon(NULL);
            } else {
                root->getFather()->setLeftSon(NULL);
            }
            delete root;
            return;
        }//
        
        if ((!root->getLeftSon()) && (root->getRightSon())){//One Right Son
            if (root == Root){
                root->getRightSon()->setFather(NULL);
                Root=root->getRightSon();
                delete root;
                return;
            }
            
            if ((root->getFather())->getRightSon() == root){
                root->getFather()->setRightSon(root->getRightSon());
            } else {
                root->getFather()->setLeftSon(root->getRightSon());
            }
            root->getRightSon()->setFather(root->getFather());
            delete root;
            return;
        }//
        
        if ((root->getLeftSon()) && (!root->getRightSon())){//One Left Son
            if (root == Root){
                root->getLeftSon()->setFather(NULL);
                Root=root->getLeftSon();
                delete root;
                return;
            }
            
            if ((root->getFather())->getRightSon() == root){
                root->getFather()->setRightSon(root->getLeftSon());
            } else {
                root->getFather()->setLeftSon(root->getLeftSon());
            }
            root->getLeftSon()->setFather(root->getFather());
            delete root;
            return;
        }//
    }
    
    void removeNode(Node* root,const K& key){
        if (root->getKey()==key){
			
            if ((root->getRightSon()) && (root->getLeftSon())){//Two Sons
//				root->decW();
                Node* followingNode = root->getRightSon();
//				followingNode->decW();
                while (followingNode->getLeftSon()){
                    followingNode = followingNode->getLeftSon();
//					followingNode->decW();
                }
                Node* followingNodeCopy = new Node(*followingNode);
                
                removeNode(Root, followingNodeCopy->getKey());
                root->setKey(followingNodeCopy->getKey());
                root->setData(followingNodeCopy->getData());
                delete followingNodeCopy;
                
                return;
            }//
            
            removeByCase(root);
            return;
            
        }
        
        if (root->getKey()>key){
			root->decW();
            removeNode(root->getLeftSon(),key);
            if (!root->getLeftSon()){
                root->setHL(0);
                checkAndRoll(root);
                return;
            }
            root->setHL(root->getLeftSon()->getHL() > root->getLeftSon()-> getHR()
                        ? root->getLeftSon()->getHL() : root->getLeftSon()->getHR());
            root->incHL();
            checkAndRoll(root);
        } else {
			root->decW();
            removeNode(root->getRightSon(),key);
            if (!root->getRightSon()){
                root->setHR(0);
                checkAndRoll(root);
                return;
            }
            root->setHR(root->getRightSon()->getHL() > root->getRightSon()-> getHR()
                        ? root->getRightSon()->getHL() : root->getRightSon()->getHR());
            root->incHR();
            checkAndRoll(root);
        }
    }
    
    void postOrderDeletion(Node* root){////POSTORDER DELETE
        if (!root){
            return;
        }
        postOrderDeletion(root->getLeftSon());
        postOrderDeletion(root->getRightSon());
        delete root;
    }
    
    void inorderDumpTreeToArray(Node* root,Node** inorderArray){
        if (!root){
            return;
        }
        inorderDumpTreeToArray(root->getLeftSon(),inorderArray);
        inorderArray[recursiveIterator++] = new Node(*root);
        inorderDumpTreeToArray(root->getRightSon(),inorderArray);
    }
    
    void inorderDumpArrayToTree(Node* root,Node** treeArray){
        if (!root){
            return;
        }
        inorderDumpArrayToTree(root->getLeftSon(),treeArray);
        root->setKey(treeArray[recursiveIterator]->getKey());
        root->setData(treeArray[recursiveIterator]->getData());
        delete treeArray[recursiveIterator++];
        inorderDumpArrayToTree(root->getRightSon(),treeArray);
    }
    
	void inorderDumpTreeToDataArrayHelper(Node* root, T** inorderArray){		
		if (!root){
			return;
		}
		inorderDumpTreeToDataArrayHelper(root->getLeftSon(), inorderArray);
		inorderArray[recursiveIterator++] = new T(root->getData());
		inorderDumpTreeToDataArrayHelper(root->getRightSon(), inorderArray);
	}

	void inorderDumpTreeToDataArrayHelper2(Node* root, int* inorderArray){
		if (!root){
			return;
		}
		inorderDumpTreeToDataArrayHelper2(root->getLeftSon(), inorderArray);
		inorderArray[recursiveIterator++] = (int) root->getData();
		inorderDumpTreeToDataArrayHelper2(root->getRightSon(), inorderArray);
	}

    template<typename C,typename G>
    void inorderUpdateTreeData(Node* root,C cond,G updateDataFunc){
        if (!root){
            return;
        }
        inorderUpdateTreeData(root->getLeftSon(),cond,updateDataFunc);
        if (cond(root->getData())){
            updateDataFunc(root->getData());
        }
        inorderUpdateTreeData(root->getRightSon(),cond,updateDataFunc);
    }

	template<typename C, typename G>
	void inorderUpdateTreeDataByKey(Node* root, C cond, G updateDataFunc){
		if (!root){
			return;
		}
		inorderUpdateTreeDataByKey(root->getLeftSon(), cond, updateDataFunc);
		if (cond(root->getKey())){
			updateDataFunc(root->getData());
		}
		inorderUpdateTreeDataByKey(root->getRightSon(), cond, updateDataFunc);
	}

	Node* selectNode(Node* root, int k){
		if (((root->getLeftSon() != NULL) ? root->getLeftSon()->getW() : 0) == k - 1){
			return root;
		}
		else if (((root->getLeftSon() != NULL) ? root->getLeftSon()->getW() : 0) > k - 1){
			return selectNode(root->getLeftSon(), k);
		}
		else {
			return selectNode(root->getRightSon(), (k - ((root->getLeftSon() != NULL) ? root->getLeftSon()->getW() : 0) - 1));
		}
	}

	Node* createEmptyAlmostCompleteTree(Node* root, int h){
		if (h <= 0){
			return root;
		}
		if (root == NULL){
			root = new Node(h, h, (int)(pow((double)2, (double)h + 1.0)) - 1);
			root->setFather(NULL);
		}
		Node* node = new Node(h - 1, h - 1, (int)(pow((double)2, (double)h)) - 1);
		node->setFather(root);
		root->setLeftSon(createEmptyAlmostCompleteTree(node, h - 1));
		Node* node1 = new Node(h - 1, h - 1, (int)(pow((double)2, (double)h)) - 1);
		node1->setFather(root);
		root->setRightSon(createEmptyAlmostCompleteTree(node1, h - 1));
		return root;
	}

	Node* inorderOppositeDeleteLeaves(Node* root, int& numToRemove){
		if (root == NULL || numToRemove == 0){//or numToRemove == 0 ?
			return root;
		}
		if (root->getRightSon() != NULL){
			root->setRightSon(inorderOppositeDeleteLeaves(root->getRightSon(), numToRemove));
			inorderOppositeFixTree(root);
		}
		if (root->getRightSon() == NULL && root->getLeftSon() == NULL){
			if (numToRemove > 0){
				delete root;
				--numToRemove;
				return NULL;
			}
			return root;
		}

		root->setLeftSon(inorderOppositeDeleteLeaves(root->getLeftSon(), numToRemove));
		inorderOppositeFixTree(root);
		return root;
	}

	int isMax(int a, int b){
		return a > b ? a : b;
	}

	void inorderOppositeFixTree(Node* root){
		if (root == NULL ){
			return;
		}
		root->setHL((root->getLeftSon() == NULL) ? 0 : isMax(root->getLeftSon()->getHL(), root->getLeftSon()->getHR()) + 1);
		root->setHR((root->getRightSon() == NULL) ? 0 : isMax(root->getRightSon()->getHR(), root->getRightSon()->getHL()) + 1);
		int wLeft = (root->getLeftSon() == NULL) ? 0 : root->getLeftSon()->getW();
		int wRight = (root->getRightSon() == NULL) ? 0 : root->getRightSon()->getW();
		root->setW(wLeft + wRight + 1);
	}

	void fixWeight(Node* root, const K& key){
		if (root == NULL){
			return;
		}
		if (root->getKey() > key){
			fixWeight(root->getLeftSon(), key);
			
		}
		if (root->getKey() < key){
			fixWeight(root->getRightSon(), key);
		}
		int Wleft=0, Wright=0;
		if (root->getLeftSon() != NULL){
			Wleft = root->getLeftSon()->getW();
		}
		if (root->getRightSon() != NULL){
			Wright = root->getRightSon()->getW();
		}
		root->setW(Wleft + Wright + 1);
	}
	
    
public:
    AvlTree():Root(NULL), size(0),recursiveIterator(0){}
    ~AvlTree(){
        postOrderDeletion(Root);
        Root = NULL;
    }
    
    const unsigned int getTreeSize() const{
        return size;
    }

	const int getW(const K& key){
		return (findByKey(Root, key))->getW();
	}

	T& select(int k){
		if (k < 0 || k >= size){
			throw AvlTreeInvalidInput();
		}
		Node* tmp = selectNode(Root, k+1);
		return tmp->getData();
	}
    
    //AvlTree(const AvlTree& avl);                  //Copy c'tor
    //AvlTree& operator=(const AvlTree& avl);       //Operator =
    
    bool isKeyExist(const K& key){
        return isKeyExist(Root, key);
    }
    
    void insert(const K& key,const T& data){
        if (!Root){
            Root = new Node(key,data);
            size++;
            return;
        }
        
        if (isKeyExist(Root, key)){
            throw AvlTreeKeyAlreadyExist();
        }
        
        insertNode(Root,key,data);
		fixWeight(Root, key);
        size++;
    }
    
    void remove(const K& key){
        if (!isKeyExist(Root, key)){
            throw AvlTreeKeyDoesNotExist();
        }
        
        
        removeNode(Root, key);
		fixWeight(Root, key);
        size--;
    }
    
    T& operator[](const K& key){
        if (!isKeyExist(Root, key)){
            throw AvlTreeKeyDoesNotExist();
        }
        
        return (findByKey(Root, key))->getData();
	}
    const T& operator[](const K& key) const{
        if (!isKeyExist(Root, key)){
            throw AvlTreeKeyDoesNotExist();
        }
        
        return (findByKey(Root, key))->getData();
    }

    T& getMax(){
		if (findMax(Root)){
			return (findMax(Root))->getData();
		}
		throw AvlTreeIsEmpty();
    }
    
    T& getMin(){
		if (findMin(Root)){
			return (findMin(Root))->getData();
		}
		throw AvlTreeIsEmpty();
    }

    T& findLessOrEqual(const K& key) {
        Node* nextBest = findMin(Root);
        if (Root==NULL)
            throw AvlTreeKeyDoesNotExist();
        if (nextBest->getKey()>key)
            throw AvlTreeKeyDoesNotExist();
        
        findNextBestByKey(Root, key,&nextBest);
        return nextBest->getData();
    }

	void inorderDumpTreeToDataArray(int* inorderArray){
		recursiveIterator = 0;
		inorderDumpTreeToDataArrayHelper2(this->Root, inorderArray);
	}

    template<typename C,typename F>
    void updateKeyByDataCondition(C cond,F updateKeyFunc){
        Node** treeArray = new Node*[size];
		Node** arrayOfChanged = new Node*[size];
		Node** arrayOfUnChanged = new Node*[size];
		int changedIterator = 0;
		int unChangedIterator = 0;
        recursiveIterator=0;
        inorderDumpTreeToArray(Root, treeArray);
        
        for (int i=0 ; i<size ; i++){
            if (cond(treeArray[i]->getData())){
				updateKeyFunc(treeArray[i]->getKey());
				arrayOfChanged[changedIterator++] = treeArray[i];
            }
			else{
				arrayOfUnChanged[unChangedIterator++] = treeArray[i];
            }
        }
        
        Node** updatedTreeArray = new Node*[size];
		int iUpdated = 0;
		int iChanged = 0;
		int iUnChanged = 0;
		assert(size == changedIterator + unChangedIterator);
		while (iUpdated < size && iChanged < changedIterator && iUnChanged < unChangedIterator){					//merge
			if (arrayOfChanged[iChanged]->getKey() < arrayOfUnChanged[iUnChanged]->getKey()){
				updatedTreeArray[iUpdated++] = arrayOfChanged[iChanged++];
			}
			else {
				updatedTreeArray[iUpdated++] = arrayOfUnChanged[iUnChanged++];
			}
		}
		while (iUpdated < size && iChanged < changedIterator){
			updatedTreeArray[iUpdated++] = arrayOfChanged[iChanged++];
		}
		while (iUpdated < size && iUnChanged < unChangedIterator){
			updatedTreeArray[iUpdated++] = arrayOfUnChanged[iUnChanged++];
		}

        delete[] treeArray;
		delete[] arrayOfChanged;
		delete[] arrayOfUnChanged;
        
        recursiveIterator=0;
        inorderDumpArrayToTree(Root, updatedTreeArray);
        
        delete[] updatedTreeArray;
    }
    
    template<typename C,typename G>
    void updateDataByDataCondition(C cond,G updateDataFunc){
        inorderUpdateTreeData(Root, cond, updateDataFunc);
    }

	template<typename C, typename G>
	void updateDataByKeyCondition(C cond, G updateDataFunc){
		inorderUpdateTreeDataByKey(Root, cond, updateDataFunc);
	}

	void create(int h){
		Root = createEmptyAlmostCompleteTree(Root, h);
		size = (int)pow(2, h + 1) - 1;
		return;
	}
	
	void inorderArrayToTree(K* treeArray1, T* treeArry2, int size){
		Node** tmpArray = new Node*[size];
		for (int i = 0; i < size; i++){
			tmpArray[i] = new Node(treeArray1[i], treeArry2[i]);
		}
		inorderDumpArrayToTree(Root, tmpArray);
		delete[] tmpArray;
		return;
	}

	void inorderOppositeDelete(int num){			//num == number of leaf to remove
		if (num == 0){
			return;
		}
		size -= num;
		inorderOppositeDeleteLeaves(Root, num);
	}

};

#endif
