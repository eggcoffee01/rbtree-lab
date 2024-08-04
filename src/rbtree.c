#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil->right = p->nil->parent = NULL;
  p->root = p->nil;
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void left_rotate(rbtree *t, node_t* parent){
  node_t *child = parent->right;

  //왼쪽 서브트리 옮기기
  parent->right = child->left;
  if (child->left != t->nil) child->left->parent = parent;
  
  //조부모 연결해주기
  child->parent = parent->parent;
  if(parent->parent == t->nil) t->root = child;
  else if (parent == parent->parent->left) parent->parent->left = child;
  else parent->parent->right = child;
  
  //부모 자식 연결해주기
  child->left = parent;
  parent->parent = child;

}

void right_rotate(rbtree *t, node_t* parent){
  node_t *child = parent->left;

  //오른쪽 서브트리 옮기기
  parent->left = child->right;
  if (child->right != t->nil) child->right->parent = parent;
  
  //조부모 연결해주기
  child->parent = parent->parent;
  if(parent->parent == t->nil) t->root = child;
  else if (parent == parent->parent->left) parent->parent->left = child;
  else parent->parent->right = child;
  
  //부모 자식 연결해주기
  child->right = parent;
  parent->parent = child;

}

void rbtree_insert_fixup(rbtree *t, node_t* child){
  
  if(child->parent->color == RBTREE_RED){
    //새로 추가된 노드가 부모의 왼쪽일때
    if(child->parent == child->parent->parent->left){ 
      node_t *uncle = child->parent->parent->right;
      if(uncle->color == RBTREE_BLACK){
        if(child == child->parent->left){
          child->parent->color = RBTREE_BLACK;
          child->parent->parent->color = RBTREE_RED;
          right_rotate(t, child->parent->parent);
        }
        else{
          left_rotate(t, child->parent);
          child->color = RBTREE_BLACK;
          child->parent->color = RBTREE_RED;
          right_rotate(t, child->parent->parent);
        }
      }
      else{
        child->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        child->parent->parent->color = RBTREE_RED;
        child = child->parent->parent;
      }
    }
    //새로 추가된 노드가 부모의 오른쪽일떄
    else{
      node_t *uncle = child->parent->parent->left;
      if(uncle->color == RBTREE_BLACK){
        if(child == child->parent->right){
          child->parent->color = RBTREE_BLACK;
          child->parent->parent->color = RBTREE_RED;
          left_rotate(t, child->parent->parent);
        }
        else{
          right_rotate(t, child->parent);
          child->color = RBTREE_BLACK;
          child->parent->color = RBTREE_RED;
          left_rotate(t, child->parent->parent);
        }
      }
      else{
        child->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        child->parent->parent->color = RBTREE_RED;
        child = child->parent->parent;
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new = malloc(sizeof(node_t));
  new->key = key;

  node_t *parent = t->nil;
  node_t *current = t->root;

  while (current != t->nil)
  {
    parent = current;
    if (new->key < current->key) current = current->left;
    else current = current->right;
  }
  new->parent = parent;
  if (parent == t->nil) t->root = new;
  else if (new->key < parent->key) parent->left = new;
  else parent->right = new;
  new->color = RBTREE_RED;
  new->left = t->nil;
  new->right = t->nil;
  
  rbtree_insert_fixup(t, new);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;
  while(current != t->nil){
    if(key == current->key){
      return current;
    }
    if(key < current->key) current = current->left;
    else current= current->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
