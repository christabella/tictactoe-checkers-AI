#include "player.hpp"
#include <cstdlib>
#include <algorithm> 

#define BOARD_SIZE 4
#define DEPTH 1
#define NUM_WINS 68
#define BUFFER 0.1

namespace TICTACTOE3D
{
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
        if (pDue.now() > pDue - BUFFER)
            break;
        int val = minimax(lNextStates[i], me, DEPTH, -99999, 99999);
        std::cerr << val << " was minimax\n";
        if (val > highest) {
            move_index = i;
            highest = val;
            std::cerr << i << " was i\n";
        }
    }
    std::cerr << "END OF FOR\n";
    std::cerr << move_index << "move index\n";
    return lNextStates[move_index];
}

const int heuristic_weights[5][5] = {
  {      0,   -1,  -10, -100, -1000 },
  {     1,     0,     0,     0, 0      },
  {    10,     0,     0,     0, 0      },
  {   100,     0,     0,     0, 0      },
  {  1000,     0,     0,     0, 0      }
};

int Player::heuristic(const GameState &state) {

    int mine, opponents, i, j, value = 0;

    if (state.isEOG()) {
        if ((state.isOWin() && me == CELL_O) || (state.isXWin() && me == CELL_X)) {
            return 1000;
        } else if ((state.isOWin() && opponent == CELL_O) || (state.isXWin() && opponent == CELL_X)) {
            return -1000;
        } else {
            return 0;
        }
    }
    
    for (i = 0; i < NUM_WINS; i++)  {
        mine = opponents = 0;
        for (j = 0; j < 4; j++)  {
            int piece = state.at(wins[i][j]);
            if (piece&me)
                mine++;
            else if (piece&opponent)
                opponents++;
      }
      value += heuristic_weights[mine][opponents];
    }
    return value;

}

int Player::minimax(const GameState &state, uint8_t player, int depth, int alpha, int beta) {
    /* state  : the current state we are analyzing
     * alpha  : the current best value achievable by A
     * beta   : the current best value acheivable by B
     * player : the current player
     * returns the minimax value of the state
     */


    if (state.isEOG() || depth == 0) {
        return heuristic(state);
    }
    
    std::vector<GameState> lNextStates;
    state.findPossibleMoves(lNextStates);
    int len = lNextStates.size();
    int val, i;

    if (player == me) {
        val = -99999;
        for (i = 0; i < len; ++i) {
            val = std::max(minimax(lNextStates[i], opponent, depth-1, alpha, beta), val);
            alpha = std::max(alpha, val);
            if (beta <= alpha)
                break; // beta prune
        }
    } else if (player == opponent) {
        val = 99999;
        for (i = 0; i < len; ++i) {
            val = std::max(minimax(lNextStates[i], me, depth-1, alpha, beta), val);
            beta = std::min(beta, val);
            if (beta <= alpha)
                break; // alpha prune            
        }
    }

    return val;

}

/*namespace TICTACTOE*/ }
