// 
// File: character.cpp 
// 
// 
// 
// @author Douglas Moyer and Alex Meddin
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

#include "define.h"
#include "character.h"
#include "math.h"
#include "coordinate.h"
using namespace std;

character::character(string initName, int initX, int initY, int initHealth, int initXP, int initSpeed, vector<quest> *quests, int *current_quest, enum character_type type) {
    //Initializes variables
    time_passed_ = 0;
    animation_state_ = 0;
    move_ = true;
    name_ = initName;
    health_ = initHealth;
    xp_ = initXP;
    x_ = initX;
    y_ = initY;
    speed = initSpeed;
    x_vel_ = 0;
    y_vel_ = 0;
    type_ = type;

    //Calculate damageMod and attackMod
    damageMod = log10((float)xp_);
    attackMod = log((float)xp_);

    quests_ = quests;
    current_quest_ = current_quest;

    facing_ = dir_down;
    if(type == character_trapdoor) facing_ = dir_right;
    exists_ = true;
    switch(type) {
        case character_player: size_ = PLAYER_SIZE; break;
        case character_fireball: size_ = FIREBALL_SIZE; break;
        case character_monster: size_ = ZOMBIE_SIZE; break;
        case character_panda: size_ = PANDA_SIZE; break;
        case character_trapdoor: size_ = TRAPDOOR_SIZE; break; 
    }
    if(type_ == character_panda)
        move_degrees(rand() % 360);
}

void character::random_pos() {
    //if(type_ != character_trapdoor) {
        coordinate random = (*quests_)[*current_quest_].curr_floor()->random_space();
        x_ = random.j() * TILE_SIZE;
        y_ = random.i() * TILE_SIZE;
    /*} else {
        do {
            coordinate random = (*quests_)[*current_quest_].curr_floor()->random_space();
        } while ((*quests_)[*current_quest_].curr_floor()->terrain_at(random.i()+1, random.j()) ||(*quests_)[*current_quest_].curr_floor()->terrain_at(random.i(), random.j()+1));
        x_ = random.j() * TILE_SIZE;
        y_ = random.i() * TILE_SIZE;
    }*/
}

void character::changeName(string newName) {
    name_ = newName;
}

int character::degrees() { return move_direction; }

void character::stop() { x_vel_ = 0; y_vel_ = 0;}

bool character::is_moving() {return (x_vel_ == 0 && y_vel_ == 0);}

void character::panda_hit_wall() {
    if(type_ == character_panda) {
        move_degrees(rand() % 360);
    }
}

void character::update(Uint32 dt) {
    if(type_ == character_player && (time_passed_ += dt) > 1000/12) { //player animation
        animation_state_ = (animation_state_ + 1) % 4;
        time_passed_ = 0;
    }
	if (type_ == character_fireball && (time_passed_ += dt) > 1000 / 12) { //fireball animation
		animation_state_ = (animation_state_ + 1) % 5;
		time_passed_ = 0;
	}
	if ((type_ == character_panda || type_ == character_monster) && (time_passed_ += dt) > 1000 / 6) { //fireball animation
		animation_state_ = (animation_state_ + 1) % 3;
		time_passed_ = 0;
	}
    if (type_ == character_trapdoor) {
        if(time_passed_ == 0)
        cout << "x: " << x_ / TILE_SIZE << " y: " << y_ / TILE_SIZE << "\n";
    }
	if (x_vel_ == 0 && y_vel_ == 0) animation_state_ = 0;

    //TODO: update GUI for character location
    x_ += x_vel_ * (dt/1000.f);
    //x_ += x_vel_;
    // i = x left
    int i = (x_ - (x_ % TILE_SIZE)) / TILE_SIZE;
    // j = y top
    int j = (y_ - (y_ % TILE_SIZE)) / TILE_SIZE;
    // k = x right
    int k = ((x_ + size_) - ((x_ + size_) % TILE_SIZE)) / TILE_SIZE;
    // l = y bot
    int l = ((y_ + size_) - ((y_ + size_) % TILE_SIZE)) / TILE_SIZE;
    if(x_vel_ < 0) {
        if((*quests_)[*current_quest_].curr_floor()->terrain_at(j, i)) {
            x_ = TILE_SIZE + (x_ - (x_ % TILE_SIZE)) + 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
        else if((*quests_)[*current_quest_].curr_floor()->terrain_at(l, i)) {
            x_ = TILE_SIZE + (x_ - (x_ % TILE_SIZE)) + 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
    }
    else if(x_vel_ > 0) {
        if((*quests_)[*current_quest_].curr_floor()->terrain_at(j, k)) {
            x_ = (x_) - ((x_ + size_) % TILE_SIZE) - 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
        else if((*quests_)[*current_quest_].curr_floor()->terrain_at(l, k)) {
            x_ = (x_) - ((x_ + size_) % TILE_SIZE) - 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
    }
    y_ += y_vel_ * (dt/1000.f);
    //y_ += y_vel_;
    // i = x left
    i = (x_ - (x_ % TILE_SIZE)) / TILE_SIZE;
    // j = y top
    j = (y_ - (y_ % TILE_SIZE)) / TILE_SIZE;
    // k = x right
    k = ((x_ + size_) - ((x_ + size_) % TILE_SIZE)) / TILE_SIZE;
    // l = y bot
    l = ((y_ + size_) - ((y_ + size_) % TILE_SIZE)) / TILE_SIZE;
    if(y_vel_ < 0) {
        if((*quests_)[*current_quest_].curr_floor()->terrain_at(j, i)) {
            y_ = TILE_SIZE + (y_ - (y_ % TILE_SIZE)) + 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
        else if((*quests_)[*current_quest_].curr_floor()->terrain_at(j, k)) {
            y_ = TILE_SIZE + (y_ - (y_ % TILE_SIZE)) + 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
    }
    else if(y_vel_ > 0) {
        if((*quests_)[*current_quest_].curr_floor()->terrain_at(l, i)) {
            y_ = (y_) - ((y_ + size_) % TILE_SIZE) - 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
        else if((*quests_)[*current_quest_].curr_floor()->terrain_at(l, k)) {
            y_ = (y_) - ((y_ + size_) % TILE_SIZE) - 1;
            if(type_ == character_fireball) exists_ = false;
            panda_hit_wall();
        }
    }
}

string character::name() {
    return name_;
}

int character::health() {
    return health_;
}

int character::xp() {
    return xp_;
}

int character::x() {
    return x_;
}

int character::y() {
    return y_;
}

void character::addXP(int plusXP) {
    xp_ += plusXP;
}

void character::take_damage(int hitValue) {
    //health_ -= (int)((float)hitValue - damageMod);
    health_ -= hitValue;
    if(health_ <= 0) exists_ = false;
}

int character::giveDamage(int hitValue) {
    return (int)((float)hitValue + attackMod);
}

void character::move(enum direction dir) {
    if(dir == dir_left)  
        x_vel_ = -speed;
    if(dir == dir_up)
        y_vel_ = -speed;
    if(dir == dir_right)
        x_vel_ = speed;
    if(dir == dir_down)
        y_vel_ = speed;
    facing_ = dir;
    move_ = true;
}

void character::move_degrees(int degrees) {
    move_direction = degrees;
    move_ = false;
    x_vel_ = speed * cos((float)degrees * PI/180.0);
    y_vel_ = -speed * sin((float)degrees * PI/180.0);
    if(x_vel_ < 0) x_vel_+=10;
    if(y_vel_ < 0) y_vel_+=10;
    if (x_vel_ < 0 && abs(x_vel_) > abs(y_vel_)) face_direction(dir_left);
    if (x_vel_ > 0 && abs(x_vel_) > abs(y_vel_)) face_direction(dir_right);
    if (y_vel_ < 0 && abs(y_vel_) > abs(x_vel_)) face_direction(dir_up);
    if (y_vel_ > 0 && abs(y_vel_) > abs(x_vel_)) face_direction(dir_down);
}

void character::stop_move(enum direction dir) {
    if(dir == dir_left && x_vel_ == -speed) {
        x_vel_ = 0;
        if(y_vel_ < 0) facing_ = dir_up;
        if(y_vel_ > 0) facing_ = dir_down;
    }
    if(dir == dir_up && y_vel_ == -speed) {
        y_vel_ = 0;
        if(x_vel_ < 0) facing_ = dir_left;
        if(x_vel_ > 0) facing_ = dir_right;
    }
    if(dir == dir_right && x_vel_ == speed) {
        x_vel_ = 0;
        if(y_vel_ < 0) facing_ = dir_up;
        if(y_vel_ > 0) facing_ = dir_down;
    }
    if(dir == dir_down && y_vel_ == speed) {
        y_vel_ = 0;
        if(x_vel_ < 0) facing_ = dir_left;
        if(x_vel_ > 0) facing_ = dir_right;
    }

}

void character::face_direction(enum direction dir) {
    facing_ = dir;
}

enum direction character::get_direction() { return facing_; }

bool character::exists(){ return exists_; }

int character::size() { return size_; }

bool character::collides_with(character b) {
    int leftA = x();
    int leftB = b.x();
    int topA = y();
    int topB = b.y();
    int rightA = x() + size();
    int rightB = b.x() + b.size();
    int bottomA = y() + size();
    int bottomB = b.y() + size();
    if(bottomB < topA) return false;
    if(topB > bottomA) return false;
    if(rightB < leftA) return false;
    if(leftB > rightA) return false;
    return true;
}

void character::unspawn() { exists_ = false; }

int character::animation_state() { return animation_state_; }

void character::set_position(int x, int y) { x_ = x; y_ = y; }
// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
// 
