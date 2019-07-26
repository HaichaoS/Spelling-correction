/* * * * * * *
 * Hash table with separate chaining in a linked list with move-to-front
 *
 * created for COMP20007 Design of Algorithms
 * partly by Matt Farrugia <matt.farrugia@unimelb.edu.au
 * partly by Haichao Song 854035
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hashtbl.h"

#include <string.h>
#include "strhash.h"
#define HASH_METHOD 'x'

#define PRINT_LIMIT 10


/* * *
 * HELPER DATA STRUCTURE: LINKED LIST OF BUCKETS
 */



Bucket *new_bucket(char *key, int value) {
	Bucket *bucket = malloc(sizeof *bucket);
	assert(bucket);

	// create own copy of key for storage in table
	bucket->key = malloc((sizeof *bucket->key) * (strlen(key) + 1));
	assert(bucket->key);
	strcpy(bucket->key, key);
	
	bucket->value = value;
	bucket->next = NULL;

	return bucket;
}

// Warning: does not free bucket->next
void free_bucket(Bucket *bucket) {
	assert(bucket != NULL);
	free(bucket->key);
	free(bucket);
}


/* * *
 * HASH TABLE CREATION/DELETION
 */

HashTable *new_hash_table(int size) {
	HashTable *table = malloc(sizeof *table);
	assert(table);

	table->size = size;
	table->buckets = malloc(size * (sizeof *table->buckets));
	assert(table->buckets);
	int i;
	for (i = 0; i < size; i++) {
		table->buckets[i] = NULL;
	}

	return table;
}

void free_hash_table(HashTable *table) { 
	assert(table != NULL);

	int i;
	for (i = 0; i < table->size; i++) {
		Bucket *this_bucket, *next_bucket;
		this_bucket = table->buckets[i];
		while (this_bucket) {
			next_bucket = this_bucket->next;
			free_bucket(this_bucket);
			this_bucket = next_bucket;
		}
	}
    free(table->buckets);
   	free(table);
}


/* * *
 * HASHING HELPER FUNCTIONS
 */

int h(char *key, int size) {
	return hash(key, size, HASH_METHOD);
}
bool equal(char *a, char *b) {
	return strcmp(a, b) == 0;
}


/* * *
 * HASH TABLE FUNCTIONS
 */

void hash_table_put(HashTable *table, char *key, int value) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	Bucket *previous = NULL;
	while (bucket) {
		if (equal(key, bucket->key)) {

			// move to front on modify
			if (previous) {
				previous->next = bucket->next;
				bucket->next = table->buckets[hash_value];
				table->buckets[hash_value] = bucket;
			}

			bucket->value = value;
			return;
		}
		previous = bucket;
		bucket = bucket->next;
	}

	// if key wasn't found, add it at front of list
	Bucket *new = new_bucket(key, value);
	new->next = table->buckets[hash_value];
	table->buckets[hash_value] = new;
}

int hash_table_get(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	Bucket *previous = NULL;
	while (bucket) {
		if (equal(key, bucket->key)) {
			
			// move to front on access
			if (previous) {
				previous->next = bucket->next;
				bucket->next = table->buckets[hash_value];
				table->buckets[hash_value] = bucket;
			}
			return bucket->value;
		}
		previous = bucket;
		bucket = bucket->next;
	}

	// key doesn't exist!
	fprintf(stderr, "error: key \"%s\" not found in table\n", key);
	exit(1);
}

bool hash_table_has(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	Bucket *previous = NULL;
	while (bucket) {
		if (equal(key, bucket->key)) {
			
			// move to front on lookup
			if (previous) {
				previous->next = bucket->next;
				bucket->next = table->buckets[hash_value];
				table->buckets[hash_value] = bucket;
			}

			return true;
		}
		previous = bucket;
		bucket = bucket->next;
	}

	// key doesn't exist!
	return false;
}


/* * *
 * PRINTING FUNCTIONS
 */
void print_bucket_key(Bucket *b);

void print_hash_table(HashTable *table) {
    assert(table != NULL);
    unsigned int i;
    for(i = 0; i < table->size; i++){
        print_bucket_key(table->buckets[i]);
    }
    return;
}

void print_bucket_key(Bucket *b) {
    Bucket *tmp = b;
    while(tmp != NULL) {
        printf("%s\n",tmp->key);
        tmp = tmp->next;
    }
}

/***************************************************************/

/* Hashtable has two keys */

Bucket *new_bucket_history(char *key, char *key_after) {
    Bucket *bucket = malloc(sizeof *bucket);
    assert(bucket);
    
    // create own copy of key for storage in table
    bucket->key = malloc((sizeof *bucket->key) * (strlen(key) + 1));
    assert(bucket->key);
    strcpy(bucket->key, key);
    
    bucket->key_after = malloc((sizeof *bucket->key_after) *
                               (strlen(key_after) + 1));
    assert(bucket->key_after);
    strcpy(bucket->key_after, key_after);
    
    bucket->next = NULL;
    
    return bucket;
}

void free_bucket_history(Bucket *bucket) {
    assert(bucket != NULL);
    free(bucket->key);
    free(bucket->key_after);
    free(bucket);
}

void free_hash_table_history(HashTable *table) {
    assert(table != NULL);
    
    int i;
    for (i = 0; i < table->size; i++) {
        Bucket *this_bucket, *next_bucket;
        this_bucket = table->buckets[i];
        while (this_bucket) {
            next_bucket = this_bucket->next;
            free_bucket_history(this_bucket);
            this_bucket = next_bucket;
        }
    }
    free(table->buckets);
    free(table);
}

void hash_table_put_history(HashTable *table, char *key, char *key_after) {
    assert(table != NULL);
    assert(key != NULL);
    assert(key_after != NULL);
    
    int hash_value = h(key, table->size);
    
    // look for existing key
    Bucket *bucket = table->buckets[hash_value];
    Bucket *previous = NULL;
    while (bucket) {
        if (equal(key, bucket->key)) {
            
            // move to front on modify
            if (previous) {
                previous->next = bucket->next;
                bucket->next = table->buckets[hash_value];
                table->buckets[hash_value] = bucket;
            }
            
            bucket->key_after = key_after;
            return;
        }
        previous = bucket;
        bucket = bucket->next;
    }
    
    // if key wasn't found, add it at front of list
    Bucket *new = new_bucket_history(key, key_after);
    new->next = table->buckets[hash_value];
    table->buckets[hash_value] = new;
}

bool hash_table_has_history(HashTable *table, char *key) {
    assert(table != NULL);
    assert(key != NULL);
    
    int hash_value = h(key, table->size);
    
    // look for existing key, if it has print it
    // else print key and ?
    Bucket *bucket = table->buckets[hash_value];
    Bucket *previous = NULL;
    while (bucket) {
        if (equal(key, bucket->key)) {
            if(!equal(bucket->key_after," ")) {
                printf("%s\n",bucket->key_after);
            } else {
                printf("%s?\n", key);
            }
            
            return true;
        }
        previous = bucket;
        bucket = bucket->next;
    }
    
    // key doesn't exist!
    return false;
}






