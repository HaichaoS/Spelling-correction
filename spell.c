/* Assignment 2: Spelling correction
 * task 1 compute the Levenshtein edit distance between two strings
 * task 2 generate and print all lower case alphabetic strings 
 * within a Levenshtein edit distance of 1 from word
 * task 3 check if every strings in document occurs in dictionary
 * task 4 correct every strings in document to dictionary under distance 3
 *
 * Solution written by Haichao Song 854035
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spell.h"
#include "list.h"
#include "hashtbl.h"
#include "strhash.h"

/****************************************************************/

#define SIZE_MULT  53   /* the string length times 53 then add 26 */
#define SIZE_ADD   26   /* will produce the number of distance 1 strings */
#define MAX_DIFF   3    /* the max length difference for distance 3 */
#define HIS_NULL   " "  /* store in history means no changed strings */
#define AVERAGE    0.5  /* average case that find distance string in linear */


/****************************************************************/

/* function prototypes */
int edit_distance(char *word1, char *word2);
HashTable *get_distance_1(char *word);
void print_distance_1(List *list, HashTable *table, char *word, 
    HashTable *history);
void print_distance_2(List *list, HashTable *table_1,
    HashTable *table_2, char *word, HashTable *history);
void linear_print(List *list, char *word, int distance, HashTable *history);

/****************************************************************/

/* task 1 print Levenshtein edit distance between two strings */
void print_edit_distance(char *word1, char *word2) {
    assert(word1 != NULL);
    assert(word2 != NULL);
    int distance = edit_distance(word1, word2);
	printf("%d\n", distance);
}

/****************************************************************/

/* compute the Levenshtein edit distance between two strings */
int edit_distance(char *word1, char *word2) {
    assert(word1 != NULL);
    assert(word2 != NULL);
    int n = strlen(word1);
    int m = strlen(word2);
    int cost;

    /* make the Levenshtein table */
    int E[n+1][m+1];
    for (int i = 0; i < n+1; i++) {
        E[i][0] = i;
    }
    for (int j = 1; j < m+1; j++) {
        E[0][j] = j;
    }

    /* compare each letter in two words and finish the table */
    for (int i = 1; i < n+1; i++) {
        for (int j = 1; j < m+1; j++) {
            if (word1[i-1] == word2[j-1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            if (E[i-1][j-1]+cost <= E[i-1][j]+1 && 
                E[i-1][j-1]+cost <= E[i][j-1]+1) {
                E[i][j] = E[i-1][j-1]+cost;
            } else if (E[i-1][j]+1 <= E[i-1][j-1]+cost && 
                E[i-1][j]+1 <= E[i][j-1]+1) {
                E[i][j] = E[i-1][j]+1;
            } else {
                E[i][j] = E[i][j-1]+1;
            }
        }
    }
    return E[n][m];
}

/****************************************************************/

/* task 2 print all lower case alphabetic strings 
 * within a Levenshtein edit distance of 1 from word
 */
void print_all_edits(char *word) {
    assert(word != NULL);
    HashTable *distance_1 =  get_distance_1(word);
    print_hash_table(distance_1);
    free_hash_table(distance_1);
}

/****************************************************************/

/* generate all lower case alphabetic strings in 1 distance from word
 * put strings into a hashtable and return it
 */
HashTable *get_distance_1(char *word) {
    assert(word != NULL);
	char alphabetic[26] = "abcdefghijklmnopqrstuvwxyz";
    int length = strlen(word), index;
    char delet[length-1];
    char insert[length+1];
    char sub[length];
    unsigned int table_size = SIZE_MULT * length + SIZE_ADD;
    HashTable *distance_1 = new_hash_table(table_size);
    
    /* put all possible distance 1 strings by using deletion
     * into the hashtable
     */
    if (length > 1) {
        for (int i = 0; i < length; i++){
            for(int j = 0; j < length; j++) {
                if (j < i) {
                    delet[j] = word[j];
                } else if (j > i) {
                    delet[j-1] = word[j];
                }
            }
            delet[length-1] = '\0';
            index = h(delet, table_size);
            hash_table_put(distance_1, delet, index);
        }
    }
    
    /* put all possible distance 1 strings by using insertion
     * into the hashtable
     */
    for(int i = 0; i < length+1 ; i++){
        for (int j = 0; j < strlen(alphabetic); j++){
            for(int k = 0; k < length+1; k++) {
                if (i < k) {
                    insert[k] = word[k-1];
                } else if (i == k) {
                    insert[k] = alphabetic[j];
                } else {
                    insert[k] = word[k];
                }
            }
            insert[length+1] = '\0';
            index = h(insert, table_size);
            hash_table_put(distance_1, insert, index);
        }
    }
    
    /* put all possible distance 1 strings by using substitution
     * into the hashtable
     */
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < strlen(alphabetic); j++){
            for(int k = 0; k < length; k++) {
                if (i == k) {
                    sub[k] = alphabetic[j];
                } else {
                    sub[k] = word[k];
                }
            }
            sub[length] = '\0';
            index = h(sub, table_size);
            hash_table_put(distance_1, sub, index);
        }
    }

    return distance_1;
}

/****************************************************************/

/* task 3 check if every strings in document occurs in dictionary */
void print_checked(List *dictionary, List *document) {
    assert(dictionary != NULL);
    assert(document != NULL);

    /* put strings in the dictionary into a hashtable */
    unsigned int table_size = dictionary->size;
    HashTable *diction = new_hash_table(table_size);
    for(Node *process = dictionary->head; process != NULL; 
        process = process->next) {
        int index = h(process->data, table_size);
        hash_table_put(diction, process->data, index);
    }
    
    /* check if every words in document is in the hashtable */
    for(Node *process = document->head; process != NULL; 
        process = process->next) {
        if (hash_table_has(diction, process->data) == 1) {
            printf("%s\n", (char *)process->data);
        } else {
            printf("%s?\n", (char *)process->data);  
        }     
    }
    free_hash_table(diction);
}

/****************************************************************/

/* task 4 correct strings in document to dictionary below  distance 3 */
void print_corrected(List *dictionary, List *document) {
    assert(dictionary != NULL);
    assert(document != NULL);

    /* put strings in the dictionary into a hashtable with its order */
	unsigned int dic_size = dictionary->size;
	int order = 0;
	HashTable *diction = new_hash_table(dic_size);
	for(Node *process = dictionary->head; process != NULL; 
        process = process->next) {
		hash_table_put(diction,process->data, order);
		order += 1;
	}

    /* create a hashtable to store all the changed strings */
    HashTable *history = new_hash_table(document->size);

    /* check if every words in document is in the hashtable
     * if it is print it directly
     * else find if it is in history
     * else run print_distance_1
     */
	for (Node *process = document->head; process != NULL; 
        process = process->next) {
		if (hash_table_has(diction, process->data) == 1) {
			printf("%s\n", (char *)process->data);
		} else if (hash_table_has_history(history, process->data) == 0) {
			print_distance_1(dictionary, diction, process->data, history);
		}
	}
    free_hash_table_history(history);
    free_hash_table(diction);
}

/****************************************************************/

/* check if there is a string in dictionary has distance 1 with word */
void print_distance_1(List *list, HashTable *table, 
    char *word, HashTable *history){
    assert(list != NULL);
    assert(table != NULL);
    assert(word != NULL);

    /* generare a hashtable including all strings 
     * have distance 1 with the word 
     * check if strings are in dictionary 
     * and store the string with highest order
     */
	HashTable *distance_1 = get_distance_1(word);
    int order = table->size, rank;
    char *result = NULL;
	for(int i = 0; i < distance_1->size; i++){
		for(Bucket *bucket = distance_1->buckets[i]; bucket != NULL; 
            bucket = bucket->next) {
			if(hash_table_has(table, bucket->key) == 1) {
				rank = hash_table_get(table, bucket->key);
				if (rank < order) {
					order = rank;
					result = bucket->key;
				}
			}
		}
	}

    /* if there is, print the string and store in history
     * else run print_distance_2
     */
	if (order == table->size) {
		print_distance_2(list, table, distance_1, word, history);
	} else {
		printf("%s\n", result);
        hash_table_put_history(history, word, result);
	}
    free_hash_table(distance_1);
}

/****************************************************************/

/* check if there is a string in dictionary has distance 2 with word */
void print_distance_2(List *list, HashTable *table_1,HashTable *table_2,
 char *word, HashTable *history) {
    assert(list != NULL);
    assert(table_1 != NULL);
    assert(table_2 != NULL);
    assert(word != NULL);
    int find = 0;
    int l = strlen(word);

    /* if linear search has smaller size
     * linear search the distance 2 word
     */
    if((SIZE_MULT * l + SIZE_ADD) * (SIZE_MULT * l + SIZE_ADD)
        > AVERAGE * table_1->size) {
        linear_print(list, word, 2, history);
    } else {

        /* generare a hashtable including all strings 
         * have distance 1 with distance 1 strings 
         * check if strings are in dictionary 
         * and store the string with highest order
         */
        int order = table_1->size, rank;
        char result[strlen(word)+ MAX_DIFF];
        HashTable *distance_2;
	    for(int i = 0; i < table_2->size; i++) {
            for(Bucket *bucket_1 = table_2->buckets[i]; bucket_1!= NULL; 
                bucket_1 = bucket_1->next) {
                distance_2 = get_distance_1(bucket_1->key);
                for (int j = 0; j < distance_2->size; j++) {
                    for (Bucket *bucket_2 = distance_2->buckets[j]; 
                    bucket_2!= NULL; bucket_2 = bucket_2->next) {
                        if(hash_table_has(table_1, bucket_2->key) == 1) {
                            find = 1;
                            rank = hash_table_get(table_1, bucket_2->key);
                            if (rank < order) {
                                order = rank;
                                strcpy(result, bucket_2->key);
                            }
                        }
                    }
                }
                free_hash_table(distance_2); 
            }
        }
        
        /* if find print the string and store in history
         * else use linear to run distance three
         */
        if (find) {
            printf("%s\n",result);
            hash_table_put_history(history, word, result);
        } else {
            linear_print(list, word, 3, history);
        }
    }
}

/****************************************************************/

/* check if there is a string in dictionary has distance 3 with word */
void linear_print(List *list, char *word, int distance, 
    HashTable *history) {
    assert(list != NULL);
    assert(word != NULL);
    int find = 0;
    char dis_3[strlen(word) + MAX_DIFF];
    int dis_3_find = 0;

    /* use linear search to compute distance between 
     * every words in dictionary and in document in order
     * if find distance 3 word, record it
     * if find a distance word then print and break
     * else return not find
     */
    for(Node *process = list->head; process != NULL; 
        process = process->next) {
        int length_diff = strlen(process->data) - strlen(word);
        int difference = edit_distance(process->data, word);
        if (length_diff > MAX_DIFF || length_diff < MAX_DIFF) {
            if (difference == MAX_DIFF && dis_3_find == 0) {
                strcpy(dis_3, (char *)process->data);
                dis_3_find = 1;
            }
            if (difference == distance && find == 0) {
                printf("%s\n", (char *)process->data);
                hash_table_put_history(history, word, (char *)process->data);
                find = 1;
                break;
            }
        }
    }
    
    /* if find distance word directly print and record in history
     * if distance not find but distance 3 find print distance 3 word nd record
     * if neither is find print word and ?
     */
    if (!find && dis_3_find) {
        printf("%s\n", dis_3);
        hash_table_put_history(history, word, dis_3);
    }  else if (!find && !dis_3_find) {
        printf("%s?\n", word);
        hash_table_put_history(history, word, HIS_NULL);
    }
}


