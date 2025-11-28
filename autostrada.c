#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node{
  uint low, high, white_count;
  uint8_t is_lazy;
  struct Node* left_child;
  struct Node* right_child;
  struct Node* parrent;
} Node;

Node* initNode(Node* node, uint low, uint high, Node* left_child, Node* right_child, Node* parrent){
  node->low = low;
  node->high = high;
  node->white_count = 0;
  node->is_lazy = 0;
  node->left_child = left_child;
  node->right_child = right_child;
  node->parrent = parrent;
  return node;
}

Node* createNodes(uint low, uint high, Node* parrent){
  Node* node = (Node*)malloc(sizeof(Node));
  if(high == low) node = initNode(node, low, high, NULL, NULL, parrent);
  else node = initNode(node, low, high, createNodes(low, ((uint)(low+high)/2), node), createNodes(((uint)(low+high)/2+1), high, node), parrent);
  return node;
}

void freeNodes(Node* node){
  if(node->low != node->high){
    freeNodes(node->left_child);
    freeNodes(node->right_child);
  }
  free(node);
}

void giveCountToGrandFather(int count, Node* node);

void paintNode(uint low, uint high, Node* node, char mode){
  if(low == node->low && high == node->high){
    int change;
    if (mode == 1){
      change = (int)(high-low+1)-node->white_count;
    } else{
      change = -1*node->white_count;
    }

    node->is_lazy = 1;

    giveCountToGrandFather(change, node);
  } else{
    if(node->is_lazy==1){
      node->is_lazy = 0;
      node->left_child->is_lazy = 1;
      node->right_child->is_lazy = 1;

      if(node->white_count != 0){
        node->left_child->white_count = node->left_child->high - node->left_child->low+1;
        node->right_child->white_count = node->right_child->high - node->right_child->low+1;
      }
    }

    if(low>= node->left_child->low && high <= node->left_child->high){
      paintNode(low, high, node->left_child, mode);
    } else if(low>= node->right_child->low && high <= node->right_child->high){
      paintNode(low, high, node->right_child, mode);
    } else{
      paintNode(low, node->left_child->high, node->left_child, mode);
      paintNode(node->right_child->low, high, node->right_child, mode);
    }
  }
}

void printIndent(char c, uint times){
  for(int _ = 0; _ < times; _++){
      printf("%c", c);
    }
}

void printNode(Node* node , uint depth){
  uint times = depth*4;
  printIndent(' ', times);
  printf("|%d %d|\n", node->low, node->high);
  printIndent(' ', times);
  printf("|%d %d|\n", node->is_lazy, node->white_count);
  if(node->left_child != NULL){
    printNode(node->left_child, depth+1);
    printf("\n");
  }
  if(node->right_child != NULL){
    printNode(node->right_child, depth+1);
  }
}

void giveCountToGrandFather(int change, Node* node){
  node->white_count += change;
  if(node->parrent != NULL)giveCountToGrandFather(change, node->parrent);
}

int main(){
  uint n;
  scanf("%d",&n);
  Node* node = createNodes(1, n, NULL);
  
  uint m;
  scanf("%d", &m);

  uint arr[m];

  for(uint i = 0; i < m; i++){
    uint l, h;
    char mode;
    scanf("%d %d %c", &l, &h, &mode);
    if (mode == 'C'){
      paintNode(l, h, node, -1);
    } else if (mode == 'B'){
      paintNode(l, h, node, 1);
    }
    arr[i] = node->white_count;
  }

  for(uint i = 0; i<m; i++){
    printf("%d\n", arr[i]);
  }

  freeNodes(node);
  return 0;
}

