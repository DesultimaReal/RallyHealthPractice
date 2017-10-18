//PROGRAM README
I Wrote this code as a project for an interview
with RallyHealth
/*This program takes in four "cost" inputs and two words
and uses a dictionary to find the shortest path between them.
The four costs are: Cost to add a letter, delete a letter, change a letter, and take an anagram.
The second input it takes are two words in the dictionary you wish to find the shortest cost between.

Example input:
1 3 1 5
health
hands

Example output:
7

Why:
health-> heath-> heats -> hents-> hends-> hands
So... delete a letter, change a letter, change a letter, change a letter,change a letter
for a cost of 3+1+1+1+1 to do these steps.

It uses dijkstra's to accomplish the shortest path, and I update in real time rather than building the paths first.

*/