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
                player.setColor("#E74F17");
                break;
            case 1:
                player.setColor("#1FC7FF");
                break;
            case 2:
                player.setColor("#00FF00");
                break;
            case 3:
                player.setColor("#FFC700");
                break;
            case 4:
                player.setColor("#00FF00");
                break;
            case 5:
                player.setColor("#989291");
                break;
            case 6:
                player.setColor("#B4B4B4");
                break;
            case 7:
                player.setColor("#4C6D6D");
                break;
            case 8:
                player.setColor("#FF8D00");
                break;
            case 9:
                player.setColor("#C86400");
                break;
            case 10:
                player.setColor("#B1A40E");
                break;
            case 11:
                player.setColor("#F400F4");
                break;
            case 12:
                player.setColor("#5AE1DF");
                break;
            case 13:
                player.setColor("#303CD5");
                break;
            case 14:
                player.setColor("#9400D3");
                break;
            case 15:
                player.setColor("#0000FF");
                break;
            case 16:
                player.setColor("#804000");
                break;
            case 17:
                player.setColor("#B93782");
                break;
            case 18:
                player.setColor("#676767");
                break;
            case 19:
                player.setColor("#C800C8");
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
        switch (player.getPlayerID())
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
                return "ML";
            case 7:
                return "AC";
            case 8:
                return "BT";
            case 9:
                return "GS";
            case 10:
                return "PF";
            case 11:
                return "TI";
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
                return tr("Mecha Legion");
            case "AC":
                return tr("Amber Corona");
            case "BT":
                return tr("Brown Tundra");
            case "GS":
                return tr("Golden Sun");
            case "PF":
                return tr("Pink Frontier");
            case "TI":
                return tr("Teal Isles");
            case "DM":
                return tr("Dark Matters");
        }
    },
}
