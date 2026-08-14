// The engine opens this file fresh and calls MapStart.gameStart(map) at
// every new-game start (gamemap.cpp, GameMap::startGame), after all script
// loading; no other mod can stomp it because each mod's mapstart.js is
// opened immediately before its own call.
var MapStart =
{
    gameStart: function(map)
    {
        DEBUG_MENU_INSTALL.onGameStart(map);
    }
};
