#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil->right = p->nil->parent = NULL;
  p->root = p->nil;
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree_recursive(rbtree *t, node_t* node){

  if(node == t->nil) return;

  delete_rbtree_recursive(t, node->left);
  delete_rbtree_recursive(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_recursive(t, t->root);
  free(t->nil);
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

  while(child->parent->color == RBTREE_RED){
    //새로 추가된 노드가 부모의 왼쪽일때
    if(child->parent == child->parent->parent->left){ 
      node_t *uncle = child->parent->parent->right;
      if(uncle->color == RBTREE_BLACK){
        if(child == child->parent->right){
          child = child->parent;
          left_rotate(t, child);
        }
        child->parent->color = RBTREE_BLACK;
        child->parent->parent->color = RBTREE_RED;
        right_rotate(t, child->parent->parent);
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
        if(child == child->parent->left){
          child = child->parent;
          right_rotate(t, child);
        }
        child->parent->color = RBTREE_BLACK;
        child->parent->parent->color = RBTREE_RED;
        left_rotate(t, child->parent->parent);
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

  return new;
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
  if(t->root == t->nil) return NULL;

  node_t *current = t->root;
  while(current->left != t->nil){
    current = current->left;
  }

  return current;
}

node_t *node_min(rbtree *t, node_t *node) {
  // TODO: implement find
  while(node->left != t->nil){
    node = node->left;
  }
  return node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if(t->root == t->nil) return NULL;

  node_t *current = t->root;
  while(current->right != t->nil){
    current = current->right;
  }

  return current;
}

void rbtree_transplant(rbtree *t, node_t *base, node_t *target){


  if(base->parent == t->nil) t->root = target;
  else if(base == base->parent->left) base->parent->left = target;
  else base->parent->right = target;
  
  target->parent = base->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t* replacedNode){

  while(replacedNode != t->root && replacedNode->color == RBTREE_BLACK){
    //노드가 부모의 왼쪽일 때
    if(replacedNode == replacedNode->parent->left)
    {
      node_t *brother = replacedNode->parent->right;
      if(brother->color == RBTREE_RED){
        brother->color = RBTREE_BLACK;
        replacedNode->parent->color = RBTREE_RED;
        left_rotate(t, replacedNode->parent);
        brother = replacedNode->parent->right;
      }
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        replacedNode = replacedNode->parent;
      }
      else {
        if(brother->right->color == RBTREE_BLACK){
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t, brother);
          brother = replacedNode->parent->right;
        }  
        brother->color = replacedNode->parent->color;
        replacedNode->parent->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t, replacedNode->parent);
        replacedNode = t->root;
      }
    }
    else{
      node_t *brother = replacedNode->parent->left;
      if(brother->color == RBTREE_RED){
        brother->color = RBTREE_BLACK;
        replacedNode->parent->color = RBTREE_RED;
        right_rotate(t, replacedNode->parent);
        brother = replacedNode->parent->left;
      }
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        replacedNode = replacedNode->parent;
      }
      else {
        if(brother->left->color == RBTREE_BLACK){
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t, brother);
          brother = replacedNode->parent->left;
        }
        brother->color = replacedNode->parent->color;
        replacedNode->parent->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t, replacedNode->parent);
        replacedNode = t->root;
      }
    }

  }
  replacedNode->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  color_t deletedColor = p->color;
  node_t *replacedNode;
  
  //오른쪽 자식만 있을 때
  if(p->left == t->nil){
    replacedNode = p->right;
    rbtree_transplant(t, p, p->right);
  }
  //왼쪽 자식만 있을 때
  else if(p->right == t->nil){
    replacedNode = p->left;  
    rbtree_transplant(t, p, p->left);
  }
  //자식이 둘 있을 때
  else{
    //후임자 찾기
    node_t *tempNode = node_min(t, p->right);
    
    //삭제되는색 업데이트
    deletedColor = tempNode->color;
    //대체노드 업데이트
    replacedNode = tempNode->right;
    //대체노드가 삭제노드의 자식일 때
    if(tempNode->parent == p) replacedNode->parent = tempNode;
    else{
      rbtree_transplant(t, tempNode, tempNode->right);
      tempNode->right = p->right;
      tempNode->right->parent = tempNode;
    }
    rbtree_transplant(t, p, tempNode);
    tempNode->left = p->left;
    tempNode->left->parent = tempNode;
    tempNode->color = p->color;
  }
  
  free(p);

  if(deletedColor == RBTREE_BLACK) rbtree_erase_fixup(t, replacedNode);

  return 0;
}

void rbtree_to_array_recursive(const rbtree *t, key_t **arr, node_t *node){  
  if(node == t->nil) return;

  rbtree_to_array_recursive(t, arr, node->left);
  **arr = node->key;
  (*arr)++;
  rbtree_to_array_recursive(t, arr, node->right);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  rbtree_to_array_recursive(t, &arr, t->root);
  return 0;
}
