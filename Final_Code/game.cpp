// 
// File: game.cpp 
// 
// 
// 
// @author douglas: Douglas Moyer
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <iostream>
#include "game.h"
#include "SDL2/SDL.h"

using namespace std;

game::game() : player_("Player 1", 400, 300, PLAYER_HEALTH, 0, TILE_SIZE), print_(&quests_, &player_, &current_quest_) {
    current_quest_ = 0;
}
    
void game::run() {
    quest nexus(true, &player_);
    quests_.push_back(nexus);
    current_quest_ = 0;
    printer print(&quests_, &player_, &current_quest_);
    print.init();
    print.load_media();
    bool running = true;
    SDL_Event e;
    while(running) {
        while (print.poll_event(&e) != 0) {
            if(e.type == SDL_QUIT)
                running = false;
            handle_event(e);
        }
        player_.update();
        print.update();
    }
    print.close();
}

void game::handle_event(SDL_Event e) {
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: player_.move(dir_up); break;
            case SDLK_DOWN: player_.move(dir_down); break;
            case SDLK_LEFT: player_.move(dir_left); break;
            case SDLK_RIGHT: player_.move(dir_right); break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: player_.stop_move(dir_up); break;
            case SDLK_DOWN: player_.stop_move(dir_down); break;
            case SDLK_LEFT: player_.stop_move(dir_left); break;
            case SDLK_RIGHT: player_.stop_move(dir_right); break;
        }
    }
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
// 
