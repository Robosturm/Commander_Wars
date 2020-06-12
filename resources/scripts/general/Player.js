var PLAYER =
{
    // static information about players
    // making it easy editable\modable for everyone :)

    getDefaultPlayerColors : function()
    {
        return 21;
    },
    getDefaultColor : function(index)
    {
        var color = "#000000";
        switch (index)
        {
            case 0:
                color = "#ff5a00";
                break;
            case 1:
                color = "#0068e8";
                break;
            case 2:
                color = "#00c010";
                break;
            case 3:
                color = "#f8c000";
                break;
            case 4:
                color = "#5f11b7";
                break;
            case 5:
                color = "#2d2dd5";
                break;
            case 6:
                color = "lightsteelblue";
                break;
            case 7:
                color = "coral";
                break;
            case 8:
                color = "peru";
                break;
            case 9:
                color = "goldenrod";
                break;                
            case 10:
                color = "magenta";
                break;                
            case 11:
                color = "teal";
                break;                
            case 12:
                color = "purple";
                break;
            case 13:
                color = "cyan";
                break;
            case 14:
                color = "#006400";
                break;
            case 15:
                color = "#FF0000";
                break;
            case 16:
                color = "firebrick";
                break;
            case 17:
                color = "#a29db9";
                break;
            case 18:
                color = "olive";
                break;
            case 19:
                color = "#0000FF";
                break;
            case 20:
                color = "silver";
                break;
            default:
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
                break;
        }
        return color;
    },

    getColorTable : function(index)
    {
        switch (index)
        {
            case 0:
                return "orange_star";
            case 1:
                return "blue_moon";
            case 2:
                return "green_earth";
            case 3:
                return "yellow_comet";
            case 4:
                return "black_hole";
            case 5:
                return "bolt_guard";
            case 6:
                return "metal_army";
            case 7:
                return "amber_corona"
            case 8:
                return "brown_desert";
            case 9:
                return "golden_sun";
            case 10:
                return "pink_frontier";
            case 11:
                return "teal_isle";
            case 12:
                return "dark_matter";
            case 13:
                return "cyan";
            case 14:
                return "dark_green";
            case 15:
                return "red";
            case 16:
                return "red_fire";
            case 17:
                return "light_grey";
            case 18:
                return "olive";
            case 19:
                return "cobalt_ice";
            case 20:
                return "silver";
        };
        return "";
    },

    getColorForTable(index)
    {
        switch (index)
        {
            case 0:
                return 40;
            case 1:
                return 70;
            case 2:
                return 100;
            case 3:
                return 130;
            case 4:
                return 160;
            case 5:
                return 190;
            case 6:
                return 55;
            case 7:
                return 90;
            case 8:
                return 140;
            case 9:
                return 170;
        }
        return 100;
    },

    getColorTableCount : function()
    {
        // amount of different colors in the table (minus transparent)
        return 10;
    },

    // return the default set for colors
    loadDefaultPlayerColor : function (player)
    {
        var color = PLAYER.getDefaultColor(player.getPlayerID());
        player.setColor(color);
    },

    getDefaultArmy : function (player)
    {
        var id = player.getPlayerID() % 13;
        var armies = PLAYER.getArmies();
        return armies[id];
    },

    getArmyName : function (army)
    {
        switch (army)
        {
            case "OS":
                return qsTr("Orange Star");
            case "BM":
                return qsTr("Blue Moon");
            case "GE":
                return qsTr("Green Earth");
            case "YC":
                return qsTr("Yellow Comet");
            case "BH":
                return qsTr("Black Hole");
            case "BG":
                return qsTr("Bolt Guard");
            case "ML":
                return qsTr("Metall Army");
            case "AC":
                return qsTr("Amber Corona");
            case "BT":
                return qsTr("Brown Desert");
            case "GS":
                return qsTr("Jade Sun");
            case "PF":
                return qsTr("Pink Crystal");
            case "TI":
                return qsTr("Teal Guard");
            case "DM":
                return qsTr("Dark Matters");
        }
    },

    // co selection infos
    getArmies : function()
    {
        return ["OS",
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
                "DM"];
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
                "CO_GREYFIELD", "CO_JOEY", "CO_RANDOM"];
    },

    getArmyCOsDM : function()
    {
        return ["CO_EMPTY_DM", "CO_PENNY", "CO_TABITHA", "CO_CAULDER",
                "CO_MELANTHE", "CO_JULIA", "CO_GRAVES", "CO_VARLOT",
                "CO_YUKIO", "CO_RANDOM"];
    },
}
