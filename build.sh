#!/bin/sh

set -xe

cc -Wall -Werror -std=c11 -pedantic -ggdb -o trie trie.c

./trie > trie.dot

dot -Tsvg trie.dot > trie.svg
