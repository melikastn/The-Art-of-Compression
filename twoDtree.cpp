
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include <math.h>
#include <stdio.h>

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}



twoDtree::twoDtree(PNG & imIn){ 
   height = imIn.height();// height of PNG represented by the tree
   width = imIn.width(); // width of
   stats stat = stats(imIn);
   root = buildTree(stat,make_pair(0, 0), make_pair(width-1, height-1),true); // ptr to the root of the twoDtree

// YOUR CODE HERE

}


twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {

// YOUR CODE HERE!!
Node * root = new Node(ul,lr,s.getAvg(ul,lr));
if (ul.first == lr.first && ul.second == lr.second){ //leaf
  return root;
}
int cut;
pair<int,int> lrLeftChild;
pair<int,int> ulRightChild;

if (vert){
 cut = minimizeSumVar(s,ul,lr,vert);
 lrLeftChild = make_pair(cut,lr.second);
 ulRightChild =  make_pair(cut+1 , ul.second);

}else {
  cut = minimizeSumVar(s,ul,lr,vert);
  lrLeftChild = make_pair(lr.first,cut);
  ulRightChild =  make_pair(ul.first, cut+1);
}



if (ul.first == lrLeftChild.first ){
  root->left = buildTree(s,ul,lrLeftChild,false);
}else if (ul.second == lrLeftChild.second){
    root->left = buildTree(s,ul,lrLeftChild,true);
}else{root->left = buildTree(s,ul,lrLeftChild,!vert);}

if (ulRightChild.first == lr.first){
 root->right = buildTree(s,ulRightChild,lr,false); 
}else if (ulRightChild.second == lr.second){
  root->right = buildTree(s,ulRightChild,lr,true);
}else {root->right = buildTree(s,ulRightChild,lr,!vert);}


return root;

}

int twoDtree::minimizeSumVar(stats & s,pair<int,int> ul, pair<int,int> lr, bool vert){
  int sum;
  int dim = 0;
  if (vert){
    sum = s.getScore(ul,make_pair(ul.first,lr.second)) + s.getScore(make_pair(ul.first + 1, ul.second) , lr);
  for (int i = ul.first; i < lr.first ; i++ ){
    int temp = s.getScore(ul,make_pair(i, lr.second)) + s.getScore(make_pair(i + 1, ul.second),lr);
    if (temp <= sum){
      sum = temp;
      dim = i;
    }
  }
  return dim;

  } else {
    sum = s.getScore(ul,make_pair(lr.first,ul.second)) + s.getScore(make_pair(ul.first, ul.second+1) , lr);
  for (int i = ul.second; i < lr.second ; i++ ){
    int temp = s.getScore(ul,make_pair(lr.first, i)) + s.getScore(make_pair(ul.first, i+1),lr);
    if (temp <= sum){
      sum = temp;
      dim = i;
    }
  }
  return dim;
  }
}


   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. It may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
    */
PNG twoDtree::render(){
// int w = root->lowRight.first;
// int h = root->lowRight.second;
PNG img = PNG(width,height);
renderHelper( img, root);
return img;

// YOUR CODE HERE!!

}

void twoDtree::renderHelper(PNG & img, Node * root){
  if (root == NULL){return;}
  if(!root->left && !root->right){
    for (int i = root->upLeft.first ; i <= root->lowRight.first ; i++){
      for (int j = root->upLeft.second ; j <= root->lowRight.second ; j++){
        *(img.getPixel(i,j)) = root->avg;
      }
    }
  }
  renderHelper(img, root->left);
  renderHelper(img, root->right);

  //  if (root->left != NULL){
  //   renderHelper(img, root->left);
  // }
  // if (root->right != NULL){

  // }
}


 /*
    * The idealPrune function can be considered to be the inverse
    * of the pruneSize function. It takes as input a number of leaves
    * and returns the minimum tolerance that would produce that resolution
    * upon a prune. It does not change the structure of the tree.
    * 
    */
int twoDtree::idealPrune(int leaves){
  int zero = pruneSize(0);
  if (zero == leaves){
    estTol = 0;
  }else{
  int upperBoundTol = 1;
 while (pruneSize(upperBoundTol) > leaves){
    upperBoundTol = upperBoundTol *2;
    printf("in while\n");

  }
// getEstimateTol(leaves,upperBoundTol/2, upperBoundTol);
  }
//   if ( estleaves == leaves){
//     return estTol;
//   } else if (estleaves >= leaves ){
//     while (pruneSize(estTol) != leaves) { // estTol = estimated tolerance
// 	estTol--;}
// }else if (estleaves <= leaves ){
//   while (pruneSize(estTol) != leaves) { // estTol = estimated tolerance
// 	estTol++;}
// }
return estTol;
  

// YOUR CODE HERE!!

}

void twoDtree:: getEstimateTol(int leaves, int estTolL, int estTolU){


  int lowerboundleaf = pruneSize(estTolU);
  int upperboundleafleaf = pruneSize(estTolL);
  int midboundleaf = pruneSize((estTolL+estTolU)/2);

   if (midboundleaf == leaves){
    printf("here 1\n");
    estTol = (estTolL+estTolU)/2;
    return;
  }else if(lowerboundleaf == leaves){
    printf("here 2 \n");
    estTol = estTolU;
    return;

  }else if (upperboundleafleaf == leaves){
    printf("here 3\n");
    estTol = estTolL;
    return;

  }else if (leaves < midboundleaf ){
    printf("here 4 \n");
        getEstimateTol(leaves,(estTolL+estTolU)/2, estTolU );
        return;
  }
  else if(leaves > midboundleaf){
    printf("here 5 \n");
        getEstimateTol(leaves,estTolL, (estTolL+estTolU)/2 );
        return;
  }else{
    printf("fail\n");}
}

int twoDtree::pruneSize(int tol){
    // int * counter=0;
       psLeafs = 0;
    pruneSizeHelper(tol, root,root);
    // int result = *counter;
    // delete counter;
    //printf("returning leafs %d\n", psLeafs);
    return psLeafs;
// YOUR CODE HERE!!

}

void twoDtree::pruneSizeHelper(int tol, Node * root, Node * subtree){
  if (subtree == NULL){
    return ;
  //  printf("subtree is null\n");
  }

  if (prunable(root,subtree,tol)){
    psLeafs++;
   // printf("increasing leafs t0 %d\n",psLeafs);
  } else{
    pruneSizeHelper(tol,root->left,root->left);
   // printf("to the left");
    pruneSizeHelper(tol,root->right,root->right);
    //printf("to the right");
  }
}

int twoDtree::distance( Node* root , Node * curr){
  int r = curr->avg.r - root->avg.r;
  int b = curr->avg.b - root->avg.b;
  int g = curr->avg.g - root->avg.g;
 int  distance = pow(r,2) + pow(b,2) + pow(g,2);
 return distance;

}

bool twoDtree::prunable(Node * root, Node * subtree, int tol){
  if (subtree == NULL){
    return true;
  }
  if (subtree->left == NULL && subtree->right== NULL){
     if (distance(root,subtree) <= tol){
       return true;
     }else {
       return false;
       }
  }
  return prunable(root, subtree->left,tol) && prunable(root, subtree->right,tol);

}



void twoDtree::prune(int tol){

prune(root,root,tol);

}

void twoDtree::prune(Node * root, Node * subtree, int tol){
 if (root == NULL){
    return ;
  }
  if (prunable(root,root,tol)){
    //delete the subtrees before setting to null
    clear(root->right);
    clear(root->left);
    root->right = NULL;
    root->left = NULL;
  }else{
    prune(root->left,root->left,tol);
    prune(root->right,root->right,tol);
  }
}

/**
    * Destroys all dynamically allocated memory associated with the
    * current twoDtree class. Complete for PA3.
   * You may want a recursive helper function for this one.
    */
void twoDtree::clear() {

// YOUR CODE HERE!!
clear(root);
}

void twoDtree::clear(Node * root){
  if (root == NULL){return;}
  clear(root->left);
  clear(root->right);
  if (root->left == NULL && root->right == NULL){
    // delete root->upLeft;
    // delete root->lowRight;
    // delete root->avg;
    delete root->right;
    delete root->left;
  }

}
 /**
   * Copies the parameter other twoDtree into the current twoDtree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The twoDtree to be copied.
   */
void twoDtree::copy(const twoDtree & orig){
  width = orig.width;
  height = orig.height;
  root = copy( orig.root);
// YOUR CODE HERE!!

}

twoDtree::Node * twoDtree::copy(Node * orig){
  if (orig == NULL){return NULL;}
  Node* root = new Node(orig->upLeft,orig->lowRight,orig->avg);
// root->upLeft = make_pair(orig->upLeft.first, orig->upLeft.second);
// root->lowRight = make_pair(orig->lowRight.first, orig->lowRight.second);
// root->avg = RGBAPixel(orig->avg.r,orig->avg.g,orig->avg.b,orig->avg.a);
root->left = copy( orig->left);
root->right = copy( orig->right);
return root;
      
}


