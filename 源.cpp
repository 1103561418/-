#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义哈希表节点和哈希表结构
typedef struct Node {
    char word[100];
    int lines[100];
    int line_count;
    struct Node* next;
} Node;

typedef struct {
    Node* table[1000];
    int size;
} HashTable;

// 定义二叉排序树节点结构
typedef struct BSTNode {
    char word[100];
    int lines[100];
    int line_count;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// 哈希函数
int hash_function(char* word, int table_size) {
    int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash = (hash * 31 + word[i]) % table_size;
    }
    return hash;
}

// 插入单词到哈希表
void insert_word(HashTable* ht, char* word, int line_number) {
    int index = hash_function(word, ht->size);
    Node* node = ht->table[index];

    while (node != NULL) {
        if (strcmp(node->word, word) == 0) {
            node->lines[node->line_count++] = line_number;
            return;
        }
        node = node->next;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->word, word);
    new_node->lines[0] = line_number;
    new_node->line_count = 1;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
}

// 读取文件并构建哈希表
void read_file_and_build_hash_table(HashTable* ht, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[1000];
    int line_number = 1;
    while (fgets(line, sizeof(line), file)) {
        char* word = strtok(line, " \n");
        while (word) {
            insert_word(ht, word, line_number);
            word = strtok(NULL, " \n");
        }
        line_number++;
    }

    fclose(file);
}

// 打印哈希表内容
void print_hash_table(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* node = ht->table[i];
        while (node) {
            printf("%d\t%s", i, node->word);
            for (int j = 0; j < node->line_count; j++) {
                printf("\t%d", node->lines[j]);
            }
            printf("\n");
            node = node->next;
        }
    }
}

// 插入单词到二叉排序树
BSTNode* insert_into_bst(BSTNode* root, char* word, int line_number) {
    if (root == NULL) {
        BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
        strcpy(new_node->word, word);
        new_node->lines[0] = line_number;
        new_node->line_count = 1;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    if (strcmp(word, root->word) < 0) {
        root->left = insert_into_bst(root->left, word, line_number);
    }
    else if (strcmp(word, root->word) > 0) {
        root->right = insert_into_bst(root->right, word, line_number);
    }
    else {
        root->lines[root->line_count++] = line_number;
    }
    return root;
}

// 将哈希表内容转换为二叉排序树
void convert_hash_to_bst(HashTable* ht, BSTNode** bst_root) {
    for (int i = 0; i < ht->size; i++) {
        Node* node = ht->table[i];
        while (node) {
            for (int j = 0; j < node->line_count; j++) {
                *bst_root = insert_into_bst(*bst_root, node->word, node->lines[j]);
            }
            node = node->next;
        }
    }
}

// 中序遍历打印二叉排序树
void inorder_traversal(BSTNode* root) {
    if (root) {
        inorder_traversal(root->left);
        printf("%s", root->word);
        for (int i = 0; i < root->line_count; i++) {
            printf("\t%d", root->lines[i]);
        }
        printf("\n");
        inorder_traversal(root->right);
    }
}

// 主函数
int main() {
    HashTable ht;
    ht.size = 1000;
    for (int i = 0; i < ht.size; i++) {
        ht.table[i] = NULL;
        ht.table[i] = NULL;
    }

    // 读取文件并构建哈希表
    read_file_and_build_hash_table(&ht, "input.txt");

    // 打印哈希表内容
    printf("Hash Table:\n");
    print_hash_table(&ht);

    // 将哈希表内容转换为二叉排序树
    BSTNode* bst_root = NULL;
    convert_hash_to_bst(&ht, &bst_root);

    // 中序遍历打印二叉排序树
    printf("\nBinary Search Tree (Inorder Traversal):\n");
    inorder_traversal(bst_root);

    return 0;
}