#ifndef NETWORKCOMMANDS_H
#define NETWORKCOMMANDS_H

#include "qstring.h"

namespace NetworkCommands
{
    /**
     * @brief MAPINFO map info of from the host
     */
    static const QString MAPINFO = "MAPINFO";
    /**
     * @brief REQUESTRULE request the game rules from the host
     */
    static const QString REQUESTRULE = "REQUESTRULE";
    /**
     * @brief RANDOMMAPIDENTIFIER
     */
    static const QString RANDOMMAPIDENTIFIER = "????";
    /**
     * @brief SENDINITUPDATE initial update from the host containing the game rules and all player data
     */
    static const QString SENDINITUPDATE = "SENDINITUPDATE";
    /**
     * @brief REQUESTMAP the client daoesn't own the map request it from the host
     */
    static const QString REQUESTMAP = "REQUESTMAP";
    /**
     * @brief MAPDATA the map from the host either as savegame or as map file
     */
    static const QString MAPDATA = "MAPDATA";
    /**
     * @brief INITGAME initializes the game
     */
    static const QString INITGAME = "INITGAME";
    /**
     * @brief CLIENTREADY the client is ready to start the game
     */
    static const QString CLIENTREADY = "CLIENTREADY";
    /**
     * @brief PLAYERDATA contains the complete player as stream data
     */
    static const QString PLAYERDATA = "PLAYERDATA";
    /**
     * @brief COLORDATA the color of the selected player
     */
    static const QString COLORDATA = "COLORDATA";
    /**
     * @brief CODATA the selected co's of a player
     */
    static const QString CODATA = "CODATA";
    /**
     * @brief PLAYERCHANGED the ai owner of a player has changed
     */
    static const QString PLAYERCHANGED = "PLAYERCHANGED";
    /**
     * @brief REQUESTPLAYER request a player from the host
     */
    static const QString REQUESTPLAYER = "REQUESTPLAYER";
    /**
     * @brief SERVERREADY the host is ready to start the game
     */
    static const QString SERVERREADY = "SERVERREADY";
    /**
     * @brief CLIENTINITGAME the client has initialized the game
     */
    static const QString CLIENTINITGAME = "CLIENTINITGAME";
    /**
     * @brief STARTGAME the host has started the game. the client is allowed to start the game as well.
     */
    static const QString STARTGAME = "STARTGAME";
}

#endif // NETWORKCOMMANDS_H
