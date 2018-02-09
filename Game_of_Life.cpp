/****************************************************************
*****************************************************************
****A C++11 and OpenGl implementation of Conway's Game of Life***
*****************************************************************

  * Author: Rodrigo García Valiente
  * E-mail: rodrigo.garcia.valiente@gmail.com
  * Version: 1.0
  * Date: 04-Feb-2018
  * Github: https://github.com/RdeBiotec
*****************************************************************/

/*****************************************************
  * OVERVIEW:
  * The present programme is an implementation in C++ and OpenGl of the denominated Game of Life, a cellular automaton designed by the British mathematician John Horton Conway.
  * It is a zero-player game; its evolution is determined by its initial state, requiring no further input. 
  * One interacts with the Game of Life by creating an initial configuration and observing how it evolves or by creating patterns with particular properties. 
  * The universe of the Game of Life is a two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, 
  * alive or dead, or "populated" or "unpopulated". Every cell interacts with its eight neighbours, 
  * which are the cells that are horizontally, vertically, or diagonally adjacent. 
  * At each step in time, the following rules are applied:
		1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
		2. Any live cell with two or three live neighbours lives on to the next generation.
		3. Any live cell with more than three live neighbours dies, as if by overpopulation.
		4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

  * This implementation uses OOP, although it is not required, because it is a very interesting feature of C++, along other languages.
  * The user defines initial parameters about the grid (game board) and cells, and then using a graphical interface places the live cells and observes its evolution.
  * 
  * Have fun!

  * NOTES:
  1. Tested on Ubuntu 16.04.03. Headers (mainly GLUT) should be adapted for Windows and MacOS.
  2. Remember to build using "g++ -Wall  -o "%e" "%f"  -std=c++11 -lglut -lGL -lGLU" or equivalent
  3. Global variables are not optimum, but they have to be created to work with them in the GenerateBoard function
  4. The game can be done much more simpler, but I wanted to use some OOP and make it more visual.
  5. Possible future work:
  * 	-This Game of Life is too simple. Create new rules (stem cells, environment, etc.)
  * 	-Add textures, create a more eye-candy interface.
  * 	-Improve mouse-clicking for certain window sizes and ratios
  * 	-Save each cycle on a .txt, with it, create a heatmap and study the evolution of cell number depending on the starting positions.
  * 	-Improve reshaping of the window (glutReshapeFunc). Currently it works mediocrely when the window is maximized.
  * 	-Detect patterns (gliders, blinkers, etc.). I left some ideas in the code.
  * 	-Create GIFs of the system.
  * 	-Create a graphical interactive menu.
  * 	-Give more possibilities related to the grid size.
******************************************************/

//////////////////////////////////////////////////////
//0. Loading libraries
//////////////////////////////////////////////////////
#ifdef __linux__ 
   #include <GL/glut.h>
#else
	#error "Platform not supported, you have to modify the GLUT statement to it."
#endif
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;



//////////////////////////////////////////////////////
//1. Defining Cell class and global variables
//////////////////////////////////////////////////////
class Cell {
	public:
		int xCoord; //X-axis coordinates
		int yCoord; //Y-axis coordinates
		bool alive;  //Alive? (true/false)
		Cell update(Cell cell_to_update); //This function is articulated regarding to future implementations, where it would include more functions apart from getState, such as getClass
	private:
		bool getState(int xCoord , int yCoord , bool alive);  // It works well, but it will be changed in a future.
		//char getClass( char state , char xCoord , char yCoord , bool alive);  // This is for future implementations.
		//int born_in_cycle; // This is for future implementations.
		//vector<int> born_history; // This is for future implementations.
};

//These variables must be global to be used in the GenerateBoard function
int w=900; //Set display-window width. It will be changed in the GenerateBoard function if the user re-scales the window.
int h=900; //Set display-window height. It will be changed in the GenerateBoard function if the user re-scales the window.
int n=101; //Cell width for the game board. It is just a container, it will be later specified by the user.
int m=101; //Cell height for the game board. It is just a container, it will will be later specified by the user.
int max_cycles=0; //Number of maximum cycles to run the game. It is just a container, it will will be later specified by the user.
int current_cycles=0; //Number of current cycles executed in-game
int starting_alive=0; //Number of starting alive cells to run the game. It is just a container, it will will be later specified by the user.
int number_alive_clicked=0; //Number of starting alive cells that the user has placed over the game board.
vector<Cell> Cells; //We initialize a vector of Cell elements, which will be later filled with m x n elements after the user specifies n and m.
bool end_game = false; //To stop creating the board game when enough cycles are met or all cells are dead 

//////////////////////////////////////////////////////
//2. Defining functions
//////////////////////////////////////////////////////

//Cell::update function. Public.
//This function employs private functions (currently only getState) and gives an update of their output.
Cell Cell::update (Cell cell_to_update) {
    cell_to_update.alive =  Cell::getState(cell_to_update.xCoord, cell_to_update.yCoord, cell_to_update.alive);
    
    return cell_to_update;
}


//Cell::getState function. Private.
//This function updates the live/dead status of a cell according to its environment and its own live/dead status.
bool Cell::getState( int xCoord , int yCoord , bool alive ) {
    int live_neighbours = 0;
	for ( int i = xCoord - 1; i <= xCoord + 1; i++ ) {
		for ( int j = yCoord - 1; j <= yCoord + 1; j++ ) {
			if (i != xCoord or j != yCoord) {
				if (i >= 0 and i < ::n and j >= 0 and j < ::m ) {				
					if ( ::Cells[i*m + j].alive) {
						live_neighbours++;
					}
				}
			}
		}
	}
	
    if ( alive ) {
		return ( live_neighbours >= 2 && live_neighbours <= 3 ) ? true : false;
    } else {
       return ( live_neighbours == 3 ) ? true : false;
    }
}

//init function.
//This function includes GLUT parameters to start the visualization.
void init (void)
{
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h= glutGet(GLUT_WINDOW_HEIGHT);
  glClearColor (0,0,0,0.0); //I like more Wisteria color (0.56,0.27,0.68,0.0), but here its better pure black.
  glMatrixMode (GL_PROJECTION);
  glClear (GL_COLOR_BUFFER_BIT);
  gluOrtho2D (0,w,0,h); //It uses the window width and height
}

//GenerateBoard function.
//This function is essential. It calculate the grid of cells according to their previous status,
// and gives instructions to GLUT and OpenGl to generate it.
void GenerateBoard() {

  //We update the board's width and height in according to the window size
  ::h=glutGet(GLUT_WINDOW_HEIGHT);
  ::w=glutGet(GLUT_WINDOW_WIDTH);

  glClear (GL_COLOR_BUFFER_BIT);
  int sw = w/n, sh = h/m; //square width and height respectively
  //for each width and height draw a rectangle with a specific color
  int current= 0;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j) {
      //oscillate the color per square of the board
      if(::Cells[current].alive) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      };

      //draw a rectangle in the ith row and jth column
      glRecti(i*sw, j*sh, (i+1)*sw, (j+1)*sh);
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      glColor3f(1, 0.26, 0.26);
      glRecti(i*sw, j*sh, (i+1)*sw, (j+1)*sh);
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      current++;
    };
  }
   
  glFlush();	  //We refresh here the screen so it will not feel clunky with the nanosleep
  
  //When all starting live cells are selected, this part starts. 
  if (starting_alive == number_alive_clicked and !end_game) {
	  int alive = 0;
	  vector<Cell> vector_of_survival; //We create a vector to drop the updated parameters of the grid, so they will not interfere with the calculations. 
	  
	  //If there are no alive cells or all cycles requested by the user are done, the game is over.
	  for (unsigned int i =0; i < ::Cells.size(); ++i) {
		 if (::Cells[i].alive) {
			 alive++;
		 };
		 
	  };
	  if (alive == 0) {
		  end_game = true;
		  cout << "The game ended at " << current_cycles << " cycles. There are no alive cells." << endl;
	  } else if (max_cycles != 0 and current_cycles == max_cycles) {
		  end_game = true;
		  cout << "The game ended at " << current_cycles << " cycles. There remains " << alive << " alive cells." << endl;

	  } else {
		  //If not, we start calculating the next generation of cells.
	  	  for(int i = 0; i < n; ++i) {
			for(int j = 0; j < m; ++j) {
				  vector_of_survival.push_back(Cells[i*m + j].update(Cells[i*m + j]));
			};
		  };
		  for(int i = 0; i < n; ++i) {
			for(int j = 0; j < m; ++j) {
				  ::Cells[i*m + j]=vector_of_survival[i*m + j];
			};
		  };
		  ::current_cycles++;
	   };
	  
	  
  //We delay a bit the next iteration of the loop.  
  struct timespec wait;
  wait.tv_sec = 970000 / (1000 * 1000);
  wait.tv_nsec = (970000 % (1000 * 1000)) * 1000;
  nanosleep(&wait, NULL);
  glutPostRedisplay();
  
  };
}

///i*m + j   

//mouse_click function.
//This function calculates which grid is clicked by the user according to its coordinates
void mouse_click(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN  && starting_alive > number_alive_clicked) 
	{ 
		if (x <= w && (h-y) <= h)  
		{
			if (::Cells[(x*n/w) * m + (h-y)*m/(h) ].alive == false) { //For certain ratios it is not a perfect solution, as it works in int and not in floats.
				::Cells[(x*n/w) * m + (h-y)*m/(h) ].alive=true;
				number_alive_clicked += 1;
				cout << "Number of remaining cells to click on and make alive: " << starting_alive - number_alive_clicked << endl;
			}
		}

	} glutPostRedisplay();
   
}




//////////////////////////////////////////////////////
//3. Main
//////////////////////////////////////////////////////
int main (int argc, char** argv)
{ 
  //Requesting parameters
  cout <<"*****************************************************************"<< endl;
  cout <<"****A C++11 and OpenGl implementation of Conway's Game of Life***"<< endl;
  cout <<"*****************************************************************"<< endl << endl;
  cout << "Before starting, please introduce some parameters" << endl;
  
  while ( ::n > 100 ) {
	  cout << "Grid width (less than or equal to 100): ";
	  cin >> ::n;  
  }
  
  while ( ::m > 100 ) {
	  cout << "Grid height (less than or equal to 100): ";
	  cin >> ::m;  
  }	  

  while ( ::starting_alive ==0 or ::starting_alive > ::n*::m ) {
	  cout << "Number of starting alive cells (less than or equal to " << ::n*::m << "): ";
	  cin >> ::starting_alive;  
  }	  

  cout << "Number of maximum cycles to run in-game (if 0, infinite until all cells are dead): ";
  cin >> ::max_cycles; 

  cout << "Good. Now an additional window will open. You will have to click "<< ::starting_alive << " dead cells to make them alive. The game will start when you press <ENTER>, and will end when the cycles you indicated are over or when all cells are dead." << endl;
  cin.ignore();
  cin.get();
  int total= 0;
  for(int i = 0; i < ::n; ++i) {
    for(int j = 0; j < ::m; ++j) {
      ::Cells.push_back(Cell());
      ::Cells[total].xCoord = i;
      ::Cells[total].yCoord = j;
      ::Cells[total].alive = false;
      total +=1;
    };
  };
  
  
  
  //Loading OpenGL and GLUT
  glutInit(&argc,argv); //Initialize GLUT.
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Display mode.
  glutInitWindowPosition (0,0); //Display-window position: top-left
  glutInitWindowSize (::w,::h); //Display-window width and height as set in the global parameters.
  glutCreateWindow ("Game of Life"); //Opening display window.
  glutMouseFunc(mouse_click); // Function to execute when mouse clicking.
  //glutIdleFunc(GenerateBoard);
  init(); // Execute initialization procedure.
  glutDisplayFunc( GenerateBoard ); //Send graphics to display window.
  glutMainLoop(); //Display loop.

  
  
  return 0;
}


