/////////////////////
// Общие настройки //
/////////////////////
const int GAME_FPS = 30;

////////////////////////////////////
// Настройки игрового окна и поля //
////////////////////////////////////
const int windowX = 50;
const int windowY = 50;
const int windowH = 980;
const int windowW = 940;
const int playAreaTop = 0;
const int playAreaBottom = windowH;
const int playAreaLeft = 0;
const int playAreaRight = windowW;

//////////////////////////
// Настройки самолёта   //
//////////////////////////
const int planeX0 = playAreaLeft + (playAreaRight - playAreaLeft) / 2;

//////////////////////
// Настройки домов  //
//////////////////////
const int TOWERS_NUM = 5;

const int MAGIC_DURATION_MS = 5000; // длительность бессмертия (мс)
