var PLAYER =
{
    // static information about players
    // making it easy editable\modable for everyone :)

    getDefaultPlayerColors : function()
    {
        return 20;
    },
    getDefaultColor : function(index)
    {
        var color = "#000000";
        switch (index)
        {
            case 0:
                color = "#f85800";
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
                color = "#6038a0";
                break;
            case 5:
                color = "darkslateblue";
                break;
            case 6:
                color = "steelblue";
                break;
            case 7:
                color = "coral";
                break;
            case 8:
                color = "peru";
                break;
            case 9:
                color = "silver";
                break;
            case 10:
                color = "magenta";
                break;
            case 11:
                color = "teal";
                break;
            case 12:
                color = "indigo";
                break;
            case 13:
                color = "cyan";
                break;
            case 14:
                color = "green";
                break;
            case 15:
                color = "red";
                break;
            case 16:
                color = "firebrick";
                break;
            case 17:
                color = "purple";
                break;
            case 18:
                color = "goldenrod";
                break;
            case 19:
                color = "blue";
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
        switch (id)
        {
            case 0:
                return "OS";
            case 1:
                return "BM";
            case 2:
                return "GE";
            case 3:
                return "YC";
            case 4:
                return "BH";
            case 5:
                return "BG";
            case 6:
                return "MA";
            case 7:
                return "AC";
            case 8:
                return "BD";
            case 9:
                return "JS";
            case 10:
                return "PC";
            case 11:
                return "TG";
            case 12:
                return "DM";
        }
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
}
