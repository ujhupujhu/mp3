#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>

// ───────────────────────────────────────────────────────────────
// PIN DEFINITIONS
// ───────────────────────────────────────────────────────────────
static const uint8_t PIN_ARDUINO_RX = 11;
static const uint8_t PIN_ARDUINO_TX = 10;
static const uint8_t PIN_JOY_X = A0;
static const uint8_t PIN_JOY_Y = A1;
static const uint8_t PIN_JOY_BTN = 2;

// ───────────────────────────────────────────────────────────────
// CONFIGURATION
// ───────────────────────────────────────────────────────────────
static const uint8_t DEFAULT_VOLUME = 15;
static const uint8_t MAX_VOLUME = 30;
static const uint8_t MIN_VOLUME = 0;

static const int16_t JOY_THRESHOLD = 300;
static const int16_t JOY_CENTER = 512;

static const uint32_t DEBOUNCE_MS = 250UL;
static const uint32_t HOLD_TRIGGER_MS = 3000UL;
static const uint32_t HOLD_REPEAT_MS = 800UL;
static const uint32_t VOLUME_INTERVAL_MS = 300UL;
static const uint32_t SCROLL_SPEED_MS = 150UL;
static const uint32_t ANIM_INTERVAL_MS = 200UL;

// Display geometry
static const uint8_t DISPLAY_W = 128;
static const uint8_t DISPLAY_H = 64;
static const uint8_t SCROLL_GAP = 40;

// Layout Y coordinates
static const uint8_t Y_TOP_BAR = 11;
static const uint8_t Y_TITLE_BASE = 27;
static const uint8_t Y_ARTIST_BASE = 40;
static const uint8_t Y_BTM_BAR = 44;
static const uint8_t Y_PROGRESS = 46;
static const uint8_t Y_BTM_TOP = 45;
static const uint8_t Y_EQ_BASE = 62;
static const uint8_t Y_VOL = 50;

// ───────────────────────────────────────────────────────────────
// OBJECTS
// ───────────────────────────────────────────────────────────────
SoftwareSerial dfSerial(PIN_ARDUINO_RX, PIN_ARDUINO_TX);
DFRobotDFPlayerMini dfPlayer;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ───────────────────────────────────────────────────────────────
// SONG DATABASE (Flash)
// ───────────────────────────────────────────────────────────────
const char s01[] PROGMEM = "Bruno Major|Nothing";
const char s02[] PROGMEM = "|Infrunami";
const char s03[] PROGMEM = "Lyn Lapid|In My Mind";
const char s04[] PROGMEM = "Joy Again|Looking Out for You";
const char s05[] PROGMEM = "|Necromancer";
const char s06[] PROGMEM = "Daniel Caesar|Toronto 2014";
const char s07[] PROGMEM = "Daniel Caesar|Japanese Denim";
const char s08[] PROGMEM = "Daniel Caesar|Better";
const char s09[] PROGMEM = "Daniel Caesar|Get You ft. Kali Uchis";
const char s10[] PROGMEM = "Mac DeMarco|No Other Heart";
const char s11[] PROGMEM = "|Neu Roses (Transgressor's)";
const char s12[] PROGMEM = "Hotel Ugly|Shut Up My Moms Calling";
const char s13[] PROGMEM = "Bruno Major|Easily";
const char s14[] PROGMEM = "Strawberry Guy|Mrs Magic";
const char s15[] PROGMEM = "|For the First Time";
const char s16[] PROGMEM = "Steve Lacy|Dark Red";
const char s17[] PROGMEM = "Frankie Cosmos|Fool";
const char s18[] PROGMEM = "Cigarettes After Sex|Sweet";
const char s19[] PROGMEM = "|Some";
const char s20[] PROGMEM = "Cigarettes After Sex|Sunsetz";
const char s21[] PROGMEM = "Malcolm Todd|Roommates";
const char s22[] PROGMEM = "Laufey|From The Start";
const char s23[] PROGMEM = "Laufey|I Wish You Love";
const char s24[] PROGMEM = "Hotel Ugly|Is There Free Breakfast";
const char s25[] PROGMEM = "Daniel Caesar|Who Knows";
const char s26[] PROGMEM = "|C U Girl";
const char s27[] PROGMEM = "|Uuuu";
const char s28[] PROGMEM = "|Moonlight on the River";
const char s29[] PROGMEM = "Her's|Harvey";
const char s30[] PROGMEM = "Malcolm Todd|Mr. Incorrect";
const char s31[] PROGMEM = "wave to earth|seasons";
const char s32[] PROGMEM = "Rex Orange County|happiness";
const char s33[] PROGMEM = "grentperez|Clementine";
const char s34[] PROGMEM = "Rocco|Baby Blue";
const char s35[] PROGMEM = "late night drive home|Stress Relief";
const char s36[] PROGMEM = "Laufey|Dreamer";
const char s37[] PROGMEM = "|Hesitating";
const char s38[] PROGMEM = "Vacations|Young";
const char s39[] PROGMEM = "Daniel Caesar|Do You Like Me";
const char s40[] PROGMEM = "The Smiths|There Is A Light...";
const char s41[] PROGMEM = "|Better in the Dark";
const char s42[] PROGMEM = "Clairo|Amoeba";
const char s43[] PROGMEM = "Mac DeMarco|Freaking Out the Nbrhd";
const char s44[] PROGMEM = "Malcolm Todd|Earrings";
const char s45[] PROGMEM = "|Streetcar";
const char s46[] PROGMEM = "MarQ|Farewell";
const char s47[] PROGMEM = "|On & Off";
const char s48[] PROGMEM = "|Dna";
const char s49[] PROGMEM = "Daniel Caesar|Sweet (Unreleased)";
const char s50[] PROGMEM = "Daniel Caesar|Sign Of The Times";
const char s51[] PROGMEM = "Laufey|Promise";
const char s52[] PROGMEM = "|Room for you";
const char s53[] PROGMEM = "Daniel Caesar|Best Part ft. H.E.R.";
const char s54[] PROGMEM = "|Nope ur too late i died";
const char s55[] PROGMEM = "Clairo|Pretty Girl";
const char s56[] PROGMEM = "Faye Webster|Kingston";
const char s57[] PROGMEM = "Vacations|Telephones";
const char s58[] PROGMEM = "|Chamber Of Reflection";
const char s59[] PROGMEM = "|I Thought I Saw Your Face";
const char s60[] PROGMEM = "Clairo|Bubble Gum";
const char s61[] PROGMEM = "Rex Orange County|Pluto Projector";
const char s62[] PROGMEM = "Daniel Caesar|Buyer's Remorse";
const char s63[] PROGMEM = "Malcolm Todd|Sweet Boy";
const char s64[] PROGMEM = "|Loose";
const char s65[] PROGMEM = "grentperez|Cherry Wine";
const char s66[] PROGMEM = "Rex Orange County|Corduroy Dreams";
const char s67[] PROGMEM = "grentperez|Silver Lining";
const char s68[] PROGMEM = "Rex Orange County|Sunflower";
const char s69[] PROGMEM = "TV Girl|Lovers Rock";
const char s70[] PROGMEM = "Laufey|Lover Girl";
const char s71[] PROGMEM = "Laufey|Best Friend";
const char s72[] PROGMEM = "Rex Orange County|Best Friend";
const char s73[] PROGMEM = "|i just want to hold her";
const char s74[] PROGMEM = "boy pablo|Everytime";
const char s75[] PROGMEM = "Rex Orange County|THE SHADE";
const char s76[] PROGMEM = "Laufey|Valentine";
const char s77[] PROGMEM = "Laufey|Falling Behind";
const char s78[] PROGMEM = "Rex Orange County|Television/SFSG";
const char s79[] PROGMEM = "Joji|SLOW DANCING IN THE DARK";
const char s80[] PROGMEM = "Yot Club|YKWIM?";
const char s81[] PROGMEM = "SunKissed Lola|Pasilyo";
const char s82[] PROGMEM = "Cigarettes After Sex|K.";
const char s83[] PROGMEM = "CUCO|Lover Is a Day";
const char s84[] PROGMEM = "Laufey|Let You Break My Heart";
const char s85[] PROGMEM = "Steve Lacy|Bad Habit";
const char s86[] PROGMEM = "Eyedress & Dent May|Something About You";

const char* const songTable[] PROGMEM = {
  s01,s02,s03,s04,s05,s06,s07,s08,s09,s10,
  s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,
  s21,s22,s23,s24,s25,s26,s27,s28,s29,s30,
  s31,s32,s33,s34,s35,s36,s37,s38,s39,s40,
  s41,s42,s43,s44,s45,s46,s47,s48,s49,s50,
  s51,s52,s53,s54,s55,s56,s57,s58,s59,s60,
  s61,s62,s63,s64,s65,s66,s67,s68,s69,s70,
  s71,s72,s73,s74,s75,s76,s77,s78,s79,s80,
  s81,s82,s83,s84,s85,s86
};

static const uint8_t TOTAL_TRACKS = sizeof(songTable) / sizeof(songTable[0]);

// ───────────────────────────────────────────────────────────────
// EQUALISER ANIMATION FRAMES
// ───────────────────────────────────────────────────────────────
const uint8_t eqBars[8][5] PROGMEM = {
  { 4, 8, 12, 6, 10},
  { 8, 12, 6, 10, 4},
  {12, 6, 10, 4, 8},
  { 6, 10, 4, 8, 12},
  {10, 4, 8, 12, 6},
  { 6, 12, 10, 4, 8},
  { 8, 6, 4, 12, 10},
  { 4, 10, 8, 6, 12}
};

// ───────────────────────────────────────────────────────────────
// STATE
// ───────────────────────────────────────────────────────────────

// Playback
uint8_t currentTrack = 1;
uint8_t currentVolume = DEFAULT_VOLUME;
bool isPlaying = false;
bool isPaused = false;

// Joystick RIGHT
bool joyRightActive = false;
uint32_t joyRightStartMs = 0;
bool joyRightHoldFired = false;
uint32_t joyRightLastRepeat = 0;

// Joystick LEFT
bool joyLeftActive = false;
uint32_t joyLeftStartMs = 0;
bool joyLeftHoldFired = false;
uint32_t joyLeftLastRepeat = 0;

// Shared debounce
uint32_t lastTrackChangeMs = 0;

// Volume rate-limit
uint32_t lastVolumeMs = 0;

// Button debounce
bool lastBtnState = HIGH;
uint32_t lastBtnPressMs = 0;

// Scroll
int16_t scrollTitle = 0;
int16_t scrollArtist = 0;
uint32_t lastScrollMs = 0;
bool titleScrolls = false;
bool artistScrolls = false;
int16_t titleW = 0;
int16_t artistW = 0;

// Equaliser animation
uint8_t animFrame = 0;
uint32_t lastAnimMs = 0;

// String buffers
char rawBuf [64];
char artistBuf[32];
char titleBuf [36];

// Progress dot grid
static const uint8_t PROGRESS_DOTS = 31;
static const uint8_t PROGRESS_START = 4;
static const uint8_t PROGRESS_STEP = 4;

// ───────────────────────────────────────────────────────────────
// LOAD SONG INFO FROM FLASH
// ───────────────────────────────────────────────────────────────
void loadSongInfo(uint8_t trackNum)
{
  artistBuf[0] = '\0';
  titleBuf [0] = '\0';
  rawBuf   [0] = '\0';

  const char* flashPtr =
    (const char*)pgm_read_word(&songTable[trackNum - 1]);

  strncpy_P(rawBuf, flashPtr, sizeof(rawBuf) - 1);
  rawBuf[sizeof(rawBuf) - 1] = '\0';

  char* sep = strchr(rawBuf, '|');
  if (sep) {
    *sep = '\0';
    strncpy(artistBuf, rawBuf,  sizeof(artistBuf) - 1);
    artistBuf[sizeof(artistBuf) - 1] = '\0';
    strncpy(titleBuf,  sep + 1, sizeof(titleBuf)  - 1);
    titleBuf [sizeof(titleBuf)  - 1] = '\0';
  } else {
    strncpy(titleBuf, rawBuf, sizeof(titleBuf) - 1);
    titleBuf[sizeof(titleBuf) - 1] = '\0';
  }
}

// ───────────────────────────────────────────────────────────────
// RESET SCROLL STATE
// ───────────────────────────────────────────────────────────────
void resetScroll()
{
  scrollTitle  = 0;
  scrollArtist = 0;
  lastScrollMs = millis();

  u8g2.setFont(u8g2_font_7x14B_tr);
  titleW       = (int16_t)u8g2.getStrWidth(titleBuf);
  titleScrolls = (titleW > (int16_t)(DISPLAY_W - 4));

  u8g2.setFont(u8g2_font_6x10_tr);
  artistW       = (int16_t)u8g2.getStrWidth(artistBuf);
  artistScrolls = (artistBuf[0] != '\0') &&
                  (artistW > (int16_t)(DISPLAY_W - 4));
}

// ───────────────────────────────────────────────────────────────
// PLAYBACK CONTROL
// ───────────────────────────────────────────────────────────────
void playTrack(uint8_t track)
{
  currentTrack = track;
  dfPlayer.playMp3Folder(currentTrack);
  isPlaying = true;
  isPaused  = false;
  animFrame = 0;
  lastAnimMs = millis();

  loadSongInfo(currentTrack);
  resetScroll();

  Serial.print(F("Playing #"));
  Serial.print((int)currentTrack);
  Serial.print(F("  "));
  if (artistBuf[0] != '\0') {
    Serial.print(artistBuf);
    Serial.print(F(" - "));
  }
  Serial.println(titleBuf);
}

void nextTrack()
{
  uint8_t n = (currentTrack >= TOTAL_TRACKS) ? 1 : currentTrack + 1;
  playTrack(n);
}

void prevTrack()
{
  uint8_t p = (currentTrack <= 1) ? TOTAL_TRACKS : currentTrack - 1;
  playTrack(p);
}

void togglePause()
{
  if (isPaused) {
    dfPlayer.start();
    isPaused  = false;
    isPlaying = true;
    lastAnimMs = millis();
    Serial.println(F("Resumed"));
  } else {
    dfPlayer.pause();
    isPaused  = true;
    isPlaying = false;
    Serial.println(F("Paused"));
  }
}

void adjustVolume(int8_t delta)
{
  int16_t v = (int16_t)currentVolume + (int16_t)delta;
  if (v < (int16_t)MIN_VOLUME) v = (int16_t)MIN_VOLUME;
  if (v > (int16_t)MAX_VOLUME) v = (int16_t)MAX_VOLUME;
  if ((uint8_t)v != currentVolume) {
    currentVolume = (uint8_t)v;
    dfPlayer.volume(currentVolume);
    Serial.print(F("Volume: "));
    Serial.println((int)currentVolume);
  }
}

// ───────────────────────────────────────────────────────────────
// DRAWING HELPERS
// ───────────────────────────────────────────────────────────────
static void drawCentred(const char* str, int16_t strW, uint8_t baselineY)
{
  int16_t x = ((int16_t)DISPLAY_W - strW) / 2;
  if (x < 0) x = 0;
  u8g2.drawStr((u8g2_uint_t)x, baselineY, str);
}

static void drawScrolling(const char* str,
                          int16_t strW,
                          int16_t scrollPos,
                          uint8_t baselineY,
                          uint8_t clipY0,
                          uint8_t clipY1)
{
  int16_t x    = -scrollPos;
  int16_t loopW = strW + (int16_t)SCROLL_GAP;

  u8g2.setClipWindow(0, clipY0, DISPLAY_W - 1, clipY1);
  u8g2.drawStr(x,          baselineY, str);
  u8g2.drawStr(x + loopW,  baselineY, str);
  u8g2.setMaxClipWindow();
}

static void drawEqualizer(uint8_t x, uint8_t baseY)
{
  uint8_t maxH = (baseY > Y_BTM_TOP) ? (baseY - Y_BTM_TOP) : 1;

  if (!isPlaying || isPaused) {
    for (uint8_t i = 0; i < 5; i++) {
      u8g2.drawBox(x + i * 5, baseY - 2, 3, 2);
    }
    return;
  }

  for (uint8_t i = 0; i < 5; i++) {
    uint8_t h = pgm_read_byte(&eqBars[animFrame][i]);
    if (h > maxH) h = maxH;
    if (h == 0)   h = 1;
    u8g2.drawBox(x + i * 5, baseY - h, 3, h);
  }
}

static void drawVolumeBar(uint8_t x, uint8_t y)
{
  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.drawStr(x, y + 7, (currentVolume == 0) ? "M" : "V");

  const uint8_t BAR_X = x + 8;
  const uint8_t BAR_W = 46;
  const uint8_t BAR_H = 5;

  u8g2.drawFrame(BAR_X, y + 2, BAR_W, BAR_H);

  uint8_t fill = (uint8_t)map((long)currentVolume,
                              (long)MIN_VOLUME,
                              (long)MAX_VOLUME,
                              0L,
                              (long)(BAR_W - 2));
  if (fill > 0) {
    u8g2.drawBox(BAR_X + 1, y + 3, fill, BAR_H - 2);
  }

  char buf[4];
  buf[0] = '0' + (currentVolume / 10);
  buf[1] = '0' + (currentVolume % 10);
  buf[2] = '\0';
  u8g2.drawStr(BAR_X + BAR_W + 2, y + 7, buf);
}

static void drawPlayPauseIcon(uint8_t x, uint8_t y)
{
  if (isPlaying && !isPaused) {
    u8g2.drawBox(x,     y, 3, 10);
    u8g2.drawBox(x + 5, y, 3, 10);
  } else {
    for (uint8_t row = 0; row < 10; row++) {
      uint8_t halfH = 5;
      int8_t  dist  = (int8_t)row - (int8_t)halfH;
      if (dist < 0) dist = -dist;
      uint8_t w = (uint8_t)(halfH - dist + 1);
      if (w == 0) w = 1;
      u8g2.drawHLine(x, y + row, w);
    }
  }
}

static void drawProgressBar(uint8_t y)
{
  uint8_t activeIdx = (uint8_t)map((long)(currentTrack - 1),
                                   0L,
                                   (long)(TOTAL_TRACKS - 1),
                                   0L,
                                   (long)(PROGRESS_DOTS - 1));

  for (uint8_t i = 0; i < PROGRESS_DOTS; i++) {
    uint8_t px = PROGRESS_START + i * PROGRESS_STEP;
    if (i == activeIdx) {
      u8g2.drawBox(px, y, 2, 2);
    } else {
      u8g2.drawPixel(px, y + 1);
    }
  }
}

static void drawTrackNumber(uint8_t x, uint8_t y)
{
  char    buf[8];
  uint8_t i = 0;

  if (currentTrack >= 10) {
    buf[i++] = '0' + (currentTrack / 10);
  }
  buf[i++] = '0' + (currentTrack % 10);
  buf[i++] = '/';
  if (TOTAL_TRACKS >= 10) {
    buf[i++] = '0' + (TOTAL_TRACKS / 10);
  }
  buf[i++] = '0' + (TOTAL_TRACKS % 10);
  buf[i]   = '\0';

  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.drawStr(x, y, buf);
}

// ───────────────────────────────────────────────────────────────
// FULL DISPLAY RENDER
// ───────────────────────────────────────────────────────────────
void drawDisplay()
{
  u8g2.firstPage();
  do {
    // Top strip
    u8g2.setFont(u8g2_font_5x7_tr);
    drawTrackNumber(2, 8);
    drawPlayPauseIcon(112, 1);
    u8g2.drawHLine(0, Y_TOP_BAR, DISPLAY_W);

    // Song title
    u8g2.setFont(u8g2_font_7x14B_tr);
    if (titleScrolls) {
      drawScrolling(titleBuf, titleW, scrollTitle, Y_TITLE_BASE, 12, 29);
    } else {
      drawCentred(titleBuf, titleW, Y_TITLE_BASE);
    }

    // Artist name
    if (artistBuf[0] != '\0') {
      u8g2.setFont(u8g2_font_6x10_tr);
      if (artistScrolls) {
        drawScrolling(artistBuf, artistW, scrollArtist, Y_ARTIST_BASE, 30, 43);
      } else {
        drawCentred(artistBuf, artistW, Y_ARTIST_BASE);
      }
    }

    // Bottom strip
    u8g2.drawHLine(0, Y_BTM_BAR, DISPLAY_W);
    drawProgressBar(Y_PROGRESS);
    drawEqualizer(4,  Y_EQ_BASE);
    drawVolumeBar(34, Y_VOL);

  } while (u8g2.nextPage());
}

// ───────────────────────────────────────────────────────────────
// JOYSTICK HANDLER
// ───────────────────────────────────────────────────────────────
void handleJoystick()
{
  const uint32_t now  = millis();
  const int      joyX = analogRead(PIN_JOY_X);
  const int      joyY = analogRead(PIN_JOY_Y);

  const bool goRight = (joyX > JOY_CENTER + JOY_THRESHOLD);
  const bool goLeft  = (joyX < JOY_CENTER - JOY_THRESHOLD);
  const bool goUp    = (joyY < JOY_CENTER - JOY_THRESHOLD);
  const bool goDown  = (joyY > JOY_CENTER + JOY_THRESHOLD);

  // RIGHT → Next track
  if (goRight) {
    if (!joyRightActive) {
      joyRightActive    = true;
      joyRightStartMs   = now;
      joyRightHoldFired = false;
      if (now - lastTrackChangeMs >= DEBOUNCE_MS) {
        nextTrack();
        lastTrackChangeMs = now;
      }
    } else if (!joyRightHoldFired &&
               (now - joyRightStartMs >= HOLD_TRIGGER_MS)) {
      joyRightHoldFired  = true;
      joyRightLastRepeat = now;
      nextTrack();
      lastTrackChangeMs  = now;
    } else if (joyRightHoldFired &&
               (now - joyRightLastRepeat >= HOLD_REPEAT_MS)) {
      joyRightLastRepeat = now;
      nextTrack();
      lastTrackChangeMs  = now;
    }
  } else {
    joyRightActive    = false;
    joyRightHoldFired = false;
  }

  // LEFT → Previous track
  if (goLeft) {
    if (!joyLeftActive) {
      joyLeftActive     = true;
      joyLeftStartMs    = now;
      joyLeftHoldFired  = false;
      if (now - lastTrackChangeMs >= DEBOUNCE_MS) {
        prevTrack();
        lastTrackChangeMs = now;
      }
    } else if (!joyLeftHoldFired &&
               (now - joyLeftStartMs >= HOLD_TRIGGER_MS)) {
      joyLeftHoldFired  = true;
      joyLeftLastRepeat = now;
      prevTrack();
      lastTrackChangeMs = now;
    } else if (joyLeftHoldFired &&
               (now - joyLeftLastRepeat >= HOLD_REPEAT_MS)) {
      joyLeftLastRepeat = now;
      prevTrack();
      lastTrackChangeMs = now;
    }
  } else {
    joyLeftActive    = false;
    joyLeftHoldFired = false;
  }

  // UP / DOWN → Volume
  if (now - lastVolumeMs >= VOLUME_INTERVAL_MS) {
    if (goUp) {
      adjustVolume(+1);
      lastVolumeMs = now;
    } else if (goDown) {
      adjustVolume(-1);
      lastVolumeMs = now;
    }
  }

  // Button → Play / Pause
  bool btnNow = digitalRead(PIN_JOY_BTN);
  if (btnNow == LOW && lastBtnState == HIGH) {
    if (now - lastBtnPressMs >= DEBOUNCE_MS) {
      togglePause();
      lastBtnPressMs = now;
    }
  }
  lastBtnState = btnNow;
}

// ───────────────────────────────────────────────────────────────
// SCROLL UPDATE
// ───────────────────────────────────────────────────────────────
void updateScroll()
{
  const uint32_t now = millis();
  if (now - lastScrollMs < SCROLL_SPEED_MS) return;
  lastScrollMs = now;

  if (titleScrolls) {
    scrollTitle++;
    if (scrollTitle >= titleW + (int16_t)SCROLL_GAP) {
      scrollTitle = 0;
    }
  }

  if (artistScrolls) {
    scrollArtist++;
    if (scrollArtist >= artistW + (int16_t)SCROLL_GAP) {
      scrollArtist = 0;
    }
  }
}

// ───────────────────────────────────────────────────────────────
// ANIMATION UPDATE
// ───────────────────────────────────────────────────────────────
void updateAnimation()
{
  const uint32_t now = millis();
  if (isPlaying && !isPaused &&
      (now - lastAnimMs >= ANIM_INTERVAL_MS)) {
    lastAnimMs = now;
    animFrame  = (animFrame + 1) % 8;
  }
}

// ───────────────────────────────────────────────────────────────
// STARTUP SPLASH SCREEN
// ───────────────────────────────────────────────────────────────
void showStartupScreen()
{
  for (uint8_t f = 0; f < 32; f++) {
    u8g2.firstPage();
    do {
      uint8_t bW = (uint8_t)map((long)f, 0L, 31L, 4L, 120L);
      uint8_t bH = (uint8_t)map((long)f, 0L, 31L, 4L, 22L);
      uint8_t bX = (DISPLAY_W - bW) / 2;
      uint8_t bY = (DISPLAY_H - bH) / 2 - 6;

      u8g2.drawRFrame(bX, bY, bW, bH, 3);

      if (f > 10) {
        u8g2.setFont(u8g2_font_7x14B_tr);
        const char* hdr = "MP3 Player";
        int16_t hw = (int16_t)u8g2.getStrWidth(hdr);
        u8g2.drawStr((DISPLAY_W - hw) / 2, bY + 16, hdr);
      }

      if (f > 20) {
        u8g2.setFont(u8g2_font_5x7_tr);
        uint8_t pct = (uint8_t)map((long)f, 20L, 31L, 0L, 100L);

        char    lb[18];
        uint8_t i = 0;
        const char prefix[] = "Loading... ";
        for (uint8_t p = 0; prefix[p]; p++) {
          lb[i++] = prefix[p];
        }
        if (pct >= 100) {
          lb[i++] = '1';
          lb[i++] = '0';
          lb[i++] = '0';
        } else if (pct >= 10) {
          lb[i++] = '0' + (pct / 10);
          lb[i++] = '0' + (pct % 10);
        } else {
          lb[i++] = '0' + pct;
        }
        lb[i++] = '%';
        lb[i]   = '\0';

        int16_t lw = (int16_t)u8g2.getStrWidth(lb);
        u8g2.drawStr((DISPLAY_W - lw) / 2, 56, lb);

        u8g2.drawFrame(24, 45, 80, 6);
        u8g2.drawBox(25, 46,
                     (uint8_t)map((long)f, 20L, 31L, 0L, 78L), 4);
      }
    } while (u8g2.nextPage());

    delay(60);
  }
  delay(300);
}

// ───────────────────────────────────────────────────────────────
// SETUP
// ───────────────────────────────────────────────────────────────
void setup()
{
  pinMode(PIN_JOY_BTN, INPUT_PULLUP);
  Serial.begin(9600);

  u8g2.begin();
  u8g2.setContrast(200);
  showStartupScreen();

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(8,  30, "Connecting DFPlayer");
    u8g2.drawStr(28, 45, "Please wait...");
  } while (u8g2.nextPage());

  dfSerial.begin(9600);
  delay(1000);

  uint8_t retries = 0;
  while (!dfPlayer.begin(dfSerial, false, true)) {
    retries++;

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x10_tr);
      char    msg[20];
      uint8_t i = 0;
      const char pfx[] = "Retry ";
      for (uint8_t p = 0; pfx[p]; p++) {
        msg[i++] = pfx[p];
      }
      if (retries >= 10) {
        msg[i++] = '1';
        msg[i++] = '0';
      } else {
        msg[i++] = '0' + retries;
      }
      msg[i++] = ' ';
      msg[i++] = '/';
      msg[i++] = ' ';
      msg[i++] = '1';
      msg[i++] = '0';
      msg[i++] = '.';
      msg[i++] = '.';
      msg[i++] = '.';
      msg[i]   = '\0';
      u8g2.drawStr(18, 35, msg);
    } while (u8g2.nextPage());

    Serial.print(F("DFPlayer retry "));
    Serial.println((int)retries);
    delay(1000);

    if (retries >= 10) {
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(10, 20, "DFPlayer FAILED");
        u8g2.drawStr(10, 34, "Check wiring &");
        u8g2.drawStr(10, 48, "SD card format.");
      } while (u8g2.nextPage());

      Serial.println(F("DFPlayer FAILED. Halting."));
      while (true) {}
    }
  }

  dfPlayer.setTimeOut(500);
  dfPlayer.volume(currentVolume);

  // ── FIXED: corrected constant names ──────────────────────────
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);           // was DFROBOT_EQ_NORMAL
  dfPlayer.outputDevice(DFPLAYER_DEVICE_SD); // was DFROBOT_DEVICE_SD
  // ─────────────────────────────────────────────────────────────

  delay(200);

  playTrack(1);
  Serial.println(F("MP3 Player ready."));
}

// ───────────────────────────────────────────────────────────────
// MAIN LOOP
// ───────────────────────────────────────────────────────────────
void loop()
{
  handleJoystick();
  updateScroll();
  updateAnimation();

  if (dfPlayer.available()) {
    uint8_t msgType = dfPlayer.readType();
    if (msgType == DFPlayerPlayFinished && !isPaused) {
      if (millis() - lastTrackChangeMs >= DEBOUNCE_MS) {
        lastTrackChangeMs = millis();
        nextTrack();
      }
    }
  }

  drawDisplay();
}