# Spelling correction

Implementing a spelling corrector: a program that takes a document and attempts to correct misspelled words within that document.

Task 1: Computing edit distance
Implement the function print edit distance() defined in spell.c. This function has two input parameters: word1 and word2 (strings). The function should compute the Levenshtein edit distance between the strings word1 and word2. That is, your program should compute the minimum number of 'edits' required to transform word1 into word2, where edits are dened as letter substitutions, deletions or insertions.

Task 2: Enumerating all possible edits
Implement the function print all edits() defined in spell.c. This function has one input parameter: word (a string). The function should generate and print all lower case alphabetic strings within a Levenshtein edit distance of 1 from word. That is, all of the strings made of alphabetic characters ('abcdefghijklmnopqrstuvwxyz') that can be made from substituting a single letter in word, inserting a single letter into word, or deleting a single letter from word.

Task 3: Spell checking
Implement the function print checked() defined in spell.c. This function has two input parameters: dictionary (a List pointer), a list of strings representing correctly-spelled words in approximate order of decreasing probability of occurrence; and document (another List pointer), a list of strings to check for spelling mistakes. For each string (word) in document, the function should check if the string occurs in dictionary (that is, if it's a correctly-spelled word).

Task 4: Spelling correction
Implement the function print corrected() defined in spell.c. This function has two input parameters: dictionary (a List pointer), a list of strings representing correctly-spelled words in approximate order of decreasing probability of occurrence; and document (another List pointer), a list of strings to check for spelling mistakes (and attempt to correct).
For each string (word) in document, in order, the function should do the following:
  1. If the string occurs in dictionary (that is, if it's a correctly-spelled word) the function should print this word directly to the standard output on its own line.
  2. If the string does not occur in dictionary your function should try to find a 'corrected' version of the string with a Levenshtein edit distance of 1 in dictionary. If such a word exists, the function should print this word to the standard output on its own line. If multiple such words exist, the function should print only the word that occurs first among them in dictionary.
  3. If the string does not occur in dictionary and no words in dictionary have a Levenshtein edit distance of 1 with the string, then your program should try to find a 'corrected' version of the string with a Levenshtein edit distance of 2 in dictionary. If such a word exists, the function should print this word to the standard output on its own line. If multiple such words exist, the function should print only the word that occurs first among them in dictionary.
  4. If none of the first three conditions are met, then the function should attempt to find a 'corrected' version of the string with a Levenshtein edit distance of 3 in dictionary. If such a word exists, the function should print this word to the standard output on its own line. If multiple such words exist, the function should print only the word that occurs first among them in dictionary.
  5. If none of the previous conditions are met, then the function should print the original string to the standard output followed by a question mark, on its own line. The following format string will be useful: "%s?\n".
  
Task 5: Design of algorithms
First, discuss your approach to task 3 and task 4 in terms of the data structures and algorithms you used and the resulting asymptotic time complexity of these functions. Second, comment on any alternative approaches you considered, and why you ended up choosing your approach for each task.
