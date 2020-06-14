#include <gb/gb.h>
#include <gb/cgb.h>


UBYTE running;
UINT8 timer;
UINT8 characters_available;
UBYTE input_timer;
UBYTE current_map;
// UBYTE16 player_map_position;
// UBYTE map
unsigned char map[360]; // Controlls which map we are seeing.

#include "sprites.c"
#include "maps.c"

#include "CharacterController.c"
#include "ObjectController.c"

#include "sprites/Sprites.c"

// If by collecting this object the map is won.
#define OBJECT_WIN_CONDITION_FLAG    0x01U

// If you can push this object
#define OBJECT_MOVABLE_FLAG      0x02U

// If any character is in this object position, you lose the map.
#define OBJECT_DEATH_FLAG     0x04U

#define RGB_GREEN_1    RGB(28, 30, 26)
#define RGB_GREEN_2    RGB(18, 22, 14)
#define RGB_GREEN_3    RGB(7, 12, 10)
#define RGB_GREEN_4    RGB(1, 3, 4)
#define RGB_BLUE       RGB( 0,  0, 31)
#define RGB_PURPLE     RGB(21,  0, 21)
#define RGB_LIGHTFLESH RGB(30, 20, 15)

#define RGB_RED        RGB(31,  0,  0)
#define RGB_BLUE       RGB( 0,  0, 31)
#define RGB_PURPLE     RGB(21,  0, 21)
#define RGB_LIGHTFLESH RGB(30, 20, 15)

UWORD spritePalette[] = {
  RGB_GREEN_1, RGB_GREEN_2, RGB_GREEN_3, RGB_GREEN_4,
  // 0, RGB_GREEN, RGB_BLUE, RGB_PURPLE,
  // 0, RGB_BLACK, RGB_RED, RGB_WHITE
};



CharacterController bunny;  //1
CharacterController dog1;   //2
CharacterController dog2;   //3
CharacterController cat;    //4
CharacterController horse;  //5
CharacterController *player;
ObjectController key;
ObjectController item1;
ObjectController item2;
ObjectController item3;

void performantDelay(UINT8 numLoops) {
  UINT8 i;
  for (i = 0; i != numLoops; i++) {
    wait_vbl_done();
  }
}

UINT8 _get_y_from_map_position(UINT16 map_position) {
  // return 16;
  return ((map_position / 20) + 2) * 8;
}

UINT8 _get_x_from_map_position(UINT16 map_position) {
  // return 8;
  return ((map_position % 20) + 1) * 8;
}

int can_move_to_map_pos(UINT16 map_position);

int got_door(CharacterController** c);

// Check player controls.
void player_input(CharacterController** c);

// Check player is ded.
int is_ded(CharacterController** c);

// Inits the game base variables, sprites, background.
void map_init();

UINT16 _get_next_map_position(UINT16 map_pos, UINT8 direction);

int got_key(CharacterController** c);

// Handles the first map logic.
void map_loop();

void set_character_sprite(CharacterController* c);

void move_character(CharacterController* c);

int can_move(INT8 x, INT8 y, UINT8 direction);

UINT16 _get_map_position_from_xy(UINT8 x,UINT8 y) ;

void dog1_power_apply(UINT16 map_position_block, UINT8 direction, UINT16 map_position_next);

void timers() {
  // // printf("%d\n", input_timer);
  // if (input_timer != 0) {
    // input_timer -= 1;
  // }
  timer += 1;
}


void main(void)
{
  init();
  map_init();

  while(running) {

    timers();
    map_loop();
    performantDelay(2);
  
  }

  // printf("The end\n");
  while (1) {

  }
}

void init() {
  // current_map = MAP_DEV;
  current_map = MAP_START;
  // current_map = MAP_;
  characters_available = 2;

  copy_map(current_map);
  set_bkg_data(0,127,Sprites);
  set_bkg_tiles(0, 0, 20, 18, map);
  
  set_sprite_data(0,30,Characters);
  set_sprite_palette(0, 4, spritePalette);
  // set_sprite_palette(0, 1, spritePalette[1]);
  // set_sprite_palette(0, 2, spritePalette[2]);
  // set_sprite_palette(0, 3, spritePalette[3]);
  set_bkg_palette(0, 3, spritePalette);
  set_sprite_prop(0, 0);
  // set_sprite_prop(0, S_PALETTE);


  // BGP_REG = 0xF9U;
  // BGP_REG = 0xFEU;
  // BGP_REG = 0xFFU;
  BGP_REG = 0xE4U;
  // BGP_REG = OBP0_REG = OBP1_REG = 0xE4U;

  DISPLAY_ON;
  
  SHOW_BKG;

  SHOW_SPRITES;

  input_timer = 0u;
  running = 1u;
  timer = 1u;
  player = &bunny;
}

void spread_fire(UINT16 map_position) {
  UINT16 map_test;
  
  // right
  if ((map_position+1) % 20 != 0) {
    map_test = map_position +1;
    if (map[map_test] == 21 || map[map_test] == 23) {
      map[map_test] = 27;
    }
  }

  // left
  if ((map_position) % 20 != 0) {
    map_test = map_position -1;
    if (map[map_test] == 21 || map[map_test] == 23) {
      map[map_test] = 27;
    }
  }

  // up
  if (map_position > 19) {
    map_test = map_position -20;
    if (map[map_test] == 21 || map[map_test] == 23) {
      map[map_test] = 27;
    }
  }

    // down
  if (map_position < 340) {
    map_test = map_position +20;
    if (map[map_test] == 21 || map[map_test] == 23) {
      map[map_test] = 27;
    }
  }
}

// Update map 1 water
// Update super cracked ice to be abism
void map_enviroment_tiles(){
  UINT16 map_position;

  // Water sprites
  unsigned char sprite_water_1[] =
  {
    0xFF,0x00,0xDD,0x00,0xAA,0x00,0xFF,0x00,
    0xFF,0x00,0xBB,0x00,0x55,0x00,0xFF,0x00
  };

  unsigned char sprite_water_2[] =
  {
    0xFF,0x00,0xBB,0x00,0x55,0x00,0xFF,0x00,
    0xFF,0x00,0xDD,0x00,0xAA,0x00,0xFF,0x00
  };

  // Fire sprites
  unsigned char sprite_fire_1[] = // big
  {
    0x18,0x00,0x28,0x10,0x6C,0x10,0x56,0x28,
    0x5A,0x24,0x5A,0x24,0x66,0x18,0x3C,0x00,
  };

  unsigned char sprite_fire_2[] = //small
  {
    0x00,0x00,0x10,0x00,0x28,0x10,0x2C,0x10,
    0x56,0x28,0x5A,0x24,0x66,0x18,0x3C,0x00
  };

  unsigned char sprite_fire_3[] = //small
  {
    0x00,0x00,0x00,0x00,0x10,0x00,0x28,0x10,
    0x2C,0x10,0x56,0x28,0x66,0x18,0x3C,0x00,
  };

    
  

  // Update water
  if (timer % 32 == 0) {
    if (timer % 64 == 0) {
      set_bkg_data(11,1,sprite_water_1);
      
    }
    else {
      set_bkg_data(11,1,sprite_water_2);
      
    }
  }
  
  // Update fire
  if (timer % 32 == 0) {
    if (timer % 64 == 0) {
      set_bkg_data(25,1,sprite_fire_1);
      set_bkg_data(26,1,sprite_fire_3);
      
    }
    else {
      set_bkg_data(25,1,sprite_fire_2);
      set_bkg_data(26,1,sprite_fire_2);

    }
  }

  // Only update fire if the player isn't moving
  // if (input_timer == 0) {
  //   // Update fire tiles
  //   if (timer == 0) {
  //     map_position = 360;
  //     while (map_position != 0) {
  //       map_position -= 1;
  //       // // printf("%d\n", map_position);
  //       switch((UINT16)map[map_position]) {
  //         case (UINT16) 25: // big fire
  //           map[map_position] = 26; // little fire
  //           spread_fire(map_position);
  //           break;

  //         case (UINT16) 26: // little fire
  //           spread_fire(map_position);
  //           map[map_position] = 49; // burned tile
  //           break;

  //       }
  //     }
  //   }
  //   // // spread fire!
  //   // if (timer == 10) {
  //   //   //spread fire
  //   //   map_position = 360;
  //   //   while (map_position != 0) {
  //   //     map_position -= 1;
  //   //     // // printf("%d\n", map_position);
  //   //     switch((UINT16)map[map_position]) {
  //   //       case (UINT16) 25: // big fire
  //   //       case (UINT16) 26: // little fire
  //   //         spread_fire(map_position);
  //   //         break;
  //   //     }
  //   //   }
  //   // }
  //   //   // spread fire!
  //   // if (timer == 20) {
  //   //   //spread fire
  //   //   map_position = 120;
  //   //   while (map_position != 0) {
  //   //     map_position -= 1;
  //   //     // // printf("%d\n", map_position);
  //   //     switch((UINT16)map[map_position]) {
  //   //       case (UINT16) 25: // big fire
  //   //       case (UINT16) 26: // little fire
  //   //         spread_fire(map_position);
  //   //         break;

  //   //     }
  //   //   }
  //   // }

  //   //   // spread fire!
  //   // if (timer == 30) {
  //   //   //spread fire
  //   //   map_position = 240;
  //   //   while (map_position != 120) {
  //   //     map_position -= 1;
  //   //     // // printf("%d\n", map_position);
  //   //     switch((UINT16)map[map_position]) {
  //   //       case (UINT16) 25: // big fire
  //   //       case (UINT16) 26: // little fire
  //   //         spread_fire(map_position);
  //   //         break;

  //   //     }
  //   //   }
  //   // }

  // }

  // Update any tile that needs updating
  // only if the player isn't moving
  if (input_timer == 0 && timer % 32 == 0) {
    map_position = 360;
    while (map_position != 0) {
      map_position -= 1;
      // // printf("%d\n", map_position);
      switch((UINT16)map[map_position]) {
        case (UINT16) 17: // Cracked ice
          map[map_position] = SPRITE_VOID;
          break;
        case (UINT16) 19: // Cracked ground
          map[map_position] = SPRITE_VOID;
          
          break;
        case (UINT16) 27: // Started fire!
          map[map_position] = 25; // big fire

      }
    }
  }
  
  set_bkg_tiles(0, 0, 20, 18, map);
  
}

// Rotate char based on available chars on this map.
// IE: chars that do not have their x as 0
int change_char() {
  UINT8 type = player->type;

  if (type == 1) { // bunny
    if(dog1.x != 0) {
      player = &dog1;
      return 0;
    }
    if(dog2.x != 0) {
      player = &dog2;
      return 0;
    }
    if(cat.x != 0) {
      player = &cat;
      return 0;
    }
  }

  if (type == 2) { // dog1
    if(dog2.x != 0) {
      player = &dog2;
      return 0;
    }
    if(cat.x != 0) {
      player = &cat;
      return 0;
    }
    if(bunny.x != 0) {
      player = &bunny;
      return 0;
    }
  }

  if (type == 3) { // dog2
    if(cat.x != 0) {
      player = &cat;
      return 0;
    }
    if(bunny.x != 0) {
      player = &bunny;
      return 0;
    }
    if(dog1.x != 0) {
      player = &dog1;
      return 0;
    }
  }

  if (type == 4) { // dog2
    if(bunny.x != 0) {
      player = &bunny;
      return 0;
    }
    if(dog1.x != 0) {
      player = &dog1;
      return 0;
    }
    if(dog2.x != 0) {
      player = &dog2;
      return 0;
    }
  }

  // Only one char available, so we just return 0;
}

void map_loop() {
  
  if (is_ded(&player)) {
    map_init();
  }

  got_door(&player);
  // // printf("%u\n", (UINT16)_get_map_position_from_xy(player->x, player->y));
  got_key(&player);
  map_enviroment_tiles();

  // generate_bunny();
  player_input(&player);

 
  move_character(&bunny);
  move_character(&dog1);
  move_character(&dog2);
  move_character(&cat);


}


void map_init() {
  copy_map(current_map);
  instanciate_chars();
  set_bkg_tiles(0, 0, 20, 18, map);


  // key.x = 152;
  // key.y = 144;
  // // key.x = 8;
  // // key.y = 32;
  // key.sprite_1 = 14;


  // set_character_sprite(&key);

}

void instanciate_chars() {
  UINT16 i;

  // generate bunny
  bunny.x = 0;//8
  bunny.y = 0;//16
  bunny.sprite_1 = 0;
  bunny.sprite_2 = 1;
  bunny.sprite_3 = 2;
  bunny.sprite_4 = 16;
  bunny.sprite_5 = 17;
  bunny.sprite_6 = 18;
  bunny.direction = 0;
  bunny.power_active = 0;
  bunny.type = 1;
  bunny.map_position = 400;

  dog1.x = 0;
  dog1.y = 0;
  dog1.sprite_1 = 3;
  dog1.sprite_2 = 4;
  dog1.sprite_3 = 5;
  dog1.sprite_4 = 19;
  dog1.sprite_5 = 20;
  dog1.sprite_6 = 21;
  dog1.direction = 0;
  dog1.power_active = 0;
  dog1.type = 2;
  dog1.map_position = 400;

  dog2.x = 0;
  dog2.y = 0;
  dog2.sprite_1 = 6;
  dog2.sprite_2 = 7;
  dog2.sprite_3 = 8;
  dog2.sprite_4 = 22;
  dog2.sprite_5 = 23;
  dog2.sprite_6 = 24;
  dog2.direction = 0;
  dog2.power_active = 0;
  dog2.type = 3;
  dog2.map_position = 400;

  cat.x = 0;
  cat.y = 0;
  cat.sprite_1 = 9;
  cat.sprite_2 = 10;
  cat.sprite_3 = 5;
  cat.sprite_4 = 19;
  cat.sprite_5 = 20;
  cat.sprite_6 = 21;
  cat.direction = 0;
  cat.power_active = 0;
  cat.type = 4;
  cat.map_position = 400;

  // Update players positions.
  i = 0;
  while(i != 360) {
    // // printf("%u - %u\n", i, map[i]);
    // performantDelay(2);
    switch (map[i]) {
      case 50: // bunny
        bunny.y = _get_y_from_map_position(i);
        bunny.x = _get_x_from_map_position(i);
        map[i] = 0;

        break;
      case 51: // dog1
        dog1.y = _get_y_from_map_position(i);
        dog1.x = _get_x_from_map_position(i);
        map[i] = 0;
        break;
      case 52: // dog2
        dog2.y = _get_y_from_map_position(i);
        dog2.x = _get_x_from_map_position(i);
        map[i] = 0;
        break;
      case 53: // cat
        cat.y = _get_y_from_map_position(i);
        cat.x = _get_x_from_map_position(i);
        map[i] = 0;
        break;
      // default:
        // map[i] = 0;
    }

    i += 1;
  }

  set_character_sprite(&bunny);
  set_character_sprite(&dog1);
  set_character_sprite(&dog2);
  set_character_sprite(&cat);

  // Update player char (in case we don't have 
  // the bunny, for example, get the next available
  // character)
  if(bunny.x != 0) {
    player = &bunny;
    return;
  }
  if(dog1.x != 0) {
    player = &dog1;
    return;
  }
  if(dog2.x != 0) {
    player = &dog2;
    return;
  }
  if(cat.x != 0) {
    player = &cat;
    return;
  }
}


int got_key(CharacterController** c) {
  UINT16 player_map_position = _get_map_position_from_xy(player->x, player->y);

  // 46 is the key
  if ((UINT16)map[player_map_position] == 46) {
    // update the block so you can pass it
    UINT16 map_position;


    map_position = 360;
    while (map_position != 0) {
      map_position -= 1;
      // Update locks and keys tiles.
      switch((UINT16)map[map_position]) {
        case (UINT16) 41: // The lock
          map[map_position] = 42;
          break;
        case (UINT16) 46: // The lock
          map[map_position] = 9;
          break;

      }
    }
    set_bkg_tiles(0, 0, 20, 18, map);
  }
}

// the x position on the map has an extra 8 pixels positioning
int fix_x(UINT16 x) {
  return (x - 8) / 8;
}

int fix_y(UINT16 x) {
  return (x - 16) / 8;
}



int got_door(CharacterController** c) {
  UINT16 player_map_position = _get_map_position_from_xy(player->x, player->y);

  if (player->power_active && player->type == 1) {
    // The bunny is jumping!.
    return 0;
  }
  
  // // printf("%d - %d\n", player->x, player->y);
  // // printf("%d - %d\n", fix_x(player->x), fix_y(player->y));

  if ((UINT16)map[player_map_position] == SPRITE_DOOR) {
        //     cat.y = _get_y_from_map_position(i);
        // cat.x = _get_x_from_map_position(i);
    current_map = got_door_switch(
      fix_x(_get_x_from_map_position(player_map_position)),
      fix_y(_get_y_from_map_position(player_map_position)), current_map);
    map_init();  
  }
}


int is_ded(CharacterController** c) {

  UINT16 map_position;

  if ((*c)->power_active && (*c)->type == 1) {
    // The bunny is jumping!.
    return 0;
  }

      // // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;
  map_position = _get_map_position_from_xy((*c)->x, (*c)->y);


  switch((UINT16)map[map_position]) {
    case (UINT16)11: // Wata
    case (UINT16)SPRITE_VOID: // abism

      return 1;
  }
  return 0;
}

// Return 1 if from the map_pos on the direction there are more ice
// or something that would make it slide
// int can_slide_to_map_pos(UINT16 map_pos) {
//   UINT16 next_map_pos;
//   UINT8 i;
//   i = 20;
//   if ((UINT16)map[map_pos] == 13 || (UINT16)map[map_pos] == 14) {
//     next_map_pos =_get_next_map_position(map_pos, direction); // handles colision
//     if(map_pos == next_map_pos) {
//       return 0;
//     }
//     return 1;
//   }
  
  
//   return 0;
// }

void dog1_power() {
  UINT16 map_position_block;
  UINT16 map_position_next;

  set_sprite_tile(player->type, player->sprite_3);

  map_position_block = _get_map_position_from_xy(player->x, player->y);
  
  map_position_block = _get_next_map_position(map_position_block,player->direction );
  map_position_next = _get_next_map_position(map_position_block, player->direction);

  // // printf("%u %u\n", map_position_block, map_position_next);
  switch((UINT16)map[map_position_block]) {
    case (UINT16)45:// Brunio's block
    case (UINT16)15:// Brunio's block on ice
    case (UINT16)16:// Brunio's block on cracked ice
      // Update the position on the map to be the block after the block movment 47
      
      // Is there something there, like a player, that should make it notmove?
      if (can_move_to_map_pos(map_position_next) == 0) {
        break;
      }
      


      // move it!

      if ((UINT16)map[map_position_next] < (UINT16)13 || 
        (UINT16)map[map_position_next] == 47 || 
        (UINT16)map[map_position_next] == 48 ||
        (UINT16)map[map_position_next] == 42) {

        dog1_power_apply((UINT16)map_position_block, (UINT8)player->direction, map_position_next);
      }

      // The ice makes the block go way further, so we have a loop here to handle that.
      if ((UINT16)map[map_position_next] == 13 || (UINT16)map[map_position_next] == 14) {
        
        dog1_power_apply((UINT16)map_position_block, (UINT8)player->direction, map_position_next);
        map_position_block = map_position_next;
          map_position_next = _get_next_map_position(map_position_block, player->direction);

        while((UINT16)map[map_position_block] == 13
            || (UINT16)map[map_position_block] == 14
            || (UINT16)map[map_position_block] == 15
            || (UINT16)map[map_position_block] == 16) {

          if (can_move_to_map_pos(map_position_next) == 0) {
            break;
          }
          dog1_power_apply((UINT16)map_position_block, (UINT8)player->direction, map_position_next);
          map_position_block = map_position_next;
          map_position_next = _get_next_map_position(map_position_block, player->direction);
        }
        

      }
      // switch ((UINT16)map[map_position_next]) {
        
      // }

      break;
  }
  // // printf("4\n");
  // UINT8 x_map = (x * 20);
  // // printf("%d %d\n", x, x_map );
  // return 1;
  set_sprite_tile(player->type, player->sprite_1);
}

UINT16 _get_next_map_position(UINT16 map_pos, UINT8 direction) {
  switch(direction) {
    case 1: //up
      if (map_pos > 20) {
        map_pos -= 20;
      }
      break;
    case 2: //right  
      if (((map_pos + 1) % 20) != 0) {
        map_pos += 1;
      }
      break;
    case 3: //down  
      if (map_pos < 340) {
        map_pos += 20;
      }
      break;
    case 4: //left
      if (((map_pos) % 20) != 0) {
        map_pos -= 1;
      }
      // map_pos -= 1;
      break;
  }
  return map_pos;
}

void dog1_power_apply(UINT16 map_position_block, UINT8 direction, UINT16 map_position_next) {
  UINT8 x;
  UINT8 y;
  UINT8 i;

  // Update the current tile
  if ((UINT16)map[map_position_block] == 45) {  // If its a normal block
    map[map_position_block] = 47;               // update to dirt
  }
  if ((UINT16)map[map_position_block] == 15) {  // If its a block on ice
    map[map_position_block] = 13;               // update to ice again
  }
  if ((UINT16)map[map_position_block] == 16) {  // If its a craced ice with block
    map[map_position_block] = 17;               // now its more cracked ice and it will be abism
  }
  // set_bkg_tiles(0, 0, 20, 18, map);

  // Creates the sprite block
  y = ((map_position_block / 20 )*8) + 16;
  x = ((map_position_block % 20 )*8) + 8;
  set_sprite_tile(5, 15);
  
  // move the sprite
  i = 4;
  move_sprite(5, x, y);   
  
  while(i != 0) {
    switch (direction) {
      case 1:
        y -= 2;
        break;
      case 2:
        x += 2;
        break;
      case 3:
        y += 2;
        break;
      case 4:
        x -= 2;
        break;
    }
    performantDelay(2);
    move_sprite(5, x, y);  
    i -=1;
  }

  // Now that the block has stopped moving,
  // we update the tile it got to
  switch (map[map_position_next]) {
    case 11: // Water
      map[map_position_next] = 48; // wet dirt
      break;
    case SPRITE_VOID: // abism, does nothing, it felt
      break;
    case 13: // ice
      map[map_position_next] = 15; // ice with block
      break;
    case 14: // cracked_ice
      map[map_position_next] = 16; // now it is an abism
      break;
    default:
      map[map_position_next] = 45; // normal dog1 block
      break;
  }
  set_bkg_tiles(0, 0, 20, 18, map); 
  move_sprite(5, 0, 0);  
  

}

// Given x an y coordinates, get the correponding map 
// position integer.
UINT16 _get_map_position_from_xy(UINT8 x,UINT8 y) {
  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;

  // Translate the xy to the map tile they are currently on.
  // Resolution: 160x144 pixels (AKA 20x18 8x8-pixel tiles)
  
  _x = (((unsigned) x) / 8) -1;
  _y = (((unsigned) y) / 8) -2;

  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  return map_position;
}



void player_input(CharacterController** c) {
  // // printf("%d %d %d\n", input_timer, (*c)->power_timer, (*c)->power_active);

  if ((*c)->power_timer == 0 && input_timer == 0) {

    switch (joypad()) {
      case J_A:
        if ((*c)->type == 1) {
          (*c)->power_timer = 16;
          // input_timer = 16;
          (*c)->power_active = 1;
          // input_timer = 4;
        }
        if ((*c)->type == 2) {
          dog1_power();
        }
        
        // input_timer needs to be updated inside the player stuff
        break;

      case J_B:
        // // printf("troca\n");
        change_char();

        // This will make the player know which player char was 
        // selected and give some time so it does not keep
        // pressing and changing chars.
        rotate_player();
        break;

      case J_LEFT:
          input_timer = 4;
          (*c)->direction = 4; 
        break;
      case J_RIGHT:
          input_timer = 4;
          (*c)->direction = 2; 
        break;
      case J_UP:
          input_timer = 4;
          (*c)->direction = 1; 
        break;
      case J_DOWN:
          (*c)->direction = 3; 
          input_timer = 4;
        break;


    }
    // Update cracked ground
    if (input_timer != 0) { // Ie, the player is going to move
      if (map[(*c)->map_position] == 18) { // Cracked ice
        map[(*c)->map_position] = 19; // Cracked ice even more cracked
        set_bkg_tiles(0, 0, 20, 18, map); 

      }
    }
  }


  // move_sprite((*c)->type, (*c)->x, (*c)->y);

  // c->x = 10;
  // move_sprite(*c->type, *c->x, *c->y);
}

int can_move_to_map_pos(UINT16 map_position) {
  // // printf("%u\n", map_position);
  if ((UINT16)map[map_position] > (UINT16)20 &&
    (UINT16)map[map_position] < (UINT16)41) {
    // // printf("2\n");
    return 0;
  }

  // Or, test individual blocks
  switch((UINT16)map[map_position]) {
    case (UINT16)41:
    case (UINT16)45:// Brunio's block
    case (UINT16)15:// Brunio's block on ice
    case (UINT16)16:// Brunio's block on cracked ice

    // // printf("PlayerNotMove\n");
    // // printf("3\n");
      return 0;
      break;
  }

  if ((UINT16)map_position == bunny.map_position 
    || (UINT16)map_position == dog1.map_position 
    || (UINT16)map_position == dog2.map_position 
    || (UINT16)map_position == cat.map_position ) {
    // f("3\n");
    return 0;
  }
  // f("4\n");
  return 1;
}

int can_move(INT8 x, INT8 y, UINT8 direction) {
  UINT8 _x;
  UINT8 _y;
  UINT16 map_position;
      // // printf("%u %u\n", (unsigned) player->x, ((unsigned) player->x) / 8) -1;
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
    case 0:
      return 0;
  }

  _x = (((unsigned) x) / 8) -1;
  _y = (((unsigned) y) / 8) -2;


  //// printf("x:%u _ %u %u\n", (unsigned) x, (unsigned) _x, (unsigned) direction);
  // x logic
  // 8 -> 0 (because it starts at 8)
  // Stops the player of leaving the map.
  if (_x < 0 || (unsigned) _x > 19 || _y < 0 || (unsigned) _y > 17) {
    // // printf("1\n");
    return 0;
  }

  // If we are here, they are not on a border of the map, lets test other collisions.

  // Map position
  map_position = 0;
  map_position += (unsigned) _x;
  map_position += ((unsigned) _y) * 20;

  if (can_move_to_map_pos(map_position)) {
    return 1;
  }

  return 0;
}

void rotate_player() {
  set_sprite_tile(player->type, player->sprite_6);
  set_sprite_prop(player->type, S_FLIPX);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_3);
  set_sprite_prop(player->type, S_FLIPX | S_FLIPY);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_6);
  set_sprite_prop(player->type, S_FLIPY);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_3);
  set_sprite_prop(player->type, 0);
  performantDelay(5);
  set_sprite_tile(player->type, player->sprite_1);
  set_sprite_prop(player->type, 0);
}

void move_character(CharacterController* c) {
  UINT8 movement;
  // We only want this to run for the current active player.
  if (player->type != c->type){
    return;
  }

  if (c->power_active != 0) {

    // Bunny power  -----------------------------------------
    if (c->type == 1) {// bunny
      // First time running here
      movement = 2;

      // Test once each time we are on a tile
      if (c->power_timer == 4 ||
          c->power_timer == 8 ||
          c->power_timer == 12 || 
          c->power_timer == 16) 
      {
        if (!can_move(c->x, c->y, c->direction)) {
          c->power_timer = 1; // 1 will make it end this loop.
          // // printf('cantmove\n');
          movement = 0;
        }
      }
      
      // Do the movment 
      switch(c->direction) {
        case 4: //LEFT
          c->x -= movement;
          set_sprite_tile(c->type, c->sprite_3);
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          set_sprite_tile(c->type, c->sprite_3);
          set_sprite_prop(c->type, 0);
          c->x += movement; 
          break;
        case 1: // UP
          set_sprite_tile(c->type, c->sprite_6);
          set_sprite_prop(c->type, S_FLIPX);
          c->y -= movement; 
          break;
        case 3: // DOWN
          set_sprite_tile(c->type, c->sprite_6);
          set_sprite_prop(c->type, S_FLIPY);
          c->y += movement; 
          break;
        case 0:
          rotate_player();
          // The player didn't chose a direction, so we just... do a loop!
      }
      
      c->power_timer -= 1;
      if (c->power_timer == 0) {
        c->power_active = 0;
        switch(c->direction) {
          case 4: //LEFT
            // Add the feet movement
            set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPX);
            break;
          case 2: // RIGHT
            // Add the feet movement
            set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, 0);
            break;
          case 1: // UP
            set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPX);
            break;
          case 3: // DOWN
            set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
            set_sprite_prop(c->type, S_FLIPY);
            break;
        }
      }
      // Update the input timer!
      if (input_timer != 0) {
        input_timer -= 1;
      }
    }

    // Dog power -----------------------------------------
    if (c->type == 2) {
      set_sprite_tile(c->type, c->sprite_3);

      if (c->power_active == 1) {
        // // printf("Power active 1\n");
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


    // The first time the player moves, the input timer is 4
    // then we test if the player can move, if not, we cancel
    // all other checks by setting the input timer as incatctive
    if (input_timer == 4) {
      if (!can_move(c->x, c->y, c->direction)) {
        input_timer = 0;
        // But, we still need to update the sprites accordingly, since
        // the last direction is used by the powers.
      }
    }

    // Handle the sprites. We have this separated from the movment
    // because the movment can not happen in the end.
    switch(c->direction) {
        case 4: //LEFT
          // Add the feet movement
          set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_2);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_1);
          }
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 2: // RIGHT
          // Add the feet movement
          set_sprite_tile(c->type, c->sprite_1); // Default in case the player cannot move to this direction
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_2);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_1);
          }
          // Unflip just to be sure
          set_sprite_prop(c->type, 0);
          break;
        case 1: // UP
          set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
          // Add the feet movement
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_5);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_4);
          }
        
          set_sprite_prop(c->type, S_FLIPX);
          break;
        case 3: // DOWN
          set_sprite_tile(c->type, c->sprite_4); // Default in case the player cannot move to this direction
          // Add the feet movement
          if (input_timer == 4) {
            set_sprite_tile(c->type, c->sprite_5);
          }
          if (input_timer == 2) {
            set_sprite_tile(c->type, c->sprite_4);
          }
          set_sprite_prop(c->type, S_FLIPY);
          break;
      }

    // Normal movement
    if (input_timer != 0) {
      
      switch(c->direction) {
        case 4: //LEFT
          c->x -= 2;
          break;
        case 2: // RIGHT
          c->x += 2; 
          break;
        case 1: // UP
          c->y -= 2; 
          break;
        case 3: // DOWN
          c->y += 2; 
          break;
      }
    }

    if (map[c->map_position] == 14) { // Cracked ice
      map[c->map_position] = 17; // Cracked ice even more cracked
      set_bkg_tiles(0, 0, 20, 18, map); 
    }
    
    // Update the input timer!
    if (input_timer != 0) {
      input_timer -= 1;
      // We only test if the ice should slide IF the player WAS moving this turn.
      c->map_position = _get_map_position_from_xy(c->x, c->y);
      if (map[c->map_position] == 13 || map[c->map_position] == 14 || map[c->map_position] == 17 ) {
        if (can_move(c->x, c->y, c->direction) && input_timer == 0) {
          input_timer = 4;
        }
      }
    }

  }

  c->map_position = _get_map_position_from_xy(c->x, c->y);
  move_sprite(c->type, c->x, c->y);

  

}

void set_character_sprite(CharacterController* c) {
  // We use the x as the way to know if the char is set
  // if (c->x != 0 ) {
    set_sprite_tile(c->type, c->sprite_1);
    move_sprite(c->type, c->x, c->y);  
    c->map_position = _get_map_position_from_xy(c->x, c->y);
  // }
}
// 