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
                color = "#00FF00";
                break;
            case 15:
                color = "#FF0000";
                break;
            case 16:
                color = "firebrick";
                break;
            case 17:
                color = "#FFFF00";
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
                return tr("Orange Star");
            case "BM":
                return tr("Blue Moon");
            case "GE":
                return tr("Green Earth");
            case "YC":
                return tr("Yellow Comet");
            case "BH":
                return tr("Black Hole");
            case "BG":
                return tr("Bolt Guard");
            case "ML":
                return tr("Metall Army");
            case "AC":
                return tr("Amber Corona");
            case "BT":
                return tr("Brown Desert");
            case "GS":
                return tr("Jade Sun");
            case "PF":
                return tr("Pink Crystal");
            case "TI":
                return tr("Teal Guard");
            case "DM":
                return tr("Dark Matters");
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
                "CO_RANDOM"];
    },
}
