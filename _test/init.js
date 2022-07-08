var Init =
{
    step = 0,
    playTest = 0,
    steps = ["creditsTest",
             "achievementTest",
             "optionTest",
             "wikiTest",
             "shopTest",
             "coStyleTest",
             "mapEditorTest",
             "simplePlayTest",
             "simplePlayTest",
             "simplePlayTest",
             "simplePlayTest",
             "simplePlayTest",
            ],
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
        menu.toggleGridLayout();
        menu.toggleGridLayout();
        menu.toggleMiddleCrossGrid();
        menu.toggleMiddleCrossGrid();
        menu.showResizeMap();
        menu.showSaveMap();
        menu.showLoadMap();
        menu.showEditScript();
        menu.showEditCampaign();
        menu.showImportCoWTxTMap();
        menu.showExportAwdsAws();
        menu.showImportAwdsAw4();
        menu.showImportAwByWeb();
        menu.showEditMap();
        menu.flipX();
        menu.flipY();
        menu.rotateX();
        menu.rotateY();
        menu.showRandomMap();
        menu.changePlaceSelection();
        menu.editUnits();
        menu.editTerrains();
        menu.optimizePlayers();
        menu.showEditPlayers();
        menu.showEditRules();
        menu.copy();
        menu.viewMapStats();
        menu.showExit();
        menu.exitEditor();
    },
    simplePlayTest = function(menu)
    {
        menu.enterSingleplayer();
    },
    mapsSelection = function(menu)
    {
        if (Init.playTest === 0)
        {
            GameConsole.print("Testing ingame menus", 0);
            menu.selectMap("maps/2_player/", "Agitated.map");
            menu.buttonNext();
            menu.buttonNext();
            var selection = menu.getPlayerSelection();
            selection.selectPlayerAi(0, 0);
            selection.selectPlayerAi(1, 2);
            selection.playerCO1Changed("CO_RANDOM", 0);
            selection.playerCO2Changed("CO_RANDOM", 0);
            selection.playerCO1Changed("CO_RANDOM", 1);
            selection.playerCO2Changed("CO_RANDOM", 1);
            menu.startGame();
        }
        else
        {
            GameConsole.print("Testing ai's and fog of war", 0);
            menu.selectMap("maps/2_player/", "Agitated.map");
            menu.buttonNext();
            menu.buttonNext();
            var selection = menu.getPlayerSelection();
            var map = selection.getMap();
            var gameRules = map.getGameRules();
            gameRules.setFogMode(Init.playTest - 1);
            selection.selectPlayerAi(0, 1);
            selection.selectPlayerAi(1, 2);
            selection.playerCO1Changed("CO_RANDOM", 0);
            selection.playerCO2Changed("CO_RANDOM", 0);
            selection.playerCO1Changed("CO_RANDOM", 1);
            selection.playerCO2Changed("CO_RANDOM", 1);
            menu.startGame();
        }
    },
    gameMenu = function(menu)
    {
        if (Init.playTest === 0)
        {
            GameConsole.print("Showing all ingame menus", 0);
            menu.victoryInfo();
            menu.showCOInfo();
            menu.showGameInfo(0);
            menu.showOptions();
            menu.showChangeSound();
            menu.showAttackLog(0);
            menu.showRules();
            menu.showUnitInfo(0);
            menu.showUnitStatistics(0);
            menu.showExitGame();
            menu.showSurrenderGame();
            menu.showSaveAndExitGame();
            var wikiView = menu.showWiki();
            wikiView.tagChanged(0);
            wikiView.showPage("CO_ANDY");
            wikiView.showPage("MECH");
            wikiView.showPage("PLAINS");
            wikiView.showPage("HQ");
            menu.exitGame();
        }
    },
    onVictory = function(menu)
    {
        GameConsole.print("On Victory", 0);
        ++Init.playTest;
        menu.exitMenue();
    },
}
