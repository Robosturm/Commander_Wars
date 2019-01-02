var PLAYER =
{
    // static information about players
    // making it easy editable\modable for everyone :)

    // return the default set for colors
    loadDefaultPlayerColor : function (player)
    {
        switch (player.getPlayerID())
        {
            case 0:
                player.setColor("tomato");
                break;
            case 1:
                player.setColor("royalblue");
                break;
            case 2:
                player.setColor("lime");
                break;
            case 3:
                player.setColor("yellow");
                break;
            case 4:
                player.setColor("darkslategray");
                break;
            case 5:
                player.setColor("darkslateblue");
                break;
            case 6:
                player.setColor("steelblue");
                break;
            case 7:
                player.setColor("coral");
                break;
            case 8:
                player.setColor("peru");
                break;
            case 9:
                player.setColor("silver");
                break;
            case 10:
                player.setColor("magenta");
                break;
            case 11:
                player.setColor("teal");
                break;
            case 12:
                player.setColor("indigo");
                break;
            case 13:
                player.setColor("cyan");
                break;
            case 14:
                player.setColor("green");
                break;
            case 15:
                player.setColor("red");
                break;
            case 16:
                player.setColor("firebrick");
                break;
            case 17:
                player.setColor("purple");
                break;
            case 18:
                player.setColor("goldenrod");
                break;
            case 19:
                player.setColor("blue");
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
                var color = "#" + red.toString(16) + green.toString(16) + blue.toString(16)
                player.setColor(color);
                break;
        }
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


    // enumeration
    Alliance_Friend : 0,
    Alliance_Enemy : 1,
}
