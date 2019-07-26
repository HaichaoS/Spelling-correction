/* * * * * * *
 * Hash table with separate chaining in a linked list
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * move-to-front added by ...
 */

#include <stdbool.h>

typedef struct table HashTable;

HashTable *new_hash_table(int size);
void free_hash_table(HashTable *table);
void hash_table_put(HashTable *table, char *key, int value);
int  hash_table_get(HashTable *table, char *key);
bool hash_table_has(HashTable *table, char *key);
void print_hash_table(HashTable *table);
int h(char *key, int size);

/*********************************************************************/

void free_hash_table_history(HashTable *table);
void hash_table_put_history(HashTable *table, char *key, char *key_after);
bool hash_table_has_history(HashTable *table, char *key);


typedef struct bucket Bucket;
struct bucket {
    char *key;
    char *key_after;
    unsigned int value;
    Bucket *next;
};

struct table {
    int size;
    Bucket **buckets;
};


