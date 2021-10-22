var PLAYER =
{
    // static information about players
    // making it easy editable\modable for everyone :)

    getDefaultPlayerColors : function()
    {
        return PLAYER.defaultColors.length;
    },
    // mapping table for table color to display color
    defaultColors : [["#f00008", "#f05038"], // os
                     ["#0098f8", "#6058f0"], // bm
                     ["#00c010", "#60e058"], // ge
                     ["#d08000", "#d8d008"], // yc
                     ["#6038a0", "#978e97"], // bh
                     ["#5c5663", "#b552c6"], // bg
                     ["#797b78", "#4a424a"], // ma
                     ["#e88613", "#e88613"], // ac
                     ["#bc8248", "#8f541a"], // bd
                     ["#bf901c", "#a58c31"], // gs
                     ["#ff33cc", "#ff33cc"], // pf
                     ["#17a195", "#17a195"], // ti
                     ["#800080", "#474370"], // dm
                     ["#01cbff", "#01cbff"], // cyan
                     ["#006400", "#006400"], // dark_green
                     ["#ff0000", "#ff0000"], // red
                     ["#c4443d", "#943142"], // red_fire
                     ["#a29db9", "#7d798e"], // light_grey
                     ["#617c0e", "#617c0e"], // olive
                     ["#2342ba", "#2342ba"], // cobalt_ice
                     ["#85927b", "#85927b"], // silver
                     ["#908890", "#908890"], // neutral
                    ],

    getDefaultColor : function(index)
    {
        var color = "#000000";
        if (index < PLAYER.defaultColors.length)
        {
            color = PLAYER.defaultColors[index][0];
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
    getDisplayColor : function(index)
    {
        var color = "";
        if (index < PLAYER.defaultColors.length)
        {
            color = PLAYER.defaultColors[index][1];
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
                    "silver",
                    "neutral"],
    getColorTable : function(index)
    {
        if (index < PLAYER.colorTables.length)
        {
            return PLAYER.colorTables[index];
        }
        return "";
    },

    colorShades : [[Qt.point(2, 0),     40],
                   [Qt.point(5, 0),     70],
                   [Qt.point(8, 0),     100],
                   [Qt.point(11, 0),    130],
                   [Qt.point(14, 0),    160],
                   [Qt.point(17, 0),    190],
                   [Qt.point(20, 0),    55],
                   [Qt.point(23, 0),    90],
                   [Qt.point(26, 0),    140],
                   [Qt.point(29, 0),    170],
                   [Qt.point(32, 0),    10],
                   [Qt.point(35, 0),    15],
                   [Qt.point(38, 0),    20],
                   [Qt.point(41, 0),    25],
                   [Qt.point(44, 0),    35],
                   [Qt.point(47, 0),    40],
                   [Qt.point(50, 0),    45],
                   [Qt.point(53, 0),    50],
                   [Qt.point(56, 0),    60],
                   [Qt.point(59, 0),    65],
                   [Qt.point(62, 0),    70],
                   [Qt.point(65, 0),    75],
                   [Qt.point(68, 0),    85],
                   [Qt.point(71, 0),    90],
                   [Qt.point(74, 0),    95],
                   [Qt.point(77, 0),    100],
                   [Qt.point(80, 0),    130],
                   [Qt.point(83, 0),    135],
                   [Qt.point(86, 0),    140],
                   [Qt.point(89, 0),    155],
                   [Qt.point(92, 0),    160],
                   [Qt.point(95, 0),    165],
                   [Qt.point(98, 0),    170],
                   [Qt.point(101, 0),   175],
                   [Qt.point(104, 0),   180],
                   // 1 row
                   [Qt.point(130, 130),   20],
                   [Qt.point(133, 130),   40],
                   [Qt.point(136, 130),   80],
                   [Qt.point(139, 130),   100],
                   [Qt.point(142, 130),   120],
                   [Qt.point(145, 130),   140],
                   [Qt.point(148, 130),   170],
                   // 2 row
                   [Qt.point(130, 133),   "#000000"],
                   [Qt.point(133, 133),   30],
                   [Qt.point(136, 133),   60],
                   [Qt.point(139, 133),   90],
                   [Qt.point(142, 133),   120],
                   [Qt.point(145, 133),   140],
                   [Qt.point(148, 133),   160],
                   [Qt.point(151, 133),   "#ffffff"],
                   // 3 row
                   [Qt.point(130, 136),   "#000000"],
                   [Qt.point(133, 136),   50],
                   [Qt.point(136, 136),   70],
                   [Qt.point(139, 136),   100],
                   [Qt.point(142, 136),   130],
                   [Qt.point(145, 136),   "#ffffff"],
                   // 4 row
                   [Qt.point(130, 139),   "#000000"],
                   [Qt.point(133, 139),   30],
                   [Qt.point(136, 139),   60],
                   [Qt.point(139, 139),   90],
                   [Qt.point(142, 139),   130],
                   [Qt.point(145, 139),   160],
                   [Qt.point(148, 139),   "#ffffff"],
                   // 5 row
                   [Qt.point(130, 142),   "#000000"],
                   [Qt.point(133, 142),   30],
                   [Qt.point(136, 142),   60],
                   [Qt.point(139, 142),   90],
                   [Qt.point(142, 142),   130],
                   [Qt.point(145, 142),   160],
                   [Qt.point(148, 142),   "#ffffff"],
                   // 6 row
                   [Qt.point(130, 145),   "#000000"],
                   [Qt.point(133, 145),   30],
                   [Qt.point(136, 145),   60],
                   [Qt.point(139, 145),   90],
                   [Qt.point(142, 145),   130],
                   [Qt.point(145, 145),   160],
                   [Qt.point(148, 145),   "#ffffff"],
                   // 7 row
                   [Qt.point(130, 148),   "#000000"],
                   [Qt.point(133, 148),   20],
                   [Qt.point(136, 148),   40],
                   [Qt.point(139, 148),   60],
                   [Qt.point(142, 148),   90],
                   [Qt.point(145, 148),   130],
                   [Qt.point(148, 148),   160],
                   [Qt.point(151, 148),   "#ffffff"],
                   // 8 row
                   [Qt.point(130, 151),   "#000000"],
                   [Qt.point(133, 151),   20],
                   [Qt.point(136, 151),   40],
                   [Qt.point(139, 151),   60],
                   [Qt.point(142, 151),   80],
                   [Qt.point(145, 151),   90],
                   [Qt.point(148, 151),   110],
                   [Qt.point(148, 151),   130],
                   [Qt.point(151, 151),   140],
                   [Qt.point(154, 151),   160],
                   [Qt.point(157, 151),   180],
                   [Qt.point(160, 151),   "#ffffff"],
                   // grays 1
                   [Qt.point(2, 133),   "#6b5a63"],
                   [Qt.point(5, 133),   "#948c84"],
                   [Qt.point(8, 133),   "#cebdad"],
                   // grays 2
                   [Qt.point(2, 136),   "#383038"],
                   [Qt.point(5, 136),   "#707060"],
                   [Qt.point(8, 136),   "#c0a890"],
                   // grays 3
                   [Qt.point(2,  139),  "#282030"],
                   [Qt.point(5,  139),  "#403850"],
                   [Qt.point(8,  139),  "#585060"],
                   [Qt.point(11, 139),  "#807878"],
                   [Qt.point(14, 139),  "#d0b8a8"],
                   // grays 4
                   [Qt.point(2,  142),  "#393139"],
                   [Qt.point(5,  142),  "#4a4a4a"],
                   [Qt.point(8,  142),  "#847b73"],
                   [Qt.point(11, 142),  "#b5b5a5"],
                   [Qt.point(14, 142),  "#d6cebd"],
                   // grays 5
                   [Qt.point(2,  145),  "#393139"],
                   [Qt.point(5,  145),  "#4a4a4a"],
                   [Qt.point(8,  145),  "#847b73"],
                   [Qt.point(11, 145),  "#b5b5a5"],
                   [Qt.point(14, 145),  "#d6cebd"],
                   // grays 7
                   [Qt.point(2,  148),  "#584060"],
                   [Qt.point(5,  148),  "#807878"],
                   [Qt.point(8,  148),  "#b8a890"],
                   // grays 8
                   [Qt.point(2,  151),  "#424242"],
                   [Qt.point(5,  151),  "#6b6b6b"],
                   [Qt.point(8,  151),  "#a5a5a5"],
                   // face 1
                   [Qt.point(130,  4),  "#de8452"],
                   [Qt.point(133,  4),  "#ffde84"],
                   [Qt.point(139,  4),  "#addee7"],
                   [Qt.point(145,  4),  "#addee7"],
                   // face 2
                   [Qt.point(130,  7),  "#905038"],
                   [Qt.point(133,  7),  "#f89860"],
                   [Qt.point(136,  7),  "#f8d090"],
                   [Qt.point(139,  7),  "#006888"],
                   [Qt.point(142,  7),  "#88e8a8"],
                   // face 3
                   [Qt.point(139, 10),  "#083152"],
                   [Qt.point(142, 10),  "#507878"],
                   [Qt.point(145, 10),  "#98d8a0"],
                   // face 4
                   [Qt.point(139, 13),  "#f79c5a"],
                   [Qt.point(142, 13),  "#f0b838"],
                   [Qt.point(145, 13),  "#f8f0a0"],
                   // face 6
                   [Qt.point(139, 16),  "#083152"],
                   [Qt.point(142, 16),  "#507878"],
                   [Qt.point(145, 16),  "#98d8a0"],
                   // face 7
                   [Qt.point(130, 19),  "#885850"],
                   [Qt.point(133, 19),  "#a86848"],
                   [Qt.point(136, 19),  "#f0b058"],
                   [Qt.point(139, 19),  "#083152"],
                   [Qt.point(142, 19),  "#295a63"],
                   [Qt.point(145, 19),  "#528c7b"],
                   [Qt.point(148, 19),  "#73b594"],
                   [Qt.point(151, 19),  "#9cdea5"],
                   // face 8
                   [Qt.point(139, 22),  "#b510ef"],
                   [Qt.point(142, 22),  "#de7bff"],
                   [Qt.point(145, 22),  "#f784f7"],
    ],

    getColorForTable(index)
    {
        if (index < PLAYER.colorShades.length)
        {
            return PLAYER.colorShades[index][1];
        }
        return 100;
    },
    getPositionForTable(index)
    {
        if (index < PLAYER.colorShades.length)
        {
            return PLAYER.colorShades[index][0];
        }
        return Qt.point(255, 255);
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
