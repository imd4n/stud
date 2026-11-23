/////////////////////
// Общие настройки //
////////////////////
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

////////////////////////
// Настройки самолета //
////////////////////////
const int PLANE_HEALTH = 100;
const int planeX0 = playAreaLeft + (playAreaRight - playAreaLeft) / 2;
const int planeY0 = playAreaBottom- 150;

/////////////////////
// Настройки башен //
/////////////////////
const int TOWERS_NUM = 5;
// Теперь задана в настройках башен
//const int TOWER_SPEED = 2; // Скорость движения башен вниз (пикселей за кадр)
const int MIN_SPAWN_INTERVAL =
    2000; // Минимальное время между спавном башен (мс)
const int MAX_SPAWN_INTERVAL =
    5000; // Максимальное время между спавном башен (мс)


const int MAGIC_DURATION_MS = 10000; // длительность волшебства (мс)
const int STAR_SPAWN_INTERVAL =
    5000; // интервал между появлением звёздочек (мс)