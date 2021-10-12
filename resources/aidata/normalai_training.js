var Init =
{
    // training setup data
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "Amber Valley.map",           // map that will be used for training
    mutationRate    = 0.1,                          // chance for a weight to mutate at random
    fogOfWar        = GameEnums.Fog_Off,            // fog of war rule for training
    maxRuns         = 4000,                         // maximum amount of iterations
    turnLimit       = 40,
    // ai's and names that will be used for training
    topAis          = 3,
    trainingAis     =  [["normal.ini",    2],
                        ["normal0.ini",   2],
                        ["normal1.ini",   2],
                        ["normal2.ini",   2],
                        ["normal3.ini",   2],
                        ["normal4.ini",   2],
                        ["normal5.ini",   2],
                        ["normal6.ini",   2],
                        ["normal7.ini",   2],
                        ["normal8.ini",   2],
                        ["normal9.ini",   2],
                        ["normal10.ini",  2],
    ],
    // internal data
    startAi = 0,
    rotationStartAi = 0,
    rotationCount = 0,
    currentMatch = [],
    matchData   = [],
    runCount    = 0,
    logLevel    = 1,
    cos = ["CO_ANDY", "CO_JESS"],
    start = false,
    main = function(menu)
    {
        if (Init.start === false)
        {
            Init.selectCos();
            Init.start = true;
        }
        if (Init.runCount < Init.maxRuns)
        {
            menu.enterSingleplayer();
        }
    },

    mapsSelection = function(menu)
    {
        GameConsole.print("Preparing next match", Init.logLevel);
        menu.selectMap(Init.trainingFolder, Init.trainingMap);
        menu.slotButtonNext();
        menu.slotButtonNext();
        var gameRules = map.getGameRules();
        gameRules.addVictoryRule("VICTORYRULE_TURNLIMIT_CAPTURE_RACE");
        var victoryRule = gameRules.getVictoryRule("VICTORYRULE_TURNLIMIT_CAPTURE_RACE");
        victoryRule.setRuleValue(Init.turnLimit, 0);
        var selection = menu.getPlayerSelection();
        var playerCount = map.getPlayerCount();
        Init.currentMatch = [];
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
            }
        }
        gameRules.setFogMode(Init.fogOfWar);
        gameRules.setRandomWeather(false);
        for (var i = 0; i < playerCount; ++i)
        {
            var playerIdx = Init.rotationCount + i;
            if (playerIdx >= playerCount)
            {
                playerIdx -= playerCount;
            }
            var aiIdx = Init.rotationStartAi + i;
            if (i == 0)
            {
                aiIdx = Init.startAi;
            }
            GameConsole.print("Using ai at index " + aiIdx + " for player " + playerIdx, Init.logLevel);
            selection.selectPlayerAi(playerIdx, Init.trainingAis[aiIdx][1]);
            GameConsole.print("Using ai-setting " + Init.trainingAis[aiIdx][0] + " for player " + playerIdx, Init.logLevel);
            Init.currentMatch.push(aiIdx);
            selection.playerCO1Changed(Init.cos[0], i);
            selection.playerCO2Changed(Init.cos[1], i);
            map.getPlayer(playerIdx).getBaseGameInput().readIni(Init.trainingAis[aiIdx][0]);
        }
        menu.startGame();
    },

    onVictory = function(menu)
    {
        if (Init.evaluateMatch())
        {
            Init.prepareNextRun();
        }
        menu.exitMenue();
    },

    evaluateMatch = function()
    {
        var playerCount = map.getPlayerCount();
        var team = map.getGameRules().getVictoryTeam();
        var nextRound = false;
        if (team >= 0)
        {
            var winnerAi = Init.currentMatch[team];
            GameConsole.print("Winning Ai is " + Init.trainingAis[winnerAi][0], Init.logLevel);
            Init.matchData[winnerAi] += 1;
        }
        Init.rotationCount += 1;
        if (Init.rotationCount === playerCount)
        {
            Init.rotationCount = 0;
            Init.rotationStartAi += 1;
            if (Init.rotationStartAi > Init.trainingAis.length - playerCount)
            {
                GameConsole.print("Going for next match up", Init.logLevel);
                Init.startAi += 1;
                Init.rotationStartAi = Init.startAi;
                if (Init.startAi > Init.trainingAis.length - playerCount)
                {
                    nextRound = true;
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
                GameConsole.print("Mutating ai: " + Init.trainingAis[i][0], Init.logLevel);
                var dummyAi = map.getPlayer(0).getBaseGameInput();
                var ai = globals.randInt(0, aiNames.length - 1);
                dummyAi.readIni(aiNames[ai]);
                dummyAi.randomizeIni(Init.trainingAis[i][0], Init.mutationRate);
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

