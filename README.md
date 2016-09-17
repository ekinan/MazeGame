
# Maze Game
------------------------------------------------------------------------------------------------------------------
This was a text-based maze game that I wrote for a project in Portland
State University's CS202 course. It consists of two randomly generated 
levels. The main goal of each level is for the player to get from the 
starting position to the finish. Along the way, they can either pick up 
coins to boost their score, acquire a "power" that they can use to figure 
out which way leads to the finish, or hit a dead end and lose a life. The 
player can start over at any point in the maze, or they can choose to try 
a different one instead. The game continues until the player either gives
up or reaches the finish point, whereby their overall score is then
calculated and printed to the screen. The player gains points based
on how many remaining lives they have, and coins and powers that they've
acquired. Note that the player also has the option to see the maze's
structure themselves, where an "X" corresponds to a dead-end, a "P"
corresponds to the players' current position, "O" is an intersection,
and "F" is the finish point.

Level One is organized as a binary search tree with randomly generated 
keys. There are three possible directions the player can move: Right,
Left, and Backwards. The maze is displayed in accordance with the tree's
structure ("X" represents NULL pointers, or dead ends). Level Two uses 
an adjacency list, randomly generating each node in a breadth-first 
fashion. There are five possible directions here: Straight, Backwards, 
Diagonal, Right, and Left. The random generation uses the following 
method for a given Node A:
	1. Randomly generate the number of children A will have.
	2. Randomly assign which of the five directions each child will
	represent.
	3. Repeat Steps 1-2 for A's children, until enough nodes have
	been created, in a breadth-first fashion.
	
Level Two is displayed as an adjacency list. "S", "L", "R", and "D"
stand for "Straight", "Left", "Right", and "Diagonal," respectively.

------------------------------------------------------------------------------------------------------------------
To compile the program, please type
	make clean
	make
into the command prompt.

To run the program, please type
	./a.out
into the command prompt.

