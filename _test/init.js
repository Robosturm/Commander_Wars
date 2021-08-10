var Init =
{
    step = 0,
    steps = ["creditsTest",
             "achievementTest",
             "optionTest",
             "wikiTest",
             "shopTest",
             "coStyleTest",
             "mapEditorTest",
             "simpleAiTest",
            ],
    aiTest = false,
    main = function(menu)
    {
        var current = Init.step;
        if (current < Init.steps.length)
        {
            ++Init.step;
            Init[steps[current]](menu);
        }
        else
        {
            menu.quitGame();
        }
    },

    creditsTest = function(menu)
    {
        menu.enterCreditsmenue();
    },
    creditsMenu = function(menu)
    {
        menu.exitMenue();
    },
    achievementTest = function(menu)
    {
        menu.enterAchievementmenue();
    },
    achievementMenu = function(menu)
    {
        menu.exitMenue();
    },
    optionTest = function(menu)
    {
        menu.enterOptionmenue();
    },
    optionMenu = function(menu)
    {
        menu.showMods();
        menu.showGameplayAndKeys();
        menu.showSettings();
        menu.exitMenue();
    },
    wikiTest = function(menu)
    {
        menu.enterWikimenue();
    },
    wikiMenu = function(menu)
    {
        var wikiView = menu.getWikiView();
        wikiView.tagChanged(0);
        wikiView.showPage("CO_ANDY");
        wikiView.showPage("MECH");
        wikiView.showPage("PLAINS");
        wikiView.showPage("HQ");
        menu.exitMenue();
    },
    shopTest = function(menu)
    {
        menu.enterShopMenu();
    },
    shopMenu = function(menu)
    {
        menu.exitMenue();
    },
    coStyleTest = function(menu)
    {
        menu.enterCOStyleMenu();
    },
    coStyleMenu = function(menu)
    {
        menu.selectedCOIDChanged("CO_ANDY");
        menu.editCOStyle();
        menu.exitMenue();
    },
    mapEditorTest = function(menu)
    {
        menu.enterEditor();
    },
    mapEditorMenu = function(menu)
    {
        menu.loadMap(":/maps/6_player/Stella di David.map");
        menu.resizeMap(1, 1, 1, 1);
        menu.resizeMap(-1, -1, -1, -1);
        menu.changeMap("test", "test", "test", "", 30, 30, 5, 0, 0);
        menu.newMap("test", "test", "test", "", 30, 30, 5, 0, 0);
        menu.exitEditor();
    },
    simpleAiTest = function(menu)
    {
        Init.aiTest = true;
        menu.enterSingleplayer();
    },
    mapsSelection = function(menu)
    {
        if (Init.aiTest === true)
        {
            menu.selectMap("maps/2_player/", "Agitated.map");
            menu.slotButtonNext();
            menu.slotButtonNext();
            var gameRules = map.getGameRules();
            gameRules.setFogMode(GameEnums.Fog_OfWar);
            var selection = menu.getPlayerSelection();
            selection.selectPlayerAi(0, 1);
            selection.selectPlayerAi(1, 2);
            selection.playerCO1Changed("CO_RANDOM", 0);
            selection.playerCO2Changed("CO_RANDOM", 0);
            selection.playerCO1Changed("CO_RANDOM", 1);
            selection.playerCO2Changed("CO_RANDOM", 1);
            menu.startGame();
        }
    },
    onVictory = function(menu)
    {
        Init.aiTest = false;
        menu.exitMenue();
    },
}
