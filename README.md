# linked-list-structures
**Objectives:** Use structures and pointers to create linked lists. Use knowledge of pointers to modify linked lists.
Implement a recursive function.

**Problem:** Mario Paint was a great innovation in its time allowing people with a home console to create art and
music easily. One of the features of Mario Paint was that it let users create their own 8-bit stamps to use within
the game. With this idea, you are going to create a program that will allow users to create black and white pixel
art. Since all of the output will be file-based, we can’t easily add color to the drawings.

**Details:**
* The user’s canvas will be a grid (50 characters x 50 characters)
** The newline character on each line is not considered to be part of the canvas
** Remember that a newline character in Windows is treated as 2 bytes
* Input will consist of a series of commands read from a file
* Commands
  * 1 – Pen up
  * 2 – Pen down
  * 3, D, # – Move pen in given direction the given number of characters
    * Example – 3, N, 10 would move the pen up (north) 10 characters
    * Directions
      * N – north / up
      * S – south / down
      * E – east / right
      * W – west / left
      * 4 – Display output file in console
      * B – bold on
    * Use # to draw
    * b – bold off
      * Use * to draw
* All commands must be loaded into memory using a linked list
* Commands may move the pen outside of the canvas space
  * If this happens, ignore the command and delete it from the linked list
* The canvas will exist in memory
  * Do not directly modify the file (it is inefficient)
  * Create a solution to store the drawing in memory and then output the result to the file.
* If the pen is “down” and moved, record the given number of characters in the given direction
* By default, bold is off when the program begins

**Input:** All input will be read from a file named commands.txt. All commands in the file will be valid and of the
proper format. Each command will be on a separate line

The commands in the file will not be in chronological order. Each command must be loaded into an ordered
linked list.

Each command will be preceded by an alphabetic identifier. They should be stored in order from A to Z. If there
are more than 26 commands, the alphabet will be repeated with double letters, then triple, etc. (e.g. A…Z AA…ZZ
AAA…ZZZ, etc.)

**Output:** Each input file will produce an output file named paint.txt. If a print command (4) is listed in the input file,
display the current state of the entire file to the console window, making sure that each line of the output file is
displayed on a separate line in the console window. After the file has been displayed to the console, send two
blank lines to the console window as a buffer for the next print command in the file.

After the entire input file has been processed, display the completed artwork to the console and write the picture
to the output file before the program exits. Writing to the file should only be done once.

Also, overwrite the input file with the ordered list of commands with a recursive function. Remember that
commands that move/draw outside of the canvas are to be deleted from the list.
