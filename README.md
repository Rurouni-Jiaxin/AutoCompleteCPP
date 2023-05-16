# AutoCompleteCPP
Given the prefix, this CLI tool will print out all matching words in the given .txt file
This is my attempt to solve the problem using the Trie data structure with BFS. I tried to perform BFS using more than one thread, cannot achieve any increase in performance, it only got slower. Maybe I should not have used shared memory, but sharing messages instead ?

The timer is borrowed from my friend Jakob: https://github.com/Jakobimatrix/timer 
