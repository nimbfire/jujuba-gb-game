#include <gb/gb.h>
#include <stdio.h>

#include "sprites/Characters.c"
#include "CharacterController.c"

#include "maps/map1.c"
#include "sprites/SpritesPark.c"


// If by collecting this object the map is won.
#define OBJECT_WIN_CONDITION_FLAG    0x01U

// If you can push this object
#define OBJECT_MOVABLE_FLAG      0x02U

// If any character is in this object position, you lose the map.
#define OBJECT_DEATH_FLAG     0x04U

UBYTE running;
UINT8 timer;
UBYTE input_timer;
// UBYTE16 player_map_position;
// UBYTE map
UBYTE map; // Controlls which map we are seeing.

CharacterController bunny;  //1
CharacterController dog1;   //2
CharacterController dog2;   //3
CharacterController cat;    //4
CharacterController horse;  //5
CharacterController *player;

void performantDelay(UINT8 numLoops) {
  UINT8 i;
  for (i = 0; i != numLoops; i++) {
    wait_vbl_done();
  }
}

// Check player controls.
void player_input(CharacterController** c);

// Check player is ded.
int is_ded(CharacterController** c);

// Inits the game base variables, sprites, background.
void init();

// Handles the first map logic.
void map_1();

void set_character_sprite(CharacterController* c);

void move_character(CharacterController* c);

int can_move(INT8 x, INT8 y, UINT8 direction);

void timers() {
  // printf("%d\n", input_timer);
  if (input_timer != 0) {
    input_timer -= 1;
  }
  timer += 1;
}

void main(void)
{
  init();

  while(running) {
    // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;
    // printf("%u %u\n", (unsigned) player->y, (((unsigned) player->y) / 8) -2);

    timers();
    if (map == 1) {
      map_1();
      performantDelay(2);
      map_1_water();
    }
  }

  printf("The end\n");
  while (1) {

  }
}

// Update map 1 water
void map_1_water(){
  UINT16 map_position;

  if (timer % 32 == 0) {
    map_position = 360;
    while (map_position != 0) {
      map_position -= 1;
      // printf("%d\n", map_position);
      switch((UINT16)Map1[map_position]) {
        case (UINT16) 2:
          Map1[map_position] = 3;
          break;
        case (UINT16) 3:
          Map1[map_position] = 2;
          break;

      }
    }
    set_bkg_tiles(0, 0, 20, 18, Map1);

  }
  
}

void map_1() {
  
  if (is_ded(&player)) {
    printf("you ded\n");
  }

  // generate_bunny();
  player_input(&player);

  if (input_timer != 0) {
    if (input_timer == 10) {
      // printf("TROCA\n");
      player->direction = 0;
      switch (player->type) {
        case 1:
          player = &dog1;
          break;
        case 2:
          player = &bunny;
          break;
      }

    }
  }
  move_character(&bunny);
  move_character(&dog1);


}

void init() {

  set_bkg_data(0,11,sprites_park);
  set_bkg_tiles(0, 0, 20, 18, Map1);

  SHOW_BKG;

  input_timer = 0u;
  running = 1u;
  map = 1u;
  timer = 1u;
  player = &bunny;
  DISPLAY_ON;
  SHOW_SPRITES;

  set_sprite_data(0,12,Characters);


  // generate bunny
  bunny.x = 8;
  bunny.y = 40;
  bunny.sprite_1 = 0;
  bunny.sprite_2 = 1;
  bunny.sprite_3 = 2;
  bunny.direction = 0;
  bunny.power_active = 0;
  bunny.type = 1;
  bunny.is_the_active = 1;

  dog1.x = 40;
  dog1.y = 40;
  dog1.sprite_1 = 3;
  dog1.sprite_2 = 4;
  dog1.sprite_3 = 5;
  dog1.direction = 0;
  dog1.power_active = 0;
  dog1.type = 2;
  dog1.is_the_active = 0;

  set_character_sprite(&bunny);
  set_character_sprite(&dog1);



}

int is_ded(CharacterController** c) {

  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;

  if ((*c)->power_active && (*c)->type == 1) {
    return 0;
  }

      // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;

  _x = (((unsigned) (*c)->x) / 8) -1;
  _y = (((unsigned) (*c)->y) / 8) -2;


  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  switch((UINT16)Map1[map_position]) {
    case (UINT16)2:
    // case (UINT16)7:
    // case (UINT16)9:
    // case (UINT16)4:
    // case (UINT16)10:
      // printf("PlayerNotMove\n");
      return 1;
  }
  return 0;
}

void player_input(CharacterController** c) {
  // printf("%d %d %d\n", input_timer, (*c)->power_timer, (*c)->power_active);

  if ((*c)->power_timer == 0 && input_timer == 0) {

    switch (joypad()) {
      case J_A:
        (*c)->power_active = 1;
        // input_timer needs to be updated inside the player stuff
        break;

      case J_B:
        // printf("troca\n");
        input_timer = 10;
        (*c)->direction = 0; 

        break;

      case J_LEFT:
        (*c)->direction = 4; 
        if (can_move((*c)->x, (*c)->y, (*c)->direction)) {
          input_timer = 4;
        }
        break;
      case J_RIGHT:
        (*c)->direction = 2; 
        if (can_move((*c)->x, (*c)->y, (*c)->direction)) {
          input_timer = 4;
        }
        break;
      case J_UP:
        (*c)->direction = 1; 
        if (can_move((*c)->x, (*c)->y, (*c)->direction)) {
          input_timer = 4;
        }
        break;
      case J_DOWN:
        (*c)->direction = 3; 
        if (can_move((*c)->x, (*c)->y, (*c)->direction)) {
          input_timer = 4;
        }
        break;
    }
  }

  // move_sprite((*c)->type, (*c)->x, (*c)->y);

  // c->x = 10;
  // move_sprite(*c->type, *c->x, *c->y);
}

int can_move(INT8 x, INT8 y, UINT8 direction) {
  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;
      // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;
  // return 1;

  // Translate the xy to the map tile they are currently on.
  // Resolution: 160x144 pixels (AKA 20x18 8x8-pixel tiles)
  
  // Updatex x y to test if we can move there
  switch(direction) {
    case 4: //LEFT
      x -= 8; 
      break;
    case 2: // RIGHT
      x += 8; 
      break;
    case 1: // UP
      y -= 8; 
      break;
    case 3: // DOWN
      y += 8; 
      break;
  }

  _x = (((unsigned) x) / 8) -1;
  _y = (((unsigned) y) / 8) -2;


  //printf("x:%u _ %u %u\n", (unsigned) x, (unsigned) _x, (unsigned) direction);
  // x logic
  // 8 -> 0 (because it starts at 8)
  if (_x < 0 || (unsigned) _x > 19 || _y < 0 || (unsigned) _y > 17) {
    return 0;
  }

  // If we are here, they are not on a border of the map, lets test other collisions.

  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  switch((UINT16)Map1[map_position]) {
    case (UINT16)1:
    // case (UINT16)2:
    // case (UINT16)3:
    case (UINT16)4:
    // case (UINT16)7:
    // case (UINT16)9:
    // case (UINT16)4:
    // case (UINT16)10:
      // printf("PlayerNotMove\n");
      return 0;
  }

  // UINT8 x_map = (x * 20);
  // printf("%d %d\n", x, x_map );
  return 1;
}

void move_character(CharacterController* c) {

  // We only want this to run for the current active player.
  if (player->type != c->type){
    return;
  }

  if (c->power_active != 0) {

    // Bunny power  -----------------------------------------
    if (c->type == 1) {// bunny
      // First time running here
      if (c->power_active == 1) {
        // printf("Power active 1\n");
        c->power_active = 2;
        c->power_timer = 16;
      }

      set_sprite_tile(c->type, c->sprite_3);

      // Test once each time we are on a tile
      if (c->power_timer == 4 ||
          c->power_timer == 8 ||
          c->power_timer == 12 || 
          c->power_timer == 16) 
      {
        if (!can_move(c->x, c->y, c->direction)) {
          c->direction = 0;
        }
      }

      switch(c->direction) {
        case 4: //LEFT
          c->x -= 2; 
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          c->x += 2; 
          set_sprite_prop(c->type, 0);
          break;
        case 1: // UP
          c->y -= 2; 
          break;
        case 3: // DOWN
          c->y += 2; 
          break;
      }
      c->power_timer -= 1;

      if (c->power_timer == 0) {
        c->power_active = 0;
        set_sprite_tile(c->type, c->sprite_1);
      }
    }

    // Dog power -----------------------------------------
    if (c->type == 2) {
      set_sprite_tile(c->type, c->sprite_3);

      if (c->power_active == 1) {
        // printf("Power active 1\n");
        c->power_active = 2;
        c->power_timer = 16;
      }
      c->power_timer -= 1;

      if (c->power_timer == 0) {
        c->power_active = 0;
        set_sprite_tile(c->type, c->sprite_1);
      }
    }
    
  }
  else {
    // Normal movement
    if (input_timer != 0) {
      switch(c->direction) {
        case 4: //LEFT
          c->x -= 2; 
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          c->x += 2; 
          set_sprite_prop(c->type, 0);
          break;
        case 1: // UP
          c->y -= 2; 
          break;
        case 3: // DOWN
          c->y += 2; 
          break;
      }
    }
    if (input_timer == 4) {
      set_sprite_tile(c->type, c->sprite_2);
    }
    if (input_timer == 2) {
      set_sprite_tile(c->type, c->sprite_1);
    }
  }

  move_sprite(c->type, c->x, c->y);
}

void set_character_sprite(CharacterController* c) {
  set_sprite_tile(c->type, c->sprite_1);
  move_sprite(c->type, c->x, c->y);
}
// 