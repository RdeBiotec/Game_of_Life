# A C++11 and OpenGl implementation of Conway's Game of Life
  * Version: 1.0
  * Date: 04-Feb-2018
  
The present programme is an implementation in C++ and OpenGl of the denominated Game of Life, designed by the British mathematician John Horton Conway. It is a zero-player game; its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves or by creating patterns with particular properties. The universe of the Game of Life is a two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, alive or dead, or "populated" or "unpopulated". Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.

At each step in time, the following rules are applied:
      1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
      2. Any live cell with two or three live neighbours lives on to the next generation.
      3. Any live cell with more than three live neighbours dies, as if by overpopulation.
      4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      
This implementation uses POO, although it is not required, because it is a very interesting feature of C++, along other languages.  The user defines initial parameters about the grid (game board) and cells, and then using a graphical interface places the live cells and observes its evolution.

![An example](https://github.com/RdeBiotec/Game_of_Life/raw/master/GAME.gif)

## Getting Started

The program has been designed for LINUX systems. For others, it has to be adapted. Previous execution, please install all required libraries, specially OpenGl and GLUT.

It is easy to use. You just have to compile, build, execute and follow the instructions (grid size, number of starting alive cells and number of cycles)!

## Authors

* **Rodrigo García Valiente** - *Initial work* - [RdeBiotec](https://github.com/RdeBiotec)

## License

This project is licensed under the GNU General Public License v.3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

* You!


Have fun! :D
