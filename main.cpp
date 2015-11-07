/**
 * Brandon Ferrell
 * fbb150030
 * Project 3 (Maintenance Project 2)
 * 10/26/15
 * CS 1337.002
 * Jason Smith
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

// Node for the linked list
struct node
{
    string sort;
    char command;
    char direction;
    string spaces;
    node* next;
};

// Linked List Functions
void addNode(node* &, string, char, char, string);
void buildLinkedList(node* &, string);
void alterLinkedList(node*);
void deleteLinkedList(node* &);
void runCommands(node*, char arr[][50]);

// Command Functions
void printArray(char arr[][50]);
void drawUpDown(char arr[][50], bool, bool, string, char, int &, int);
void drawLeftRight(char arr[][50], bool, bool, string, char, int, int &);

// File Manipulation Functions
void writeToInput(fstream&, node*);
void writeToOutput(fstream&, char arr[][50]);

int main()
{
    // Variable to hold each line of input
    string line;

    // Open input and output files
    fstream in("commands.txt", ios::in);
    fstream out("paint.txt", ios::out);

    // Create first linked list pointer
    node* head = nullptr;

    // Initialize grid for drawing
    char grid[50][50] = {' '};

    // Build an ordered linked list line by line
    while(getline(in, line))
    {
        buildLinkedList(head, line);
    }

    // Close input file as we have extracted all we need
    in.close();

    // Delete out-of-bounds commands
    alterLinkedList(head);

    // Run the commands in the linked list
    runCommands(head, grid);

    // Print completed drawing
    printArray(grid);

    // Open commands file to overwrite with ordered and filtered commands
    fstream in_out("commands.txt", ios::out);

    // Overwrite commands file with ordered and filtered commands
    writeToInput(in_out, head);

    // Close the commands file
    in_out.close();

    // Write to the output file with the finished drawing
    writeToOutput(out, grid);

    // Close the canvas file
    out.close();

    // Delete the linked list from memory
    deleteLinkedList(head);

    // Return saying program has run successfully
    return 1;
}

/**
 * Build the linked list from each line, and order them according to the sorting pattern
 */
void buildLinkedList(node* &h, string line)
{
    // Get length of sorting string
    int chars = 0;
    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] == ' ')
            break;
        else chars++;
    }

    // Determine what arguments to use when adding the node
    switch(line[chars+1])
    {
        case '3':
            // If drawing, pass all arguments
            addNode(h, line.substr(0,chars), '3', line[chars+4], line.substr(chars+7));
            break;
        case '1':
        case '2':
        case '4':
        case 'B':
        case 'b':
            // If not drawing, only pass the first two arguments from the command
            addNode(h, line.substr(0,chars), line[chars+1], '\0', "");
            break;
    }
}

/**
 * Delete out-of-bounds commands in the linked list
 */
void alterLinkedList(node* h)
{
    // Set up variables to hold the index of each dimension
    int i = 0;
    int j = 0;

    // Set up variables to to hold the command's amount of movement
    int mi = 0;
    int mj = 0;

    // Create node pointers for deleting invalid nodes
    node* p2 = h;
    node* p1 = h->next;

    // Handle i and j for first node
    switch(p2->direction)
    {
        case 'N':
            i = 0 - atoi(p2->spaces.c_str());
            break;
        case 'S':
            i = atoi(p2->spaces.c_str());
            break;
        case 'E':
            j = atoi(p2->spaces.c_str());
            break;
        case 'W':
            j = 0 - atoi(p2->spaces.c_str());
            break;
    }

    // While the furthest pointer points to a node
    while (p1)
    {
        // If the command says to draw
       if(p1->command == '3')
       {
            // Figure out which direction it is moving and write to mi/mj accordingly
            // Negative values are used when moving N and W
            switch(p1->direction)
            {
                case 'N':
                    mi = 0 - atoi(p1->spaces.c_str());
                    break;
                case 'S':
                    mi = atoi(p1->spaces.c_str());
                    break;
                case 'E':
                    mj = atoi(p1->spaces.c_str());
                    break;
                case 'W':
                    mj = 0 - atoi(p1->spaces.c_str());
                    break;
            }

            // If the movement causes an out-of-bounds problem
            if(i + mi < 0 || i + mi > 49 || j + mj < 0 || j + mj > 49)
            {
                // Reconnect the nodes around the invalid node
                node* ptr = p1;
                p1 = p1->next;
                p2->next = p1;

                // Delete the invalid node
                delete ptr;
            }
            else
            {
                // If the movement stays in bounds, increment the indexes with the command's value
                i += mi;
                j += mj;
            }

            // Reset the placeholders for the command's movement value
            mi = 0;
            mj = 0;
       }
       // If the last pointer is pointing to a node
       if(p1)
       {
            // Move each pointer one to the right
            p2 = p1;
            p1 = p1->next;
       }
    }
}

/**
 * Add a node to the linked list in a sorted fashion
 */
void addNode(node* &h, string sort, char command, char direction, string spaces)
{
    // Create a new node
    node* ptr = new node;

    // Set up node variables according to the passed variables
    ptr->sort = sort;
    ptr->command = command;
    ptr->direction = direction;
    if(spaces != "") ptr->spaces = spaces;
    ptr->next = nullptr;

    // If this is the first node, make the head pointer point to the new node
    if (h == nullptr)
        h = ptr;
    // If the sorting command says the node should be placed at the front
    else if(ptr->sort.length() < h->sort.length() || (ptr->sort.length() == h->sort.length() && ptr->sort < h->sort))
    {
        // Place new node at the beginning and move the head pointer to that node
        ptr->next = h;
        h = ptr;
    }
    else
    {
        // Create node pointers for adding a node the the middle
        node* p1 = h->next;
        node* p2 = h;

        // While the furthest pointer points to a node
        while(p1)
        {
            // If this is the place to put the node
            if(ptr->sort.length() < p1->sort.length() || (ptr->sort.length() == p1->sort.length() && ptr->sort < p1->sort))
            {
                // Integrate node and return
                p2->next = ptr;
                ptr->next = p1;
                return;
            }
            else
            {
                // Move pointers for next iteration
                p2 = p1;
                p1 = p1->next;
            }
        }
        // Add node to the end
        p2->next = ptr;
    }
}

/**
 * Run the commands in the linked list
 */
void runCommands(node* h, char grid[][50])
{
    // Set up variables for pen and bold status
    bool bold_status = false;
    bool pen_status = false;

    // Set up variables for the grid index
    int i = 0;
    int j = 0;

    // While the pointer points to a node
    while(h)
    {
        // Figure out which command the node uses
        switch(h->command)
        {
            case '1':
                // Pull the pen up
                pen_status = false;
                break;
            case '2':
                // Place the pen down
                pen_status = true;
                break;
            case '3':
                // If direction is North or South
                if(h->direction == 'N' || h->direction == 'S')
                    // Evoke function to draw up or down
                    drawUpDown(grid, pen_status, bold_status, h->spaces, h->direction, i, j);
                // If direction is East or West
                else
                    // Evoke function to draw left or right
                    drawLeftRight(grid, pen_status, bold_status, h->spaces, h->direction, i, j);

                break;
            case '4':
                // Print the grid as it stands
                printArray(grid);
                break;
            case 'B':
                // Pen is now bold
                bold_status = true;
                break;
            case 'b':
                // Pen is not bold
                bold_status = false;
                break;
        }
        // Move node pointer to next node
        h = h->next;
    }
}

/**
 * Print the grid array as it currently stands
 */
void printArray(char grid[][50])
{
    // Iterate through the array on first dimension
    for(int i = 0; i < 50; i++)
    {
        // Iterate through the array on second dimension
        for(int j = 0; j < 50; j++)
        {
            // Display character at the current index to the console
            cout << grid[i][j];
        }
        // Create a new line
        cout << endl;
    }
    // Create another new line to clear buffer
    cout << endl;
}

/**
 * Draw up or down on the grid depending on the variables passed
 */
void drawUpDown(char grid[][50], bool pen_status, bool bold_status, string spaces, char direction, int &i, int j)
{
    // Get the number or spaces to move
    int num_chars = atoi(spaces.c_str());

    // Set up variables to hold whether the moving index value should be negative
    bool negative;

    // If the node's direction is North
    if(direction == 'N')
    {
        // The index movement is negative
        negative = true;
    }
    // If the node's direction is South
    else
    {
        // The index movement is positive
        negative = false;
    }

    // If the pen is up
    if(!pen_status)
    {
        // Move index depending on if the index movement is negative
        if(negative)
            i -= num_chars;
        else i += num_chars;
    }
    // If the pen is down
    else
    {
        // Loop through integers counting to the number or spaces to move
        for(int k = 1; k <= num_chars; k++)
        {
            // If the pen is bold
            if(bold_status)
            {
                // Write bold character depending on if the index movement is negative
                if(negative)
                    grid[i-k][j] = '#';
                else grid[i+k][j] = '#';
            }
            // If the pen is not bold
            else
            {
                // If the index movement is negative
                if(negative)
                {
                    // If the index writing to is not bold, write a non-bold character
                    if(grid[i-k][j] != '#')
                        grid[i-k][j] = '*';
                }
                // If the index movement is positive and the index writing to is not bold
                else if(grid[i+k][j] != '#')
                {
                    // Write a non-bold character
                    grid[i+k][j] = '*';
                }
            }
        }
        // Increment/decrement the index depending on if the index movement is negative
        if(negative)
            i -= num_chars;
        else i += num_chars;
    }
}

/**
 * Draw left or right on the grid depending on the variables passed
 */
void drawLeftRight(char grid[][50], bool pen_status, bool bold_status, string spaces, char direction, int i, int &j)
{
    // Get the number or spaces to move
    int num_chars = atoi(spaces.c_str());

    // Set up variables to hold whether the moving index value should be negative
    bool negative;

    // If the node's direction is West
    if(direction == 'W')
    {
        // The index movement is negative
        negative = true;
    }
    // If the node's direction is East
    else
    {
        // The index movement is positive
        negative = false;
    }

    // If the pen is up
    if(!pen_status)
    {
        // Move index depending on if the index movement is negative
        if(negative)
            j -= num_chars;
        else j += num_chars;
    }
    // If the pen is down
    else
    {
        // Loop through integers counting to the number or spaces to move
        for(int k = 1; k <= num_chars; k++)
        {
            // If the pen is bold
            if(bold_status)
            {
                // Write bold character depending on if the index movement is negative
                if(negative)
                    grid[i][j-k] = '#';
                else grid[i][j+k] = '#';
            }
            // If the pen is not bold
            else
            {
                // If the index movement is negative
                if(negative)
                {
                    // If the index writing to is not bold, write a non-bold character
                    if(grid[i][j-k] != '#')
                        grid[i][j-k] = '*';
                }
                // If the index movement is positive and the index writing to is not bold
                else if(grid[i][j+k] != '#')
                {
                    grid[i][j+k] = '*';
                }
            }
        }
        // Increment/decrement the index depending on if the index movement is negative
        if(negative)
            j -= num_chars;
        else j += num_chars;
    }
}

/**
 * Write the drawing to the paint file
 */
void writeToOutput(fstream& out, char grid[][50])
{
    // Iterate through the array on first dimension
    for(int i = 0; i < 50; i++)
    {
        // Iterate through array on second dimension
        for(int j = 0; j < 50; j++)
        {
            // Write character at the current index to the paint file
            out << grid[i][j];
        }

        // Create a new line if it is not the last line
        if(i + 1 < 50)
            out << '\n';
    }
}

/**
 * Recursively write the sorted and edited commands to the commands file
 */
void writeToInput(fstream& out, node* h)
{
    // If the list is over, return
    if(!h)
        return;

    // Write the first two arguments in the command
    out << h->sort << " " << h->command;

    // If there is a direction, there is also a number of spaces held. Print both
    if(h->direction)
    {
        out << ", " << h->direction << ", " << h->spaces;
    }

    // If there is a next node, create a new line
    if(h->next)
        out << '\n';

    // Call itself with the next node
    writeToInput(out, h->next);
}

/**
 * Delete the linked list from memory
 */
void deleteLinkedList(node* &h)
{
    // While there is a node in the linked list
    while(h)
    {
        // Create a pointer to the node h is pointing at
        node* p1 = h;

        // Move the head pointer to the next node
        h = h->next;

        // Delete the node where the created pointer is pointing to
        delete p1;
    }
}
