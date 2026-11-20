#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>
// Define TFT display pins
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI   23
#define TFT_CLK    18
#define TFT_MISO   19

// Initialize TFT display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Define button pins
#define BUTTON_UP 12
#define BUTTON_DOWN 13
#define BUTTON_LEFT 14
#define BUTTON_RIGHT 27
#define BUTTON_SHOOT 33

// Define spaceship properties
int spaceshipX = 160;
int spaceshipY = 220;
const int spaceshipWidth = 55;
const int spaceshipHeight = 41;
int wiggleOffset = 0;
bool wiggleDirection = true; // true for right, false for left
const int bulletWidth = 3;
const int bulletHeight = 7;
int playerHP = 100;
unsigned long lastShootTime = 0;
const unsigned char bitmap_spaceship [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x06, 0x7c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x7c, 0xe0, 0x00, 0x00, 
	0x00, 0x00, 0x1e, 0x7c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7c, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0xfe, 0x7c, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x1f, 0xf9, 0x01, 
	0x3f, 0xf0, 0x00, 0x00, 0x3f, 0xf9, 0xff, 0x3f, 0xf8, 0x00, 0x00, 0x3f, 0xf9, 0xc7, 0x3f, 0xf8, 
	0x00, 0x00, 0x3f, 0xf9, 0xc7, 0x3f, 0xf8, 0x00, 0x00, 0x38, 0x3e, 0x00, 0xf8, 0x38, 0x00, 0x00, 
	0x38, 0x3e, 0x10, 0xf8, 0x38, 0x00, 0x00, 0x38, 0x3e, 0x00, 0xf8, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x84, 0x00, 0x00, 0x00, 0x40, 
	0x7e, 0x01, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x3e, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 
	0xf8, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x30, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Define chicken (enemy) properties
const int chickenWidth = 37;
const int chickenHeight = 31;
const unsigned char bitmap_chicken [] PROGMEM = {
	0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x03, 
	0xfe, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0xff, 0x83, 0xfe, 0x0f, 0xf8, 0xff, 0xe1, 0xfc, 
	0x3f, 0xf8, 0x7f, 0xfe, 0xfb, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 
	0xf8, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xe0, 
	0x7f, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xff, 0x00, 0x1f, 
	0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xff, 0xf0, 0x00, 0x00, 0x3f, 
	0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 
	0x80, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0x07, 0xf8, 
	0x00, 0x00, 0xff, 0x8f, 0xf8, 0x00, 0x00, 0x7f, 0x8f, 0xf0, 0x00
};


//for stars
struct star {
  int x;
  int y;
  bool active;
};
const int NUM_STARS = 50; 
star stars[NUM_STARS];


// Define base enemy speed and speed and bullet frequency increase per score increment
int baseEnemySpeed = random(1, 4);
int bulletFrequencyDecreasePerScore = 200;

#define NUM_COLUMNS 4
#define COLUMN_WIDTH ( tft.width()/ NUM_COLUMNS)

const int columnPositions[NUM_COLUMNS] = {
  COLUMN_WIDTH / 2,
  COLUMN_WIDTH + COLUMN_WIDTH / 2,
  2 * COLUMN_WIDTH + COLUMN_WIDTH / 2,
  3 * COLUMN_WIDTH + COLUMN_WIDTH / 2
};

const int columnSpeeds[NUM_COLUMNS] = {2, 3, 4, 5}; // Different vertical speeds for each column

struct Enemy {
  int x;
  int y;
  int speedX;
  int speedY;
  bool active;
  int hitCount;
  unsigned long lastShootTime;
};

struct Bullet {
  int x;
  int y;
  bool active;
};
#define MAX_PLAYER_BULLETS 10
#define MAX_ENEMY_BULLETS 12
Bullet playerBullets[MAX_PLAYER_BULLETS];
Bullet enemyBullets[MAX_ENEMY_BULLETS];
struct PowerUp {
  int x;
  int y;
  bool active;
  int type; // 0: Double Firepower, 1: Health Restoration, 2: Temporary Shield
  unsigned long spawnTime;
};

int MAX_ENEMIES_INCREMENT = 1;
const int MAX_ENEMIES = 4;
Enemy enemies[MAX_ENEMIES];

const int MAX_POWERUPS = 3;
PowerUp powerUps[MAX_POWERUPS];

int score = 0;
bool gameOver = false;

// Power-up types
#define DOUBLE_FIREPOWER 0
#define HEALTH_RESTORATION 1
#define TEMPORARY_SHIELD 2

// Power-up durations (in milliseconds)
#define DOUBLE_FIREPOWER_DURATION 5000
#define TEMPORARY_SHIELD_DURATION 5000

// Define power-up drop chance (percentage)
#define POWER_UP_DROP_CHANCE 20

// Power-up status
bool isDoubleFirepowerActive = false;
unsigned long doubleFirepowerStartTime = 0;
bool isShieldActive = false;
unsigned long shieldStartTime = 0;

// Power-up rendering
// Double fire power bitmap
const unsigned char bitmap_doublefirepower [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 
	0x00, 0x77, 0x00, 0x00, 0x00, 0xe3, 0x80, 0x00, 0x01, 0xc9, 0xc0, 0x00, 0x03, 0x9c, 0xe0, 0x00, 
	0x07, 0x3e, 0x70, 0x00, 0x1e, 0x7f, 0x3c, 0x00, 0x3c, 0xe3, 0x9e, 0x00, 0x79, 0xc1, 0xcf, 0x00, 
	0x67, 0x80, 0xf3, 0x00, 0x4f, 0x00, 0x79, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x78, 0x00, 0x0f, 0x00, 
	0x70, 0x00, 0x07, 0x00, 0x40, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

// Health restoration bitmap
const unsigned char bitmap_health [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

// Shield bitmap
const unsigned char bitmap_shield [] PROGMEM = {
	0x3f, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 
	0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x00, 
	0x7f, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0x00, 
	0x3f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x0f, 0xff, 0xf8, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xf0, 0x00, 
	0x03, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7f, 0x00, 0x00, 
	0x00, 0x3e, 0x00, 0x00
};

#define MAX_EXPLOSIONS 10
#define EXPLOSION_FRAMES 6

struct Explosion {
  int x, y;
  int frame;
  bool active;
};

Explosion explosions[MAX_EXPLOSIONS];

const unsigned char explosionFrame1 [] PROGMEM = {
	      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x38, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x3f, 0xc0, 0x00, 
        0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 
        0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};
const unsigned char explosionFrame2 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 
        0x00, 0x7f, 0xc0, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xe0, 0x00, 
        0x00, 0xff, 0xc0, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x01, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80, 0x00, 
        0x01, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};
const unsigned char explosionFrame3 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 
        0x0f, 0xff, 0x80, 0x00, 0x0f, 0xe3, 0xc0, 0x00, 0x07, 0xc3, 0xf0, 0x00, 0x07, 0x1f, 0xfc, 0x00, 
        0x0f, 0x3f, 0xf0, 0x00, 0x0f, 0x7f, 0xe0, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xf0, 0x00, 
        0x03, 0xfb, 0xe0, 0x00, 0x03, 0xf3, 0xe0, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0xfe, 0x00, 0x00, 
        0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};
const unsigned char explosionFrame4 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x80, 0x00, 0x00, 0x41, 0x80, 0x00, 
        0x00, 0x1c, 0x00, 0x00, 0x00, 0x3e, 0x20, 0x00, 0x1c, 0x3c, 0x00, 0x00, 0x0f, 0xfe, 0x18, 0x00, 
        0x07, 0xfe, 0x39, 0x00, 0x03, 0xef, 0xf1, 0x00, 0x21, 0xc7, 0xe0, 0x00, 0x07, 0xc3, 0xe0, 0x00, 
        0x3f, 0xc3, 0xec, 0x00, 0x3f, 0x81, 0xc8, 0x00, 0x0f, 0x01, 0xf0, 0x00, 0x07, 0xc3, 0xd0, 0x00, 
        0x1f, 0xe7, 0xf0, 0x00, 0x3b, 0xff, 0x80, 0x00, 0x40, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0xc0, 0x00, 
        0x00, 0x38, 0xc0, 0x00, 0x02, 0x10, 0x40, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};
const unsigned char explosionFrame5 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 
        0x00, 0xff, 0x00, 0x00, 0x01, 0xfc, 0xf0, 0x00, 0x01, 0xfd, 0xf0, 0x00, 0x03, 0xfd, 0xf8, 0x00, 
        0x03, 0xfd, 0xf8, 0x00, 0x0d, 0xf8, 0xf8, 0x00, 0x0c, 0xc0, 0x7c, 0x00, 0x0e, 0x00, 0x3e, 0x00, 
        0x0f, 0x00, 0x1e, 0x00, 0x1f, 0x80, 0x0c, 0x00, 0x1f, 0x80, 0x7e, 0x00, 0x0f, 0x00, 0xfc, 0x00, 
        0x0f, 0x40, 0xf8, 0x00, 0x01, 0xfe, 0xf8, 0x00, 0x01, 0xff, 0x70, 0x00, 0x00, 0xff, 0xc0, 0x00, 
        0x00, 0x7f, 0xc0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};
const unsigned char explosionFrame6 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
        0x07, 0x08, 0x40, 0x00, 0x0f, 0x80, 0xe0, 0x00, 0x0f, 0x80, 0x70, 0x00, 0x0f, 0x80, 0x38, 0x00, 
        0x0f, 0x80, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x08, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x7c, 0x00, 
        0x00, 0x00, 0x7c, 0x00, 0x03, 0xc0, 0x38, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xf1, 0x80, 0x00, 
        0x00, 0xf0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00
};

const unsigned char* explosionBitmaps[EXPLOSION_FRAMES] = {
  explosionFrame1, explosionFrame2, explosionFrame3, explosionFrame4, explosionFrame5, explosionFrame6
};

const int buzzerPin = 25; //Define Buzzer
char notes[] = "c e g C "; // a space represents a rest
int frequencies[] = {262, 330, 392, 523, 0}; // corresponding frequencies
int beats[] = {1, 1, 1, 1, 1}; // duration for each note
int tempo = 150; // tempo for the song

// Function prototypes
void setup();
void loop();
void readInput();
void updateEnemies();
void updatePowerUps();
void renderSpaceship();
void renderEnemies();
void renderPowerUps();
void renderHP();
void renderScore();
void renderGameOver();
void initializeEnemies();
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void resetGame();
void spawnPowerUp(int x, int y);
void applyPowerUp(int type);
void renderExplosion();
void updateExplosion();
void initializeStars();
void renderStars();
void drawDiamond(int x, int y, uint16_t color);
void updateStars();
void beginningScenes();
void startScreen();
void updatePlayerBullets();
void updateEnemyBullets();
void renderPlayerBullets();
void renderEnemyBullets();
void spawnEnemy(int column, int speedY);
void shootPlayerBullet(int x, int y);
void shootEnemyBullet(int x, int y);

void setup() {
  Serial.begin(115200);

  // Initialize buttons
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_SHOOT, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the display
  tft.begin();
  tft.setRotation(0); 

  // Set background color
  beginningScenes();
  tft.fillScreen(ILI9341_BLACK);

  // Initialize game entities
  initializeEnemies();
  initializeStars();
}

void loop() {

  readInput();

  while (!gameOver) {
    readInput();
    updateEnemies();
    updateStars();
    updatePlayerBullets();
    updateEnemyBullets();
    updatePowerUps();
    updateExplosions();
    for (int i = 0; i < NUM_COLUMNS; i++) {
     if(!enemies[i].active){
      spawnEnemy(i, random(2, 5));
     }
   }
    

    renderStars();
    renderSpaceship();
    renderEnemies();  
    renderPlayerBullets();
    renderEnemyBullets();
    renderPowerUps();
    renderHP();
    renderScore();
    renderExplosions();
  }
  tft.fillScreen(ILI9341_BLACK);
  tone(buzzerPin, 294, 500);
  delay(100);
  tone(buzzerPin, 294, 500); // play note D (294 Hz)
  while (gameOver) {
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;
    readInput();
    renderGameOver();
    tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 60);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("Press Shoot Button to Play Again");
    tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 110);
    tft.print("Press Left Button  to stop");
    unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (digitalRead(BUTTON_SHOOT) == LOW) {
        resetGame();
        lastDebounceTime = currentTime;
      }
      if(digitalRead(BUTTON_LEFT) == LOW){
        resetGame();
          for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].active = false;
         }
        lastDebounceTime = currentTime;
        setup();
      }
    }
  }

  
}
void beginningScenes(){
  tft.fillScreen(ILI9341_BLACK);
  initializeStars();
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 500;
  while(1){
   tft.setCursor(40,20);
   tft.setTextSize(4);
   tft.setTextColor(ILI9341_WHITE);
   tft.print("CHICKEN");
   tft.setCursor(30,70);
   tft.setTextSize(4);
   tft.setTextColor(ILI9341_WHITE);
   tft.print("INVADERS");
   tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 100);
   tft.setTextColor(ILI9341_WHITE);
   tft.setTextSize(2);
   tft.print("Press Shoot Button to Start");
   updateStars();
   renderStars();
   startScreen();
   unsigned long currentTime = millis();
   if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (digitalRead(BUTTON_SHOOT) == LOW) {
        lastDebounceTime = currentTime;
        break;
      }
    }
  } 
  tft.fillScreen(ILI9341_BLACK);
  initializeStars();
  while(1){
   updateStars();
   renderStars();
   tft.setCursor(20, tft.height()/2);
   tft.setTextSize(2);
   tft.setTextColor(ST7735_RED);
   tft.print("THEY ARE COMING!!!");
   tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 100);
   tft.setTextColor(ILI9341_WHITE);
   tft.setTextSize(1);
   tft.print("Press Shoot Button to Continue");
   unsigned long currentTime = millis();
   if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (digitalRead(BUTTON_SHOOT) == LOW) {
        lastDebounceTime = currentTime;
        break;
      }
    }
  }
  tft.fillScreen(ILI9341_BLACK);
  initializeStars();
  while(1){
   updateStars();
   renderStars();
   tft.setCursor(20, tft.height()/2);
   tft.setTextSize(1);
   tft.setTextColor(ILI9341_WHITE);
   tft.print("Intergalactic chickens  have come to invade Earth to get revenge for Earth  chickens...");
   tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 100);
   tft.setTextColor(ILI9341_WHITE);
   tft.setTextSize(1);
   tft.print("Press Shoot Button to Continue");
   unsigned long currentTime = millis();
   if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (digitalRead(BUTTON_SHOOT) == LOW) {
        lastDebounceTime = currentTime;
        break;
      }
    }
  }
  tft.fillScreen(ILI9341_BLACK);
  initializeStars();
  while(1){
   updateStars();
   renderStars();
   tft.setCursor(20, tft.height()/2 - 30);
   tft.setTextSize(2);
   tft.setTextColor(ILI9341_WHITE);
   tft.print("You are our last    hope");
   tft.setCursor(20, tft.height()/2 + 20);
   tft.print("Defend Earth with   all you've got...");
   tft.setCursor(tft.width() / 2 - 100, tft.height() / 2 + 100);
   tft.setTextColor(ILI9341_WHITE);
   tft.setTextSize(1);
   tft.print("Press Shoot Button to Start");
   unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) > debounceDelay) {
      if (digitalRead(BUTTON_SHOOT) == LOW) {
        lastDebounceTime = currentTime;
        break;
      }
    }
  }
}
void startScreen(){
   if (wiggleDirection) {
    wiggleOffset++;
    if (wiggleOffset > 2) wiggleDirection = false;
  } else {
    wiggleOffset--;
  }
    if (wiggleOffset < -2) {wiggleDirection = true;}
  int spaceshipRenderX = tft.width()/2 - 18 + wiggleOffset;
  tft.fillRect(spaceshipRenderX, 205, spaceshipWidth, spaceshipHeight+2, ILI9341_BLACK);
  tft.drawBitmap(spaceshipRenderX, 205, bitmap_spaceship, spaceshipWidth, spaceshipHeight, ILI9341_WHITE);
  enemies[0].x = tft.width()/2 - 11;
  enemies[0].y =  150;
  enemies[0].speedX = 0;
  enemies[0].speedY = 0;
  enemies[0].active = true;
  enemies[0].lastShootTime = millis();
  enemies[1].x = tft.width()/2 - 11 + chickenWidth + 45;
  enemies[1].y =  120;
  enemies[1].speedX = 0;
  enemies[1].speedY = 0;
  enemies[1].active = true;
  enemies[1].lastShootTime = millis();
  enemies[2].x = tft.width()/2 - 11 - chickenWidth - 45;
  enemies[2].y =  120;
  enemies[2].speedX = 0;
  enemies[2].speedY = 0;
  enemies[2].active = true;
  enemies[2].lastShootTime = millis();
  tft.drawBitmap(enemies[1].x, enemies[1].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
  tft.drawBitmap(enemies[2].x, enemies[2].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
  tft.drawBitmap(enemies[0].x, enemies[0].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
}
void readInput() {

  // Read buttons
  if (digitalRead(BUTTON_UP) == LOW && spaceshipY > 0) {
    spaceshipY -= 4;
  }
  if (digitalRead(BUTTON_DOWN) == LOW && spaceshipY < tft.height() - spaceshipHeight) {
    spaceshipY += 4;
  }
  if (digitalRead(BUTTON_LEFT) == LOW && spaceshipX > 0) {
    spaceshipX -= 5;
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && spaceshipX < tft.width() - spaceshipWidth) {
    spaceshipX += 5;
  }
  if (digitalRead(BUTTON_SHOOT) == LOW) {
    unsigned long currentTime = millis();
    if (currentTime - lastShootTime > (isDoubleFirepowerActive ? 250 : 500)) {
      // Adjust the firing rate based on double firepower status
      // Shoot bullets
      shootPlayerBullet(spaceshipX + spaceshipWidth / 2 - bulletWidth / 2, spaceshipY - bulletHeight);
      if (isDoubleFirepowerActive) {
        shootPlayerBullet(spaceshipX + spaceshipWidth / 2 - bulletWidth / 2 - 10, spaceshipY - bulletHeight); // Second bullet for double firepower
      }
      lastShootTime = currentTime;
    }
  }
}

void updateEnemies() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (enemies[i].active) {
      tft.drawBitmap(enemies[i].x, enemies[i].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
      // Clear the old position
      tft.fillRect(enemies[i].x, enemies[i].y, chickenWidth, chickenHeight, ILI9341_BLACK);

      // Move the enemy
      enemies[i].y += enemies[i].speedY;
      tft.drawBitmap(enemies[i].x, enemies[i].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
      // Check if enemy is out of bounds
      if (enemies[i].y > tft.height()) {
        enemies[i].active = false;
        gameOver = true;
      }

      // Check for collision with player
      if (checkCollision(enemies[i].x, enemies[i].y, chickenWidth, chickenHeight, spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight)) {
       for (int j = 0; j < MAX_EXPLOSIONS; j++) {
            if (!explosions[j].active) {
              explosions[j] = { enemies[i].x, enemies[i].y, 0, true };
              break;
            }
          }
        if(!isShieldActive){
           playerHP -= 10;
          }
        if (playerHP <= 0) {
          gameOver = true;
        }
        enemies[i].active = false;
        tone(buzzerPin, 392, 150);
      }

      // Shoot bullets with increased frequency based on score
      unsigned long currentTime = millis();
      int shootInterval = random(2500, 3500) - (score / 200) * bulletFrequencyDecreasePerScore;
      if (currentTime - enemies[i].lastShootTime > shootInterval) {
        shootEnemyBullet(enemies[i].x + chickenWidth / 2 - bulletWidth / 2, enemies[i].y + chickenHeight);
        enemies[i].lastShootTime = currentTime;
      }
    }
  }
}
void updatePlayerBullets() {
  for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (playerBullets[i].active) {
      // Clear the old position
      tft.fillRect(playerBullets[i].x, playerBullets[i].y, bulletWidth, bulletHeight, ILI9341_BLACK);

      // Update the bullet position
      playerBullets[i].y -= 10;

      // Check if bullet is out of bounds
      if (playerBullets[i].y < 0 || playerBullets[i].y > tft.height()) {
        playerBullets[i].active = false;
        continue; // Skip further processing for this bullet
      }

      // Check for collision with enemies
      for (int j = 0; j < MAX_ENEMIES; j++) {
        if (enemies[j].active && checkCollision(playerBullets[i].x, playerBullets[i].y, bulletWidth, bulletHeight, enemies[j].x, enemies[j].y, chickenWidth, chickenHeight)) {
          // Enemy got hit
          enemies[j].active = false;
          playerBullets[i].active = false;
          score += 10;
          spawnPowerUp(enemies[j].x, enemies[j].y); // Spawn a power-up upon enemy defeat

          // Spawn an explosion
          for (int k = 0; k < MAX_EXPLOSIONS; k++) {
            if (!explosions[k].active) {
              explosions[k] = { enemies[j].x, enemies[j].y, 0, true };
              break;
            }
          }
         tone(buzzerPin, 392, 150);
         break; // Stop checking other enemies
        }
      }

      // Draw the new position if the bullet is still active
      if (playerBullets[i].active) {
        tft.fillRect(playerBullets[i].x, playerBullets[i].y, bulletWidth, bulletHeight, ILI9341_WHITE);
      }
    }
  }
}

void updateEnemyBullets() {
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (enemyBullets[i].active) {
      // Clear the old position
      tft.fillRect(enemyBullets[i].x, enemyBullets[i].y, bulletWidth, bulletHeight, ILI9341_BLACK);

      // Update the bullet position
      enemyBullets[i].y += 9;

      // Check if bullet is out of bounds
      if (enemyBullets[i].y < 0 || enemyBullets[i].y > tft.height()) {
        enemyBullets[i].active = false;
        continue; // Skip further processing for this bullet
      }

      // Check for collision with player
      if (checkCollision(enemyBullets[i].x, enemyBullets[i].y, bulletWidth, bulletHeight, spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight)) {
        // Player got hit
        if (!isShieldActive) {
          playerHP -= 5;
          if (playerHP <= 0) {
            gameOver = true;
          }
        }
        enemyBullets[i].active = false;
      }

      // Draw the new position if the bullet is still active
      if (enemyBullets[i].active) {
        tft.fillRect(enemyBullets[i].x, enemyBullets[i].y, bulletWidth, bulletHeight, ILI9341_RED);
      }
    }
  }
}

void updatePowerUps() {
  unsigned long currentTime = millis();
  
  // Update power-up effects
  if (isDoubleFirepowerActive && currentTime - doubleFirepowerStartTime > DOUBLE_FIREPOWER_DURATION) {
    isDoubleFirepowerActive = false;
  }

  if (isShieldActive && currentTime - shieldStartTime > TEMPORARY_SHIELD_DURATION) {
    isShieldActive = false;
  }

  // Update power-ups
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (powerUps[i].active) {
      tft.fillRect(powerUps[i].x, powerUps[i].y, 25, 25, ILI9341_BLACK);
      powerUps[i].y += 3; // Move downwards

      // Check if power-up is out of bounds
      if (powerUps[i].y > tft.height()) {
        powerUps[i].active = false;
      }

      // Check for collision with player
      if (checkCollision(powerUps[i].x, powerUps[i].y, 16, 16, spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight)) {
        applyPowerUp(powerUps[i].type);
        powerUps[i].active = false;
      }
    }
  }
}

void renderStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    if (stars[i].active) {
      drawDiamond(stars[i].x, stars[i].y, ILI9341_WHITE);
    }
  }
}

void initializeStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(tft.width());
    stars[i].y = random(tft.height());
    stars[i].active = true;
  }
}

void updateStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    if (stars[i].active) {
      // Clear the old position
      drawDiamond(stars[i].x, stars[i].y, ILI9341_BLACK);

      // Move the star downwards
      stars[i].y += 2;
      
      // Wrap the star to the top if it goes out of bounds
      if (stars[i].y > tft.height()) {
        stars[i].x = random(tft.width());
        stars[i].y = 0;
      }

      // Draw the new position
      drawDiamond(stars[i].x, stars[i].y, ILI9341_WHITE);
    }
  }
}

void drawDiamond(int x, int y, uint16_t color) {
  tft.drawPixel(x, y, color); // Center point
  tft.drawPixel(x + 1, y, color);
  tft.drawPixel(x - 1, y, color);
  tft.drawPixel(x, y + 1, color);
  tft.drawPixel(x, y - 1, color);
}
void renderSpaceship() {
  if (wiggleDirection) {
    wiggleOffset++;
    if (wiggleOffset > 2) wiggleDirection = false;
  } else {
    wiggleOffset--;
    if (wiggleOffset < -2) wiggleDirection = true;
  }
  int spaceshipRenderX = spaceshipX + wiggleOffset;
  tft.fillRect(spaceshipRenderX, spaceshipY, spaceshipWidth, spaceshipHeight+2, ILI9341_BLACK);
  tft.drawBitmap(spaceshipRenderX, spaceshipY, bitmap_spaceship, spaceshipWidth, spaceshipHeight, isShieldActive ? ILI9341_BLUE : ILI9341_WHITE);

}

void renderEnemies() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (enemies[i].active) {
      tft.drawBitmap(enemies[i].x, enemies[i].y, bitmap_chicken, chickenWidth, chickenHeight, ILI9341_RED);
    }
  }
}
void renderPlayerBullets() {
  for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (playerBullets[i].active) {
      tft.fillRect(playerBullets[i].x, playerBullets[i].y, bulletWidth, bulletHeight, ILI9341_WHITE);
    }
  }
}

void renderEnemyBullets() {
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (enemyBullets[i].active) {
      tft.fillRect(enemyBullets[i].x, enemyBullets[i].y, bulletWidth, bulletHeight, ILI9341_RED);
    }
  }
}

void renderPowerUps() {
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (powerUps[i].active) {
      const unsigned char* bitmap = nullptr;
      switch (powerUps[i].type) {
        case DOUBLE_FIREPOWER:
          bitmap = bitmap_doublefirepower;
          break;
        case HEALTH_RESTORATION:
          bitmap = bitmap_health;
          break;
        case TEMPORARY_SHIELD:
          bitmap = bitmap_shield;
          break;
      }
      if (bitmap) {
        tft.drawBitmap(powerUps[i].x, powerUps[i].y, bitmap, 25, 25, ST7735_GREEN);
      }
    }
  }
}

void renderHP() {
  tft.fillRect(10, 10, 90, 20, ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("HP: ");
  tft.print(playerHP);
}

void renderScore() {
  tft.fillRect(10, 30 , 200, 20, ILI9341_BLACK);  
  tft.setCursor(10, 30);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.print(score);
}

void renderGameOver() {
  tft.setCursor(tft.width() / 2 - 80, tft.height() / 2 - 60); 
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.print("GAME OVER");
  tft.setCursor(tft.width() / 2 - 110, tft.height() / 2 - 20); 
  tft.print("Score:");
  tft.print(score);
}

void initializeEnemies() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies[i].active = false;
  }
  // Spawn initial enemies
  for (int i = 0; i < NUM_COLUMNS; i++) {
    spawnEnemy(i, random(1, 3));
  }
}
void spawnEnemy(int column, int speedY) {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemies[i].active) {
      enemies[i].x = column * COLUMN_WIDTH + COLUMN_WIDTH / 2 - chickenWidth / 2;
      enemies[i].y = random(-20, -5); // Spawn off-screen
      enemies[i].speedX = 0;
      enemies[i].speedY = speedY;
      enemies[i].active = true;
      enemies[i].lastShootTime = millis();
      break;
    }
  }
}

void shootPlayerBullet(int x, int y) {
  for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (!playerBullets[i].active) {
      playerBullets[i].x = x;
      playerBullets[i].y = y;
      playerBullets[i].active = true;
      break;
    }
  }
}

void shootEnemyBullet(int x, int y) {
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (!enemyBullets[i].active) {
      enemyBullets[i].x = x;
      enemyBullets[i].y = y;
      enemyBullets[i].active = true;
      break;
    }
  }
}


bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void resetGame() {
  spaceshipX = 160;
  spaceshipY = 220;
  playerHP = 100;
  score = 0;
  gameOver = false;
  isDoubleFirepowerActive = false;
  isShieldActive = false;
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies[i].active = false;
  }
  for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
    playerBullets[i].active = false;
  }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    enemyBullets[i].active = false;
  }
  for (int i = 0; i < MAX_POWERUPS; i++) {
    powerUps[i].active = false;
  }
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    explosions[i].active = false;
  }
  tft.fillScreen(ILI9341_BLACK);
  initializeEnemies();
}

void spawnPowerUp(int x, int y) {
  if (random(100) < POWER_UP_DROP_CHANCE) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
      if (!powerUps[i].active) {
        powerUps[i] = { x, y, true, random(3), millis() };
        break;
      }
    }
  }
}

void applyPowerUp(int type) {
  switch (type) {
    case DOUBLE_FIREPOWER:
      isDoubleFirepowerActive = true;
      doubleFirepowerStartTime = millis();
      break;
    case HEALTH_RESTORATION:
      playerHP = min(100, playerHP + 50);
      break;
    case TEMPORARY_SHIELD:
      isShieldActive = true;
      shieldStartTime = millis();
      break;
  }
}

void updateExplosions() {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (explosions[i].active) {
     tft.fillRect(explosions[i].x, explosions[i].y, 25, 25, ILI9341_BLACK);
      explosions[i].frame++;
      if (explosions[i].frame >= EXPLOSION_FRAMES) {
        explosions[i].active = false;
      }
    }
  }
}

void renderExplosions() {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (explosions[i].active) {
      tft.fillRect(explosions[i].x, explosions[i].y, chickenWidth, chickenHeight, ILI9341_BLACK);
      tft.drawBitmap(explosions[i].x, explosions[i].y, explosionBitmaps[explosions[i].frame], 25, 25, ST7735_ORANGE);
    }
  }
}