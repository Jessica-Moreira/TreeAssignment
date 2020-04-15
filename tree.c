#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tree.h"
#include "utils.h"


//-------------------------------------------------
tnode* tnode_create(const char* word) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  p->left = NULL;
  p->right = NULL;
  
  return p;
}

void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;
  
  return p;
}

static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

int height(struct tnode* tnode) { 
    if (tnode == NULL) {return 0;}
    else
    { 
        int lheight = height(tnode->left); 
        int rheight = height(tnode->right); 
  
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
} 

void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

bool tree_empty(tree* t) { return t->size == 0; }
size_t tree_size(tree* t) { return t->size; }

//----------------------------------------------------------------

static tnode* tree_addnode(tree* t, tnode** p, const char* word) {
  int compare;
  
  if (*p == NULL) {
    *p = tnode_create(word);
  } else if ((compare = strcmp(word, (*p)->word)) == 0) {
    (*p)->count++;
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word);
  } else {
    tree_addnode(t, &(*p)->right, word);
  }

  return *p;
}

tnode* tree_add(tree* t, const char* word) {
  tnode* p = tree_addnode(t, &(t->root), word);
  t->size++;

  return p;
}

void tree_input(tree* t){
  char wordbuf[50];
  int c, idx = 0;
  
  while((c = getchar()) != EOF){
    if(isalpha(c)) {wordbuf[idx++] = c;}
    else if (idx > 0) {
      tree_add(t, wordbuf);
      memset(wordbuf, '\0', sizeof(wordbuf));
      idx = 0;
    }
  }
}

void tree_inputfile(tree* t, FILE* fin){
  char wordbuf[50];
  int c, idx = 0;
  
  while((c = fgetc(fin)) != EOF){
    if(isalpha(c)) {wordbuf[idx++] = c;}
    else if (idx > 0) {
      tree_add(t, wordbuf);
      memset(wordbuf, '\0', sizeof(wordbuf));
      idx = 0;
    }
  }
}

//----------------------------------------------------------------

static void tree_printme(tree* t, tnode* p) {
  printf("%s", p->word);
  if (p->count > 1) { printf(" -- %d", p->count); }
  printf("\n");
}

//----------------------------------------------------------------

static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes(t, p->left);
  tree_printme(t, p);
  tree_printnodes(t, p->right);
}

static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printme(t, p);
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
}

static void tree_printnodes_postorder(tree* t, tnode* p) {
  if (p == NULL) { return; }

  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
  tree_printme(t, p);

}

static void tree_printnodes_levelorder(tree* t, tnode *p, int lvl){
  if (p == NULL) { return;}
  if(lvl == 1){
    tree_printme(t,p);
  }
  else if (lvl > 1){
    tree_printnodes_levelorder(t, p -> left, lvl-1);
    tree_printnodes_levelorder(t, p -> right, lvl-1);
  }
}

//----------------------------------------------------------------

void tree_print(tree* t) {    // INORDER-printing
  tree_printnodes(t, t->root);
  printf("\n");
}

void tree_print_preorder(tree* t) {
  tree_printnodes_preorder(t, t->root);
}

void tree_print_postorder(tree* t){
  tree_printnodes_postorder(t, t->root);
}

void tree_print_levelorder(tree* t){
  int i, h = height(t->root);
  for(i = 1; i<=h; i++){
    tree_printnodes_levelorder(t, t->root, i);
    printf("\n");
  }
}

//----------------------------------------------------------------

void tree_test() {
  tree* t = tree_create();
  tree_add(t, "now");
  tree_add(t, "is");
  tree_add(t, "the");
  tree_add(t, "time");
  tree_add(t, "for");
  tree_add(t, "all");
  tree_add(t, "good");
  tree_add(t, "men");
  tree_add(t, "and");
  tree_add(t, "women");
  tree_add(t, "to");
  tree_add(t, "come");
  tree_add(t, "to");
  tree_add(t, "the");
  tree_add(t, "aid");
  tree_add(t, "of");
  tree_add(t, "their");
  tree_add(t, "country");

  tree_print(t);
  printf("is my tree empty? %s\n", yesorno(tree_empty(t)));
  printf("size of tree: %zu\n\n", tree_size(t));
  
  tree_clear(t);
  printf("is my tree empty now? %s\n", yesorno(tree_empty(t)));
  printf("size of tree: %zu\n\n", tree_size(t));
}


int main(int argc, const char* argv[]) {
  tree_test();

  tree* t = tree_create( );

	if (argc == 2){
    FILE* fin = fopen(argv[1],"r");

    if (fin != NULL) {tree_inputfile(t, fin);} 
    
    fclose(fin);
  }  
  
  else tree_input(t); 

  printf("\n");
  printf("---------- Testing Inorder ----------\n");
  tree_print(t);
  printf("---------- Testing Preorder ----------\n");
  tree_print_preorder(t);
  printf("---------- Testing Postorder ----------\n");
  tree_print_postorder(t);
  printf("---------- Testing Levelorder ----------\n");
  tree_print_levelorder(t);

  return 0;
}
