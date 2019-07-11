#include <cpctelera.h>
#include <stdio.h>

#define MAP_WIDTH 6
#define SCALE 4 // Used for the drawing of squares
#define INITIAL_Y_POS 0xC0A0 // So the map doesn't destroy print stats

// MACRO where we pass the keyPress and the corresponding movement in the map.
// The while loop is so that the player has to lift up before moving again. 
#define MOVE(key, movement) \
	if (cpct_isKeyPressed(key)) { \
		movePlayer(map, movement, &player, &enemy); \
	while(cpct_isKeyPressed(key)) \
		cpct_scanKeyboard(); \
	}

// IDEAS for entity: 
// - X and Y pos
// - Speed to see which entity attacks first
struct Entity {
	u8 health, 
	   attack, 
	   defense;
};

void drawMap(const u8* map)
{
	u8 height = 0;

	for(u8 i = 0; i <= MAP_WIDTH; ++i)
	{
		if ( i != 0 && i % MAP_WIDTH == 0 ) 
		{
			++height;
			i = 0;
		}

		if(height == MAP_WIDTH)
		{
			break; // Consider changing to GOTO for the memes
		}

		switch( map[i+height*MAP_WIDTH] )
		{
			case 1: // WALL
				cpct_drawSolidBox ((void*) (INITIAL_Y_POS + (height*0x0050*SCALE) + (i * 0x0002*SCALE) ) , 0xF0, 2*SCALE, 8*SCALE);
				break;
			case 2: // PLAYER
				cpct_drawSolidBox ((void*) (INITIAL_Y_POS + (height*0x0050*SCALE) + (i * 0x0002*SCALE) ) , 0x0F, 2*SCALE, 8*SCALE);
				break;
			case 3: // ENEMY
				cpct_drawSolidBox ((void*) (INITIAL_Y_POS + (height*0x0050*SCALE) + (i * 0x0002*SCALE) ) , 0xFF, 2*SCALE, 8*SCALE);
				break;
		}
	}
}

void movePlayer(u8* map, i8 movement, struct Entity* player, struct Entity* enemy)
{
	// TODO: Switch this method so that it uses a stored x and y pos in the entity
	for(u8 i = 0; i < MAP_WIDTH * MAP_WIDTH; ++i) 
	{
		if(map[i] == 2) // SHOULD CHANGE, PLAYER IS HARDCODED
		{
			switch(map[i+movement])
			{
				case 0: // Free space, we move
					map[i] = 0;
					map[i+movement] = 2;
					break;
				case 3: // We collide with enemy and automatically attack
					player->health -= enemy->attack;
					enemy->health -= player->attack;

					// We could display in the print stats player.health + 1
					// so that we don't have to check if it's zero.
					if( player->health > 200 || player->health == 0 ) 
					{	// TODO: Death of the player and game Reset
					}
					if( enemy->health > 200 || enemy->health == 0 )
					{
						map[i+movement] = 0; // enemy dissapears from map
					}
					break;
			}
			break;	// After switch we leave loop
		}
	}
}


void main(void)
{
	// PLAYER AND ENEMY ARE HARDCODED INTO MAP.
	u8 map[] = { 1, 1, 1, 1, 1, 1,
		     1, 2, 0, 0, 0, 1,
		     1, 0, 0, 0, 0, 1,
		     1, 0, 0, 0, 0, 1,
		     1, 0, 0, 0, 3, 1,
		     1, 1, 1, 1, 1, 1};

	struct Entity player = {100, 30, 15};
	struct Entity enemy  = {90 , 20, 10};

	// Let's start!
	printf("RPG GAME\r\n");
	printf("\r\n");
	printf("PRESS ENTER TO START\r\n");

	// Is enter pressed?
	while (!cpct_isKeyPressed(Key_Return)) {
		cpct_scanKeyboard();
	}

	while (1) 
	{
		// CLS
		putchar(12);

		drawMap(map);

		// Print stats
		printf("PLAYER [%d] (a%d) (d%d)\r\n", player.health, player.attack, player.defense);
		printf("ENEMY  [%d] (a%d) (d%d)\r\n", enemy.health,  enemy.attack,  enemy.defense);

		// Re scan keyboard
		cpct_scanKeyboard();

		while (!cpct_isKeyPressed(Key_A) && !cpct_isKeyPressed(Key_D) && !cpct_isKeyPressed(Key_W) &&
				!cpct_isKeyPressed(Key_S)) {
			cpct_scanKeyboard();
		}

		// I'm using WASD. MODERN GAMERS RISE UP. 
		MOVE(Key_W, -MAP_WIDTH)
		MOVE(Key_S, MAP_WIDTH)
		MOVE(Key_D, 1)
		MOVE(Key_A, -1)
	}
}
