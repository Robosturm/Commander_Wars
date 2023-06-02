var Init =
        {
    // training setup data
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "Amber Valley.map",           // map that will be used for training
    playerCount     = 2,
    mutationChance  = 0.1,                          // chance for a weight to mutate at random
    mutationRate    = 0.2,                          // how much the weight gets altered.
    fogOfWar        = GameEnums.Fog_Off,            // fog of war rule for training
    maxRuns         = 4000,                         // maximum amount of iterations
    turnLimit       = 40,
    logLevel        = 1,
    randomCos       = true,
    // ai's and names that will be used for training
    topAis          = 4,    
    trainingAis     =  [["normal0.ini",   2],
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
                        ["normal11.ini",  2],
                        ["normal12.ini",  2],
                        ["normal13.ini",  2],
                        ["normal14.ini",  2],
                        ["normal15.ini",  2],
    ],
    cores = 30, // amount of games started at the same time
    // internal data
    startAi = 0,
    rotationStartAi = 0,
    rotationCount = 0,
    matchData   = [],   // array with final score results
    battleData = [],    // matrix containing data about all matches
    runCount    = 0,
    cos = ["CO_ANDY", "CO_JESS"],
    start = false,
    coreData = [],
    // enum states for remote game triggering
    runNextBattle = 0,
    sleep = 1,
    nextRun = 2,
    main = function(menu)
    {
        menu.createRandomInis(2, "resources/aidata/normal/normal", 16);
        Init.startAllCores();
    },

    startAllCores = function()
    {
        if (Init.start === false)
        {
            Init.selectCos();
            Init.start = true;
        }
        // reset all cores
        Init.coreData = [];
        // reload match data after reset or initial run
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
            }
        }
        if (Init.battleData.length === 0)
        {
            for (var ai = 0; ai < Init.trainingAis.length; ++ai)
            {
                Init.battleData.push([]);
                for (var ai2 = 0; ai2 < Init.trainingAis.length; ++ai2)
                {
                    Init.battleData[ai].push(0);
                }
            }
        }
        for (var i = 0; i < Init.cores; ++i)
        {
            Init.coreData.push(["Core" + i.toString(), false, []]);
            Init.startRemoteGame(i);
        }
    },

    startRemoteGame = function(coreIndex)
    {
        if (Init.startAi <= Init.trainingAis.length - Init.playerCount)
        {
            GameConsole.print("Preparing next match on core " + Init.coreData[coreIndex][0], Init.logLevel);
            Init.coreData[coreIndex][1] = false;
            Init.coreData[coreIndex][2] = [];
            var map = Init.trainingFolder + Init.trainingMap;
            var script = "var Init =\n" +
                    "{\n" +
                    "main = function(menu)\n" +
                    "{\n" +
                    "menu.enterSingleplayer();\n" +
                    "},\n" +
                    "mapsSelection = function(menu)\n" +
                    "{\n" +
                    "menu.selectMap(\"" + Init.trainingFolder + "\", \"" + Init.trainingMap + "\");\n" +
                    "menu.buttonNext();\n" +
                    "menu.buttonNext();\n" +
                    "var gameRules = map.getGameRules();\n" +
                    "gameRules.addVictoryRule(\"VICTORYRULE_TURNLIMIT\");\n" +
                    "var victoryRule = gameRules.getVictoryRule(\"VICTORYRULE_TURNLIMIT\");\n" +
                    "victoryRule.setRuleValue(" + Init.turnLimit + ", 0);\n" +
                    "gameRules.setFogMode(" + Init.fogOfWar.toString() + ");\n" +
                    "gameRules.setRandomWeather(false);\n" +
                    "var selection = menu.getPlayerSelection();\n";
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
                GameConsole.print("Using ai-setting " + Init.trainingAis[aiIdx][0] + " for player " + playerIdx, Init.logLevel);
                script += "selection.selectPlayerAi(" + playerIdx.toString() + ", " + Init.trainingAis[aiIdx][1].toString() + ");\n";
                Init.coreData[coreIndex][2].push(aiIdx);
                if (Init.randomCos)
                {
                    script += "selection.playerCO1Changed(\"" + Init.cos[0] + "\", " + i.toString() + ");\n";
                    script += "selection.playerCO2Changed(\"" + Init.cos[1] + "\", " + i.toString() + ");\n";
                }
                // overwrite existing default ini
                script += "selection.getMap().getPlayer(" + playerIdx.toString() + ").getBaseGameInput().loadIni(" + Init.trainingAis[aiIdx][0] + ");\n";
            }
            script += "menu.startGame();\n" +
                    "},\n" +
                    "}";

            mainServer.startRemoteGame(script, Init.coreData[coreIndex][0]);

            Init.rotationCount += 1;
            if (Init.rotationCount === Init.playerCount)
            {
                Init.rotationCount = 0;
                Init.rotationStartAi += 1;
                if (Init.rotationStartAi > Init.trainingAis.length - Init.playerCount)
                {
                    GameConsole.print("Going for next match up", Init.logLevel);
                    Init.startAi += 1;
                    Init.rotationStartAi = Init.startAi;
                }
            }
        }
    },

    onRemoteGameFinished = function(winner, core)
    {
        var coreIndex = 0;
        for (var i = 0; i < Init.cores; ++i)
        {
            if (Init.coreData[i][0] === core)
            {
                coreIndex = i;
                break;
            }
        }
        GameConsole.print("Core " + core + " finished", Init.logLevel);
        Init.coreData[coreIndex][1] = true;
        var state = Init.evaluateMatch(winner, coreIndex);
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

    evaluateMatch = function(team, coreIndex)
    {
        var playerCount = Init.playerCount;
        var nextRound = Init.runNextBattle;
        if (team >= 0)
        {
            var winnerAi = Init.coreData[coreIndex][2][team];
            GameConsole.print("Winning Ai is " + Init.trainingAis[winnerAi][0], Init.logLevel);
            for (var i = 0; i < Init.coreData[coreIndex][2].length; ++i)
            {
                var ai = Init.coreData[coreIndex][2][i];
                if (ai !== winnerAi)
                {
                    Init.battleData[winnerAi][ai] += 1;
                }
            }
        }
        else
        {
            GameConsole.print("No player won the match", Init.logLevel);
        }

        if (Init.startAi > Init.trainingAis.length - playerCount)
        {
            nextRound = Init.nextRun;
            for (var i = 0; i < Init.cores; ++i)
            {
                if (Init.coreData[i][1] === false)
                {
                    // not all cores are idle waiting for all games to be finished
                    nextRound = Init.sleep;
                    break;
                }
            }
        }
        if (nextRound === Init.nextRun)
        {
            for (var ai = 0; ai < Init.trainingAis.length; ++ai)
            {
                for (var ai2 = 0; ai2 < Init.trainingAis.length; ++ai2)
                {
                    if (ai !== ai2)
                    {
                        var wonBattles = Init.battleData[ai][ai2];
                        if (wonBattles > 0)
                        {
                            var possibleBattles = Init.playerCount;
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
                            Init.matchData[ai] += score;
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
            GameConsole.print("AI " + i + " got a score of " + Init.matchData[i] + " points", Init.logLevel);
            var aiScore = Init.matchData[i];
            if (aiScore > 0)
            {
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
                        bestAis[lowestIndex] = [Init.trainingAis[i], aiScore];
                    }
                }
            }
        }
        var aiNames = [];
        GameConsole.print("Best selected ai's are: ", Init.logLevel);
        for (i = 0; i < bestAis.length; ++i)
        {
            aiNames.push(bestAis[i][0][0]);
            GameConsole.print(aiNames[i] + " with a score of " + bestAis[i][1] + " points", Init.logLevel);
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
                mutateCount = Init.mutate(mutateCount, i, aiNames);
            }
        }
        Init.runCount = Init.runCount + 1;
        GameConsole.print("Starting run: " + Init.runCount, Init.logLevel);
        // reset data
        if (Init.runCount < Init.maxRuns)
        {
            Init.startAi = 0;
            Init.rotationStartAi = 0;
            Init.rotationCount = 0;
            Init.matchData = [];
            Init.battleData = [];
            Init.selectCos();
        }
    },

    selectCos = function()
    {
        if (Init.randomCos)
        {
            GameConsole.print("Selecting new co's at random", Init.logLevel);
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
    },

    mutate = function(mutateCount, i, aiNames)
    {
        var ai = mutateCount % Init.topAis;
        GameConsole.print("Mutating ai: " + Init.trainingAis[i][0] + " using ai: " + aiNames[ai], Init.logLevel);
        var dummyAi = map.getPlayer(0).getBaseGameInput();
        dummyAi.readIni("resources/aidata/normal/" + aiNames[ai]);
        dummyAi.randomizeIni("resources/aidata/normal/" + Init.trainingAis[i][0], Init.mutationChance, Init.mutationRate);
        ++mutateCount;
        return mutateCount;
    },
}
