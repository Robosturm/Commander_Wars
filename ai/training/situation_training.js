var Init =
{
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "Amber Valley.map",           // map that will be used for training
    mutationChance  = 0.1,                          // chance for a weight to mutate at random
    mutationRate    = 0.2,                          // how much the weight gets altered.
    fogOfWar        = GameEnums.Fog_Off,            // fog of war rule for training
    maxRuns         = 100000,                       // maximum amount of iterations
    // ai's and names that will be used for training
    topAis          = 4,
    trainingAis     =  [["heavy_ai",    5],
                        ["heavy_ai1",   6],
                        ["heavy_ai2",   7],
                        ["heavy_ai3",   8],
                        ["heavy_ai4",   9],
                        ["heavy_ai5",   10],
                        //                        ["heavy_ai6",   11],
                        //                        ["heavy_ai7",   12],
                        //                        ["heavy_ai8",   13],
                        //                        ["heavy_ai9",   14],
    ],

    // internal data
    startAi = 0,
    matchData   = [],   // array with final score results
    start = false,
    coreData = [],
    rotationCount = 0,

    main = function(menu)
    {
        menu.enterSingleplayer();
    },

    gameMenu = function(menu)
    {
        menu.exitGame();
    },

    onVictory = function(menu)
    {
        Init.startAllCores();
    },

    startAllCores = function()
    {
        // reset all cores
        Init.coreData = [];
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
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

    },

    onRemoteGameFinished = function(winner, core)
    {

    },

    evaluateMatch = function(team, coreIndex)
    {

    },

    prepareNextRun = function()
    {

    },

    mutate = function(mutateCount, i, aiNames)
    {

    },
}
