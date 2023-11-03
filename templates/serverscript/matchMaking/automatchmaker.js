var AUTOMATCHMAKER =
{
    jsonFileName = "server/automatchmaker.json",
    getName = function(autoMatchMaker, server)
    {
        var obj = JSON.parse(autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName));
        return obj.name;
    },

    getStartMmr = function(autoMatchMaker, server)
    {
        var obj = JSON.parse(autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName));
        return obj.startMmr;
    },

    getDescription = function(autoMatchMaker, server)
    {
        var obj = JSON.parse(autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName));
        return obj.description;
    },

    getIsSignUpChangeAllowed = function(autoMatchMaker, server)
    {
        var obj = JSON.parse(autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName));
        return obj.isSignUpChangeAllowed;
    },

    onNewMatchResultData = function(autoMatchMaker, usernames, results, server)
    {        
        var obj = JSON.parse(autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName));
        autoMatchMaker.updateMmr(usernames[0], usernames[1], obj.maxMmrChange, results[0])
        for (var i = 0; i < usernames.length; ++i)
        {
            AUTOMATCHMAKER.createNewMatchForPlayer(autoMatchMaker, usernames[i], server);
        }        
    },

    onCreateNewGame = function(autoMatchMaker, multiplayerMenu, players, server)
    {
        // todo
    },

    createNewMatchForPlayer = function(autoMatchMaker, username, server)
    {

    },

    onNewPlayerData = function(autoMatchMaker, player, minGames, maxGames, server)
    {
        for (var i = 0; i < minGames; ++i)
        {
            AUTOMATCHMAKER.createNewMatchForPlayer(autoMatchMaker, player, server);
        }
    },

    getBracketGraphInfo = function(autoMatchMaker, server)
    {
        // JSON.stringify()
        return "";
    },    
}
