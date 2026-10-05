var AUTOMATCHMAKER =
{
    jsonFileName: "server/automatchmaker.json",

    getConfiguration: function(autoMatchMaker)
    {
        var data = autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName);
        if (!data)
        {
            throw new Error("Unable to read " + AUTOMATCHMAKER.jsonFileName);
        }
        return JSON.parse(data);
    },

    getName: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).name;
    },

    getDescription: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).description;
    },

    getStartMmr: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).startMmr;
    },

    getState: function(autoMatchMaker)
    {
        var configuredState = AUTOMATCHMAKER.getConfiguration(autoMatchMaker).state;
        if (configuredState)
        {
            return configuredState;
        }
        return autoMatchMaker.getRunning() ? "ActiveWithSignUp" : "SignUp";
    },

    getIsSignUpChangeAllowed: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).isSignUpChangeAllowed;
    },

    onNewMatchResultData: function(autoMatchMaker, usernames, results)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        if (usernames.length === 2 && results.length === 2)
        {
            autoMatchMaker.updateMmr(usernames[0], usernames[1], config.maxMmrChange, results[0]);
        }
        else if (usernames.length === 1 && results.length === 1 && config.aiWhenNoOpponent)
        {
            autoMatchMaker.updateMmrAgainstRating(usernames[0], config.aiMmr,
                                                  config.maxMmrChange, results[0]);
        }
    },

    onCreateNewGame: function(autoMatchMaker, multiplayerMenu, players, server)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        if (!config.maps || config.maps.length === 0 || players.length === 0)
        {
            throw new Error("The ranked matchmaker needs at least one map and one player");
        }

        var map = config.maps[Math.floor(Math.random() * config.maps.length)];
        multiplayerMenu.selectMap(map.folder, map.filename);
        if (config.ruleFile)
        {
            multiplayerMenu.loadRules(config.ruleFile);
        }

        var currentMap = multiplayerMenu.getCurrentMap();
        if (!currentMap)
        {
            throw new Error("Unable to load the selected ranked map");
        }
        var playerSelection = multiplayerMenu.getPlayerSelection();
        playerSelection.setMap(currentMap);
        for (var i = 0; i < currentMap.getPlayerCount(); ++i)
        {
            var team = config.teams && config.teams.length > i ? config.teams[i] : i;
            if (i < players.length)
            {
                if (!multiplayerMenu.assignPlayerToUser(i, players[i], team))
                {
                    throw new Error("Unable to assign player " + players[i] + " to the match");
                }
            }
            else
            {
                var aiType = config.aiType === undefined ? 2 : config.aiType;
                playerSelection.forcePlayerAi(i, aiType);
                currentMap.getPlayer(i).setTeam(team);
            }
        }
    },

    onCreateNewGames: function(autoMatchMaker, server)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        if (!config.maps || config.maps.length === 0)
        {
            return;
        }
        var candidates = autoMatchMaker.getSignedUpPlayers();
        var matchedThisPass = {};
        for (var i = 0; i < candidates.length; ++i)
        {
            var player = candidates[i];
            if (matchedThisPass[player])
            {
                continue;
            }
            var opponents = autoMatchMaker.getOpponentsForPlayer(player, config.searchRangeSteps);
            var playerMmr = autoMatchMaker.getMmr(player);
            opponents.sort(function(left, right)
            {
                return Math.abs(autoMatchMaker.getMmr(left) - playerMmr) -
                       Math.abs(autoMatchMaker.getMmr(right) - playerMmr);
            });
            var history = [];
            var historyData = autoMatchMaker.getMatchHistoryData(player);
            if (historyData)
            {
                history = JSON.parse(historyData);
            }
            for (var j = 0; j < opponents.length; ++j)
            {
                var opponent = opponents[j];
                if (matchedThisPass[opponent] || history.indexOf(opponent) >= 0)
                {
                    continue;
                }
                if (autoMatchMaker.createNewGame([player, opponent], config.mods || []))
                {
                    matchedThisPass[player] = true;
                    matchedThisPass[opponent] = true;
                    var maxHistory = config.matchHistory === undefined ?
                                     10 : Math.max(0, config.matchHistory);
                    history.unshift(opponent);
                    history = history.slice(0, maxHistory);
                    autoMatchMaker.setMatchHistoryData(player, JSON.stringify(history));
                    var opponentHistoryData = autoMatchMaker.getMatchHistoryData(opponent);
                    var opponentHistory = opponentHistoryData ? JSON.parse(opponentHistoryData) : [];
                    opponentHistory.unshift(player);
                    autoMatchMaker.setMatchHistoryData(opponent, JSON.stringify(opponentHistory.slice(0, maxHistory)));
                    break;
                }
            }
            if (!matchedThisPass[player] && config.aiWhenNoOpponent &&
                autoMatchMaker.createNewGame([player], config.mods || []))
            {
                matchedThisPass[player] = true;
            }
        }
    },

    onNewPlayerData: function(autoMatchMaker, player, minGames, maxGames, server)
    {
    },

    getBracketGraphInfo: function(autoMatchMaker)
    {
        return "";
    }
};
