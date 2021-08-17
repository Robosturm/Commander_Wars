var PLAYER =
{
    // static information about players
    // making it easy editable\modable for everyone :)

    getDefaultPlayerColors : function()
    {
        return PLAYER.defaultColors.length;
    },
    defaultColors : [   "#f00008", // os
                        "#0098f8", // bm
                        "#00c010", // ge
                        "#d08000", // yc
                        "#6038a0", // bh
                        "#5c5663", // bg
                        "#797b78", // ma
                        "#e88613", // ac
                        "#bc8248", // bd
                        "#bf901c", // gs
                        "#ff33cc", // pf
                        "#17a195", // ti
                        "#800080", // dm
                        "#01cbff",
                        "#006400",
                        "#ff0000",
                        "#c4443d",
                        "#a29db9",
                        "#617c0e",
                        "#2342ba",
                        "#85927b",],
    getDefaultColor : function(index)
    {
        var color = "#000000";
        if (index < PLAYER.defaultColors.length)
        {
            color = PLAYER.defaultColors[index];
        }
        else
        {
            // create a cool random color
            var red = globals.randInt(0, 255);
            if (red < 16)
            {
                red = 16;
            }
            var green = globals.randInt(0, 255);
            if (green < 16)
            {
                green = 16;
            }
            var blue = globals.randInt(0, 255);
            if (blue < 16)
            {
                blue = 16;
            }
            color = "#" + red.toString(16) + green.toString(16) + blue.toString(16);
        }
        return color;
    },
    colorTables : [ "orange_star",
                    "blue_moon",
                    "green_earth",
                    "yellow_comet",
                    "black_hole",
                    "bolt_guard",
                    "metal_army",
                    "amber_corona",
                    "brown_desert",
                    "golden_sun",
                    "pink_frontier",
                    "teal_isle",
                    "dark_matter",
                    "cyan",
                    "dark_green",
                    "red",
                    "red_fire",
                    "light_grey",
                    "olive",
                    "cobalt_ice",
                    "silver",],
    getColorTable : function(index)
    {
        if (index < PLAYER.colorTables.length)
        {
            return PLAYER.colorTables[index];
        }
        return "";
    },
    colorShades : [ 40,
                    70,
                    100,
                    130,
                    160,
                    190,
                    55,
                    90,
                    140,
                    170,
                    10,
                    15,
                    20,
                    25,
                    35,
                    40,
                    45,
                    50,
                    60,
                    65,
                    70,
                    75,
                    85,
                    90,
                    95,
                    100,
                    130,
                    135,
                    140,
                    155,
                    160,
                    165,
                    170,
                    175,
                    180,],
    getColorForTable(index)
    {
        if (index < PLAYER.colorShades.length)
        {
            return PLAYER.colorShades[index];
        }
        return 100;
    },
    getColorTableCount : function()
    {
        // amount of different colors in the table (minus transparent)
        return PLAYER.colorShades.length;
    },
    // return the default set for colors
    loadDefaultPlayerColor : function (player)
    {
        var color = PLAYER.getDefaultColor(player.getPlayerID());
        player.setColor(color);
    },
    armies : ["OS",
              "BM",
              "GE",
              "YC",
              "BH",
              "BG",
              "MA",
              "AC",
              "BD",
              "GS",
              "PF",
              "TI",
              "DM"],
    armyNames : [  qsTr("Orange Star"),
                   qsTr("Blue Moon"),
                   qsTr("Green Earth"),
                   qsTr("Yellow Comet"),
                   qsTr("Black Hole"),
                   qsTr("Bolt Guard"),
                   qsTr("Metal Army"),
                   qsTr("Amber Corona"),
                   qsTr("Brown Desert"),
                   qsTr("Golden Sun"),
                   qsTr("Pink Frontier"),
                   qsTr("Teal Island"),
                   qsTr("Dark Matter")],
    getDefaultArmy : function (player)
    {
        var id = player.getPlayerID() % (PLAYER.armies.length);
        var armies = PLAYER.getArmies();
        return armies[id];
    },
    getArmyNames : function ()
    {
        return PLAYER.armyNames;
    },
    // co selection infos
    getArmies : function()
    {
        return PLAYER.armies;
    },

    getArmyCOsOS : function()
    {
        return ["CO_EMPTY_OS", "CO_ANDY", "CO_MAX", "CO_SAMI", "CO_JAKE",
                "CO_RACHEL", "CO_NELL", "CO_HACHI", "CO_RANDOM"];
    },

    getArmyCOsBM : function()
    {
        return ["CO_EMPTY_BM", "CO_GRIT", "CO_COLIN", "CO_SASHA", "CO_OLAF",
                "CO_ALEXANDER", "CO_RANDOM"];
    },

    getArmyCOsGE : function()
    {
        return ["CO_EMPTY_GE", "CO_DRAKE", "CO_EAGLE", "CO_JESS", "CO_JAVIER",
                "CO_PETER", "CO_SMITAN", "CO_RANDOM"];
    },

    getArmyCOsYC : function()
    {
        return ["CO_EMPTY_YC", "CO_SONJA", "CO_SENSEI", "CO_GRIMM", "CO_KANBEI",
                "CO_RATTIGAN", "CO_RANDOM"];
    },

    getArmyCOsBH : function()
    {
        return ["CO_EMPTY_BH", "CO_FLAK", "CO_ADDER", "CO_LASH", "CO_HAWKE",
                "CO_STURM", "CO_RANDOM"];
    },

    getArmyCOsBG : function()
    {
        return ["CO_EMPTY_BG", "CO_JUGGER", "CO_KOAL", "CO_KINDLE",
                "CO_VON_BOLT", "CO_RANDOM"];
    },

    getArmyCOsMA : function()
    {
        return ["CO_EMPTY_MA", "CO_EPOCH", "CO_ROBOANDY", "CO_IK_486_B7", "CO_NAPOLEON",
                "CO_ROBOSTURM", "CO_RANDOM"];
    },

    getArmyCOsAC : function()
    {
        return ["CO_EMPTY_AC", "CO_WILL", "CO_ISABELLA", "CO_LIN", "CO_BRENNER",
                "CO_SABAKI", "CO_SOPHIE", "CO_OZZY", "CO_MARY", "CO_RANDOM"];
    },

    getArmyCOsBD : function()
    {
        return ["CO_EMPTY_BD", "CO_GAGE", "CO_TASHA", "CO_ADAM", "CO_FORSYTHE",
                "CO_ZANDRA", "CO_RANDOM"];
    },

    getArmyCOsGS : function()
    {
        return ["CO_EMPTY_GS", "CO_MINAMOTO", "CO_XAVIER", "CO_SANJURO",
                "CO_MEIYO", "CO_RANDOM"];
    },

    getArmyCOsPF : function()
    {
        return ["CO_EMPTY_PF", "CO_MINA", "CO_AIRA", "CO_ALEXIS",
                "CO_NANA", "CO_AMY", "CO_RANDOM"];
    },

    getArmyCOsTI : function()
    {
        return ["CO_EMPTY_TI", "CO_CONRAD", "CO_CASSIDY", "CO_WAYLON",
                "CO_GREYFIELD", "CO_JOEY", "CO_WALTER", "CO_RANDOM"];
    },

    getArmyCOsDM : function()
    {
        return ["CO_EMPTY_DM", "CO_BEAST", "CO_PENNY", "CO_TABITHA", "CO_CAULDER",
                "CO_MELANTHE", "CO_JULIA", "CO_GRAVES", "CO_VARLOT",
                "CO_YUKIO", "CO_RANDOM"];
    },

    startOfTurn : function(player)
    {
        // do custom start of turn action for a player here.
        // Note this function gets called after earning money but before refulling units
    },
}
