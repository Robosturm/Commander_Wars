var Init =
{
    // training setup data
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "Amber Valley.map",           // map that will be used for training
    playerCount     = 2,
    mutationChance  = 0.1,                          // chance for a weight to mutate at random
    mutationRate    = 0.2,                          // chance for a weight to mutate at random
    fogOfWar        = GameEnums.Fog_Off,            // fog of war rule for training
    maxRuns         = 4000,                         // maximum amount of iterations
    turnLimit       = 40,
    logLevel        = 1,
    // ai's and names that will be used for training
    topAis          = 4,
    trainingAis     =  [["normal1.ini",   2],
                        ["normal2.ini",   2],
                        ["normal3.ini",   2],
                        ["normal4.ini",   2],
                        ["normal5.ini",   2],
                        ["normal6.ini",   2],
                        ["normal7.ini",   2],
                        ["normal8.ini",   2],
                        ["normal9.ini",   2],
                        ["normal10.ini",  2],
                        ["normal11.ini",  2],
                        ["normal12.ini",  2],
                        ["normal13.ini",  2],
                        ["normal14.ini",  2],
                        ["normal15.ini",  2],
                        ["normal16.ini",  2],
    ],    
    cores = 10, // amount of games started at the same time
    // internal data
    startAi = 0,
    rotationStartAi = 0,
    rotationCount = 0,
    currentBattleData = [],
    matchData   = [],
    runCount    = 0,
    cos = ["CO_ANDY", "CO_JESS"],
    start = false,
    coreData = [],
    runNextBattle = 0,
    sleep = 1,
    nextRun = 2,
    main = function(menu)
    {
        if (Init.start === false)
        {
            Init.selectCos();
            Init.start = true;
        }
        Init.startAllCores();
    },

    startAllCores = function()
    {
        // reset all cores
        Init.coreData = [];
        for (var i = 0; i < Init.cores; ++i)
        {
            Init.coreData.push(["Core" + i.toString(), false]);
            Init.startRemoteGame(i);
        }
    },

    startRemoteGame = function(coreIndex)
    {
        GameConsole.print("Preparing next match", Init.logLevel);
        Init.coreData[coreIndex][1] = false;
        var map = Init.trainingFolder + Init.trainingMap;
        var script = "main = function(menu)" /
                "{" /
                "if (Init.runCount < Init.maxRuns)" /
                "{" /
                "    menu.enterSingleplayer();" /
                "}" /
                "}," /
                "mapsSelection = function(menu)" /
                "{" +
                "menu.selectMap(" + Init.trainingFolder + ", " + Init.trainingMap + ");" /
                "menu.buttonNext();" /
                "menu.buttonNext();" /
                "var gameRules = map.getGameRules();" /
                "gameRules.addVictoryRule(\"VICTORYRULE_TURNLIMIT_CAPTURE_RACE\");" /
                "var victoryRule = gameRules.getVictoryRule(\"VICTORYRULE_TURNLIMIT_CAPTURE_RACE\");" /
                "gameRules.setFogMode(" + Init.fogOfWar.toString() + ");" /
                "gameRules.setRandomWeather(false);";
        for (var i = 0; i < Init.playerCount; ++i)
        {
            var playerIdx = Init.rotationCount + i;
            if (playerIdx >= Init.playerCount)
            {
                playerIdx -= Init.playerCount;
            }
            var aiIdx = Init.rotationStartAi + i;
            if (i == 0)
            {
                aiIdx = Init.startAi;
            }
            GameConsole.print("Using ai at index " + aiIdx + " for player " + playerIdx, Init.logLevel);
            script += "selection.selectPlayerAi(" + playerIdx.toString() + ", " + Init.trainingAis[aiIdx][1].toString() + ");";
            GameConsole.print("Using ai-setting " + Init.trainingAis[aiIdx][0] + " for player " + playerIdx, Init.logLevel);

            script += "selection.playerCO1Changed(" + Init.cos[0] + ", " + i.toString() + ");";
            script += "selection.playerCO2Changed(" + Init.cos[1] + ", " + i.toString() + ");";
        }
        script += "menu.startGame();" /
                  "},";

    },

    onRemoteGameFinished = function(winner, core)
    {
        var coreIndex = 0;
        for (var i = 0; i < Init.cores; ++i)
        {
            if (Init.coreData[0][0] === core)
            {
                coreIndex = i;
                break;
            }
        }
        Init.coreData[coreIndex][1] = true;
        var state = Init.evaluateMatch(winner);
        if (state === Init.nextRun)
        {
            Init.prepareNextRun();
            Init.startAllCores();
        }
        else if (state === Init.runNextBattle)
        {
            Init.startRemoteGame(coreIndex);
        }
    },

    evaluateMatch = function(team)
    {
        var playerCount = map.getPlayerCount();
        var nextRound = Init.runNextBattle;
        if (team >= 0)
        {
            var winnerAi = Init.currentMatch[team];
            GameConsole.print("Winning Ai is " + Init.trainingAis[winnerAi][0], Init.logLevel);
            Init.currentBattleData[winnerAi] += 1;
        }
        Init.rotationCount += 1;
        if (Init.rotationCount === playerCount)
        {
            Init.rotationCount = 0;
            Init.rotationStartAi += 1;
            for (var i = 0; i < Init.currentBattleData.length; ++i)
            {
                if (Init.currentBattleData[i] > 0)
                {
                    var wonBattles = Init.currentBattleData[i];
                    var possibleBattles = Init.currentMatch.length
                    var score = 0;
                    if (wonBattles === 1)
                    {
                        score = 1;
                    }
                    else if (wonBattles > wonBattles / 2)
                    {
                        score = (wonBattles - 1) * 3;
                    }
                    else
                    {
                        score = (wonBattles - 1) * 2;
                    }
                    GameConsole.print("Won " + wonBattles + " of " + possibleBattles + " possible Battles. Score=" + score, Init.logLevel);
                    Init.matchData[i] += score;
                }
            }
            Init.currentBattleData = [];
            if (Init.rotationStartAi > Init.trainingAis.length - playerCount)
            {
                GameConsole.print("Going for next match up", Init.logLevel);                
                Init.startAi += 1;
                Init.rotationStartAi = Init.startAi;
                if (Init.startAi > Init.trainingAis.length - playerCount)
                {
                    nextRound = Init.nextRun;
                    for (var i = 0; i < Init.cores; ++i)
                    {
                        if (Init.coreData[0][1] === false)
                        {
                            // not all cores are idle waiting for all games to be finished
                            nextRound = Init.sleep;
                            break;
                        }
                    }
                }
            }
        }
        GameConsole.print("Going for next match up Rotation count: " + Init.rotationCount, Init.logLevel);
        GameConsole.print("Going for next match up start ai: " + Init.startAi, Init.logLevel);
        GameConsole.print("Going for next match up Rotation start ai: " + Init.rotationStartAi, Init.logLevel);
        return nextRound;
    },

    prepareNextRun = function()
    {
        GameConsole.print("Preparing next run", Init.logLevel);
        var bestAis = [];
        for (var i = 0; i < Init.matchData.length; ++i)
        {
            GameConsole.print("AI " + i + " won " + Init.matchData[i] + " games", Init.logLevel);
            var aiScore = Init.matchData[i];
            if (bestAis.length < Init.topAis)
            {
                bestAis.push([Init.trainingAis[i], aiScore]);
            }
            else
            {
                var lowestScore = aiScore;
                var lowestIndex = -1;
                for (var i2 = 0; i2 < bestAis.length; ++i2)
                {
                    if (bestAis[i2][1] < lowestScore)
                    {
                        lowestScore = bestAis[i2][1];
                        lowestIndex = i2;
                    }
                }
                if (lowestIndex >= 0)
                {
                    bestAis[lowestIndex] = [trainingAis[i], aiScore];
                }
            }
        }
        var aiNames = [];
        GameConsole.print("Best selected ai's are: ", Init.logLevel);
        for (i = 0; i < bestAis.length; ++i)
        {
            aiNames.push(bestAis[i][0][0]);
            GameConsole.print(aiNames[i] + " with " + bestAis[i][1] + " won matches", Init.logLevel);
        }
        var mutateCount = 0;
        for (i = 0; i < Init.trainingAis.length; ++i)
        {
            var mutate = true;
            for (var i2 = 0; i2 < aiNames.length; ++i2)
            {
                if (aiNames[i2] === Init.trainingAis[i][0])
                {
                    mutate = false;
                    break;
                }
            }
            if (mutate)
            {
                var ai = mutateCount % Init.topAis;
                GameConsole.print("Mutating ai: " + Init.trainingAis[i][0] + " using ai: " + aiNames[ai], Init.logLevel);
                var dummyAi = map.getPlayer(0).getBaseGameInput();
                dummyAi.readIni(aiNames[ai]);
                dummyAi.randomizeIni(Init.trainingAis[i][0], Init.mutationChance, Init.mutationRate);
                ++mutateCount;
            }
        }
        Init.runCount = Init.runCount + 1;
        GameConsole.print("Starting run: " + Init.runCount, Init.logLevel);
        // reset data
        Init.startAi = 0;
        Init.rotationStartAi = 0;
        Init.rotationCount = 0;
        Init.currentMatch = [];
        Init.matchData = [];
        Init.selectCos();
    },

    selectCos = function()
    {
        var cos = coSpriteManager.getCoIds();
        var index = globals.randInt(0, cos.length - 1);
        while (cos[index] === "CO_RANDOM")
        {
            index = globals.randInt(0, cos.length - 1);
        }
        Init.cos[0] = cos[index];
        var index2 = globals.randInt(0, cos.length - 1);
        while (index === index2 ||
               cos[index2] === "CO_RANDOM")
        {
            index2 = globals.randInt(0, cos.length - 1);
        }
        Init.cos[1] = cos[index2];
    }
}
