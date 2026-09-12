BACKGROUNDSELECTOR =
{

    MAINMENU_IDS : [
        "mainmenu_1",
        "mainmenu_2",
        "mainmenu_3",
        "mainmenu_4",
        "mainmenu_5",
        "mainmenu_6",
        "mainmenu_7",
        "mainmenu_8",
        "mainmenu_9",
        "mainmenu_10",
        "mainmenu_11",
        "mainmenu_12",
        "mainmenu_13",
        "mainmenu_14",
        "mainmenu_15",
        "mainmenu_16",
        "mainmenu_17",
        "mainmenu_18",
        "mainmenu_19",
        "mainmenu_20",
        "mainmenu_21",
        "mainmenu_22",
        "mainmenu_23",
        "mainmenu_24",
        "mainmenu_25",
        "mainmenu_26",
        "mainmenu_27",
        "mainmenu_28",
        "mainmenu_29",
        "mainmenu_30",
        "mainmenu_31",
        "mainmenu_32",
        "mainmenu_33",
        "mainmenu_34",
        "mainmenu_35",
        "mainmenu_36",
        "mainmenu_37",
        "mainmenu_38",
        "mainmenu_39",
        "mainmenu_40",
        "mainmenu_41",
        "mainmenu_42",
        "mainmenu_43",
    ],
    GAMEMENU_IDS : [
        "gamemenu_1",
        "gamemenu_2",
        "gamemenu_3",
        "gamemenu_4",
        "gamemenu_5",
        "gamemenu_6",
        "gamemenu_7",
        "gamemenu_8",
        "gamemenu_9",
        "gamemenu_10",
        "gamemenu_11",
        "gamemenu_12",
        "gamemenu_13",
        "gamemenu_14",
        "gamemenu_15",
        "gamemenu_16",
        "gamemenu_17",
        "gamemenu_18",
        "gamemenu_19",
        "gamemenu_20",
        "gamemenu_21",
        "gamemenu_22",
    ],
    // the following hooks are called to determine the background sprite
    // note you may returna list of sprite id's the game will try to find the image which requires the least streatching and selects that as the background
    // note the main game loading screen won't call these hooks cause the hooks aren't loaded yet
    getBackGroundFromList: function(pool, background)
    {
        var count = pool.length;
        if (count <= 0)
        {
            return [background];
        }
        var index = globals.randInt(0, count - 1, true);
        return [pool[index]];
    },

    getBackgroundSprites : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesCoGeneratorMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesAchievementmenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesCampaignMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesCOStyleMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesCreditsMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesEditorMenu : function(menu, background)
    {
        return BACKGROUNDSELECTOR.getBackGroundFromList(BACKGROUNDSELECTOR.GAMEMENU_IDS, background);
    },
    getBackgroundSpritesMainwindow : function(menu, background)
    {
        return BACKGROUNDSELECTOR.getBackGroundFromList(BACKGROUNDSELECTOR.MAINMENU_IDS, background);
    },
    getBackgroundSpritesMapSelectionMapsMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesReplayMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesShopmenu : function(menu, background)
    {
        return [background];
    },

    getBackgroundSpritesVictoryMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesWikimenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesLobbyMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesLoadingScreen : function(menu, background)
    {
        return BACKGROUNDSELECTOR.getBackGroundFromList(BACKGROUNDSELECTOR.MAINMENU_IDS, background);
    },
    getBackgroundSpritesGameMenu : function(menu, background)
    {
        var variable = menu.getMap().getGameRules().getVariables().createVariable("BACKGROUND_SPRITE");
        if (variable !== null)
        {
            var sprite = variable.readDataString();
            if (sprite !== "")
            {
                background = sprite;
            }
        }
        return [background];
    },
    getBackgroundSpritesGeneratorMenu : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesMovementPlanner : function(menu, background)
    {
        return [background];
    },
    getBackgroundSpritesReplayMenu : function(menu, background)
    {
        return [background];
    },
}
