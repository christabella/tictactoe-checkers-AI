#include "player.hpp"
#include "constants.hpp"
#include <cstdlib>

// my player is playing X

#define BOARD_SIZE 4
#define DEPTH 2

namespace TICTACTOE
{
struct Heuristic {
    int rows, column, diagonals;
};
GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    opponent = pState.getNextPlayer();

    me = opponent ^ (CELL_X | CELL_O); // bitwise XOR with (11)

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    int len = lNextStates.size();

    if (len == 0) return GameState(pState, Move());
    if (len == 1) return lNextStates[0];

    int highest = -99999;
    int move_index;
    for (int i = 0; i < len; ++i) {
        int res = minimax(lNextStates[i], me, DEPTH);
        std::cerr << res << " wasres minimax\n";
        if (res > highest) {
            move_index = i;
            highest = res;
            std::cerr << i << " was i\n";
        }
    }
    std::cerr << "END OF FOR\n";
    std::cerr << move_index << "move index\n";
    return lNextStates[move_index];
}

const int wins[10][4] = {
  { 0, 1, 2, 3 },
  { 4, 5, 6, 7 },
  { 8, 9, 10, 11 },
  { 12, 13, 14, 15 },
  { 0, 4, 8, 12 },
  { 1, 5, 9, 13 },
  { 2, 6, 10, 14 },
  { 3, 7, 11, 15 },
  { 0, 5, 10, 15 },
  { 3, 6, 9, 12 }
};
const int Heuristic_Array[5][5] = {
  {      0,   -10,  -100, -1000, -10000 },
  {     10,     0,     0,     0, 0      },
  {    100,     0,     0,     0, 0      },
  {   1000,     0,     0,     0, 0      },
  {  10000,     0,     0,     0, 0      }
};

int Player::heuristic(const GameState &state) {

    int mine, opponents, i, j, value = 0;

    if (state.isEOG()) {
        if ((state.isOWin() && me == CELL_O) || (state.isXWin() && me == CELL_X)) {
            return 100000;
        } else if ((state.isOWin() && opponent == CELL_O) || (state.isXWin() && opponent == CELL_X)) {
            return -100000;
        } else {
            return 0;
        }
    }
    
    for (i = 0; i < 10; i++)  {
        mine = opponents = 0;
        for (j = 0; j < 4; j++)  {
            int piece = state.at(wins[i][j]);
            if (piece&me)
                mine++;
            else if (piece&opponent)
                opponents++;
      }
      value += Heuristic_Array[mine][opponents];
    }
    return value;

}

int Player::minimax(const GameState &state, uint8_t player, int depth) {

    std::vector<GameState> lNextStates;
    state.findPossibleMoves(lNextStates);
    int len = lNextStates.size();

    if (state.isEOG() || depth == 0) {
        return heuristic(state);
    }
    int bestPossible;

    if (player == me) {
        int bestPossible = -99999;
        for (int i = 0; i < len; ++i) {
            int res = minimax(lNextStates[i], opponent, depth-1);
            if (res > bestPossible)
                bestPossible = res;
        }
        return bestPossible;
    } else if (player == opponent) {
        int bestPossible = 99999;
        for (int i = 0; i < len; ++i) {
            int res = minimax(lNextStates[i], me, depth-1);
            if (res < bestPossible)
                bestPossible = res;
        }
        return bestPossible;
    }

    return bestPossible;

}

/*namespace TICTACTOE*/ }
