#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fruits.h"

#define ARRAY_LEN(xs) (sizeof(xs) / sizeof((xs)[0]))

typedef struct Node Node;

struct Node{
    Node *children[256];
};

#define NODE_POOL_CAP 1024
Node node_pool[NODE_POOL_CAP] = {0};
size_t node_pool_count = 0;

Node *alloc_node(void)
{
    assert(node_pool_count < NODE_POOL_CAP);
    return &node_pool[node_pool_count++];
}

void insert_text(Node *root, const char *text)
{
    if (text == NULL || *text == '\0')
    {
        return;
    }

    assert(root != NULL);

    size_t index = (size_t) *text;

    if (root->children[index] == NULL) {
        root->children[index] = alloc_node();
    }

    insert_text(root->children[index], text + 1);
}

// dot emphasizes the graphviz
void dump_dot(Node *root)
{
    size_t index = root - node_pool;

    for (size_t i = 0; i < ARRAY_LEN(root->children); ++i) {
        if (root->children[i] != NULL) {
            size_t child_index = root->children[i] - node_pool;
            printf("    Node_%zu[label=\"%c\"]\n", child_index, (char) i);
            printf("    Node_%zu -> Node_%zu [label=\"%c\"]\n", index, child_index, (char)i);
            dump_dot(root->children[i]);
        }
    }

}

void usage(FILE *sink)
{
    fprintf(sink, "USAGE: ./trie <SUBCOMMAND>\n");
    fprintf(sink, "SUBCOMMANDS:\n");
    fprintf(sink, "    visualize          Dump the Trie into a Graphviz dot file.\n");
    fprintf(sink, "    complete <prefix>  Suggest autocompletion for prefix based on the Trie.\n");
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        usage(stderr);
        fprintf(stderr, "ERROR: No subcommand is provided\n");
    }

    const char *subcommand = argv[1];

    if (strcmp(subcommand, "visualize") == 0) {
        Node *root = alloc_node();
        for (size_t i = 0; i < fruits_count; ++i){
            insert_text(root, fruits[i]);
        }
        printf("digraph Trie{\n");
        printf("Node_%zu [label=root]\n", root - node_pool);
        dump_dot(root);
        printf("}\n");
    } else if (strcmp(subcommand, "complete") == 0) {
        assert(0 && "TODO: complete not impletemented yet\n");
    } else {
        usage(stderr);
        fprintf(stderr, "ERROR: unknown subcommand `%s`\n", subcommand);

        exit(1);
    }
}

