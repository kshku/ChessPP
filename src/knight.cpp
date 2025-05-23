#include "knight.h"

#include "colors.h"

Knight::Knight() : Piece(0, 0, Color::NONE) {
}

Knight::Knight(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
    // m_imageSurface = SDL_LoadBMP(
    //     color == Color::WHITE ? "../assets/Wpawn1.bmp" :
    //     "../assets/Bpawn1.bmp");
}

Knight::~Knight() {
}

void Knight::draw(SDL_Renderer* m_renderer) {
    SDL_Rect rect;
    rect.x = xPos;
    rect.y = yPos;
    rect.h = 75;
    rect.w = 75;
    // SDL_BlitSurface(m_imageSurface, NULL, m_windowSurface, &rect);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void Knight::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface =
        SDL_LoadBMP((color == Color::WHITE ? "../assets/Wknight.bmp"
                                           : "../assets/Bknight.bmp"));
    if (!m_imageSurface) printf("Image not loaded!!\n");
    texture = SDL_CreateTextureFromSurface(m_renderer, m_imageSurface);
    SDL_FreeSurface(m_imageSurface);
}

static bool simulateMove(int BS[], char turn, int sourcePos, int destPos,
                         int kingPos) {
    bool isValid;
    int state = BS[destPos];
    BS[destPos] = BS[sourcePos];
    BS[sourcePos] = State::NONE;
    isValid = !Fen(BS).isCheck(kingPos, turn);
    // Undo the move to reuse the virtual board BS
    BS[sourcePos] = BS[destPos];
    BS[destPos] = state;

    return isValid;
}

void Knight::getValidMoves(int boardState[], int index) {
    int kingPos;

    int possibleMoves[8] = {index - 15, index - 6, index + 10, index + 17,
                            index + 15, index + 6, index - 10, index - 17};

    switch (index % 8) {
        case 0:
            possibleMoves[4] = possibleMoves[5] = possibleMoves[6] =
                possibleMoves[7] = -1;

            break;

        case 1:
            possibleMoves[5] = possibleMoves[6] = -1;
            break;

        case 6:
            possibleMoves[1] = possibleMoves[2] = -1;
            break;

        case 7:

            possibleMoves[0] = possibleMoves[1] = possibleMoves[2] =
                possibleMoves[3] = -1;

            break;
        default:
            break;
    }
    if (this->color == Color::WHITE) {
        for (int i = 0; i < 64; i++) {
            if (boardState[i] == State::WKING) kingPos = i;
        }
        for (int i = 0; i < 8; i++) {
            int idx = possibleMoves[i];
            if (idx >= 0 && idx <= 63) {
                if (boardState[idx] == State::NONE
                    || (boardState[idx] <= State::BPAWN
                        && boardState[idx] >= State::BROOK)) {
                    if (simulateMove(boardState, 'w', index, idx, kingPos))
                        boardState[idx] |= State::VALID;
                }
            }
        }
    } else {
        for (int i = 0; i < 64; i++) {
            if (boardState[i] == State::BKING) kingPos = i;
        }
        for (int i = 0; i < 8; i++) {
            int idx = possibleMoves[i];
            if (idx >= 0 && idx <= 63) {
                if (boardState[idx] == State::NONE
                    || (boardState[idx] <= State::WPAWN
                        && boardState[idx] >= State::WROOK)) {
                    if (simulateMove(boardState, 'b', index, idx, kingPos))
                        boardState[idx] |= State::VALID;
                }
            }
        }
    }
}
