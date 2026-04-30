#include "AbaloneBot.h"
#include <set>
#include <sstream>
#include <cmath>

// Board encoding: 0=empty, 1=white, -1=black, 2=forbidden

// The 6 hex directions expressed as (row-delta, transformed-col-delta).
// The "transformed column" of original [r,c] is: r%2==0 ? 2c : 2c+1.
// These six directions cover all six neighbours on the hex grid.
static const int T_DIRS[6][2] = {{1,1},{0,2},{1,-1},{-1,1},{0,-2},{-1,-1}};

struct Pos { int r, c; };

// Transformed column for original position [r, c]
static int tCol(int r, int c) { return r % 2 == 0 ? 2*c : 2*c + 1; }

// Step from original [r,c] by one hex direction (dr, dtc) in transformed space.
// Returns the resulting original-coordinate position.
static Pos tStep(Pos p, int dr, int dtc) {
    int tc = tCol(p.r, p.c) + dtc;
    if (tc < 0) return {-1, -1};
    return {p.r + dr, tc / 2};
}

static bool onBoard(const vector<vector<int>>& gs, Pos p) {
    int n = gs.size();
    if (p.r < 0 || p.r >= n || p.c < 0 || p.c >= (int)gs[0].size()) return false;
    return gs[p.r][p.c] != 2; // 2 = forbidden cell
}

static int pieceAt(const vector<vector<int>>& gs, Pos p) { return gs[p.r][p.c]; }

static string moveStr(Pos p1, Pos p2, Pos tgt) {
    return to_string(p1.r) + " " + to_string(p1.c) + " " +
           to_string(p2.r) + " " + to_string(p2.c) + " " +
           to_string(tgt.r) + " " + to_string(tgt.c);
}

// Generate all valid moves for a unit that starts at p1 and has the given
// length along the (udr, udtc) direction.  p2 is pre-computed as the last
// marble of the unit.  Results are inserted into out (a set, so duplicates
// are dropped automatically).
static void collectUnitMoves(const vector<vector<int>>& gs,
                              Pos p1, Pos p2, int len,
                              int udr, int udtc,
                              int player, set<string>& out) {
    int opp = -player;

    for (int d = 0; d < 6; d++) {
        int dr = T_DIRS[d][0], dtc = T_DIRS[d][1];
        bool inline_fwd  = (len > 1) && dr ==  udr && dtc ==  udtc;
        bool inline_bwd  = (len > 1) && dr == -udr && dtc == -udtc;

        if (len == 1 || inline_fwd || inline_bwd) {
            // ---- inline (or single-marble) move ----
            Pos target = tStep(p1, dr, dtc); // where p1 ends up
            if (!onBoard(gs, target)) continue;

            // front marble in the direction of movement
            Pos front = inline_bwd ? p1 : p2;
            Pos posT  = tStep(front, dr, dtc); // first cell the unit advances into

            // Walk forward from posT counting opponent marbles
            int numOpp = 0;
            bool valid = false;
            Pos cur = posT;
            while (true) {
                if (!onBoard(gs, cur)) {
                    valid = (numOpp > 0); // push opponents off board
                    break;
                }
                int pc = pieceAt(gs, cur);
                if (pc == opp) {
                    numOpp++;
                    if (numOpp >= len) { valid = false; break; } // can't outpush
                    cur = tStep(cur, dr, dtc);
                } else if (pc == 0) {
                    valid = true; // slide or push into empty
                    break;
                } else {
                    valid = false; // own piece or forbidden blocks
                    break;
                }
            }
            if (valid) out.insert(moveStr(p1, p2, target));

        } else {
            // ---- broadside move ----
            bool valid = true;
            Pos cur = p1;
            for (int i = 0; i < len; i++) {
                Pos nxt = tStep(cur, dr, dtc);
                if (!onBoard(gs, nxt) || pieceAt(gs, nxt) != 0) { valid = false; break; }
                if (i < len - 1) cur = tStep(cur, udr, udtc);
            }
            if (valid) out.insert(moveStr(p1, p2, tStep(p1, dr, dtc)));
        }
    }
}

/**************** AbaloneBot public interface ****************/

AbaloneBot::AbaloneBot() {
    this->setMinUtility(-200.0f);
    this->setMaxUtility(200.0f);
    this->setInitialDepthForIDS(2);
    this->gridSize = 9;
    this->edgeSize = 5;
}

void AbaloneBot::setGameState(vector<vector<int>> gs, int gridSz) {
    this->gameState = gs;
    this->gridSize  = gridSz;
    this->edgeSize  = (gridSz + 1) / 2;
}

void AbaloneBot::setForbiddenStates(vector<vector<vector<int>>> fs) {
    this->forbiddenStates = fs;
}

AbstractBot* AbaloneBot::cloneGameState() {
    AbaloneBot* copy = new AbaloneBot();
    copy->gameState = this->gameState;
    copy->gridSize  = this->gridSize;
    copy->edgeSize  = this->edgeSize;
    return copy;
}

vector<string> AbaloneBot::getValidMoves(bool isBlackTurn) {
    int player = isBlackTurn ? -1 : 1;
    set<string> movesSet;

    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            if (gameState[r][c] != player) continue;
            Pos p1 = {r, c};

            // Single-marble moves (unit direction is irrelevant, len=1)
            collectUnitMoves(gameState, p1, p1, 1, 0, 0, player, movesSet);

            // Multi-marble units extending from p1 in each direction
            for (int d = 0; d < 6; d++) {
                int udr = T_DIRS[d][0], udtc = T_DIRS[d][1];
                Pos p2 = tStep(p1, udr, udtc);
                if (!onBoard(gameState, p2) || gameState[p2.r][p2.c] != player) continue;

                collectUnitMoves(gameState, p1, p2, 2, udr, udtc, player, movesSet);

                Pos p3 = tStep(p2, udr, udtc);
                if (!onBoard(gameState, p3) || gameState[p3.r][p3.c] != player) continue;

                collectUnitMoves(gameState, p1, p3, 3, udr, udtc, player, movesSet);
            }
        }
    }

    return vector<string>(movesSet.begin(), movesSet.end());
}

void AbaloneBot::executeMove(string mv) {
    istringstream ss(mv);
    Pos p1, p2, tgt;
    ss >> p1.r >> p1.c >> p2.r >> p2.c >> tgt.r >> tgt.c;

    // Move direction in transformed space
    int dr  = tgt.r - p1.r;
    int dtc = tCol(tgt.r, tgt.c) - tCol(p1.r, p1.c);

    // Unit direction and length
    int udr = 0, udtc = 0, len = 1;
    if (p1.r != p2.r || p1.c != p2.c) {
        int dr2  = p2.r - p1.r;
        int dtc2 = tCol(p2.r, p2.c) - tCol(p1.r, p1.c);
        // Each hex step has L1-transformed-distance of 2, so:
        len  = (abs(dr2) + abs(dtc2)) / 2 + 1;
        udr  = dr2  / (len - 1);
        udtc = dtc2 / (len - 1);
    }

    bool inline_bwd = (len > 1) && dr == -udr && dtc == -udtc;
    bool is_inline  = (len > 1) && (( dr ==  udr && dtc ==  udtc) || inline_bwd);
    int player = gameState[p1.r][p1.c];
    int opp    = -player;

    // Collect all unit marble positions
    vector<Pos> unit;
    Pos cur = p1;
    for (int i = 0; i < len; i++) {
        unit.push_back(cur);
        if (i < len - 1) cur = tStep(cur, udr, udtc);
    }

    // Handle opponent push (inline moves only)
    if (len == 1 || is_inline) {
        Pos front = inline_bwd ? p1 : p2;
        Pos posT  = tStep(front, dr, dtc);

        vector<Pos> oppChain;
        Pos walker = posT;
        while (onBoard(gameState, walker) && gameState[walker.r][walker.c] == opp) {
            oppChain.push_back(walker);
            walker = tStep(walker, dr, dtc);
        }

        if (!oppChain.empty()) {
            Pos lastDest = tStep(oppChain.back(), dr, dtc);
            if (onBoard(gameState, lastDest)) {
                // Last opponent slides to lastDest
                gameState[lastDest.r][lastDest.c] = opp;
            }
            // else: last opponent is pushed off the board (simply don't place it)

            // First opponent's cell is freed for our advancing marble.
            // Intermediate cells stay "opp" because each opponent shifts into
            // its predecessor's slot (same value, no explicit change needed).
            gameState[oppChain[0].r][oppChain[0].c] = 0;
        }
    }

    // Shift all unit marbles: clear old positions first, then set new ones
    for (Pos p : unit) gameState[p.r][p.c] = 0;
    for (Pos p : unit) {
        Pos np = tStep(p, dr, dtc);
        if (onBoard(gameState, np)) gameState[np.r][np.c] = player;
    }
}

bool AbaloneBot::isGameOver() {
    int total    = edgeSize * 3 - 1;
    int numWhite = 0, numBlack = 0;
    for (auto& row : gameState)
        for (int p : row) {
            if (p ==  1) numWhite++;
            if (p == -1) numBlack++;
        }
    return (total - numBlack > edgeSize) || (total - numWhite > edgeSize);
}

float AbaloneBot::getUtility() {
    int total    = edgeSize * 3 - 1;
    int numWhite = 0, numBlack = 0;
    int center   = edgeSize - 1;
    float whiteCenterBonus = 0, blackCenterBonus = 0;

    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            int p = gameState[r][c];
            if (p == 1) {
                numWhite++;
                float dist = sqrt((float)((r-center)*(r-center) + (c-center)*(c-center)));
                whiteCenterBonus += (edgeSize - 1) - dist;
            } else if (p == -1) {
                numBlack++;
                float dist = sqrt((float)((r-center)*(r-center) + (c-center)*(c-center)));
                blackCenterBonus += (edgeSize - 1) - dist;
            }
        }
    }

    if (total - numBlack > edgeSize) return  100.0f; // white wins
    if (total - numWhite > edgeSize) return -100.0f; // black wins

    return (numWhite - numBlack) * 10.0f
         + (whiteCenterBonus - blackCenterBonus) * 0.1f;
}

bool AbaloneBot::isBotInForbiddenState(AbstractBot* abstractBot) {
    AbaloneBot* other = dynamic_cast<AbaloneBot*>(abstractBot);
    for (auto& fs : this->forbiddenStates) {
        if (other->gameState == fs) return true;
    }
    return false;
}
