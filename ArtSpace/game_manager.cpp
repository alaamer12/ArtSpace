// The Self orchestration of the game
#include "game_state.h"
#include "artwork_manager.h"
#include "artwork.h"
#include "camera.h"
#include "config.h"
#include "collision.h"
#include "config.h"
#include "feedback.h"
#include "input.h"
#include "puzzle.h"
#include "room.h"
#include "sound_manager.h"
#include "utility.h"
#include "user.h"


class GameManager {
    private:
        // The current game state
        GameState currentState;
};