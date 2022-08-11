var MainMenu =
{
    getButtonX : function(btnI)
    {
        var col = btnI % 2;
        var x = 0;
        var width = 170;
        if (col == 0)
        {
            x = settings.getWidth() / 2.0 - width - 10;
        }
        else
        {
            x = settings.getWidth() / 2.0 + 10;
        }
        return x;
    },
    getButtonY : function(btnI)
    {
        var buttonCount = 7;
        var buttonHeigth = 50;
        return settings.getHeight() / 2.0 - buttonCount  / 2 * buttonHeigth + buttonHeigth * btnI;
    },

    loadPlayermenu : function(currentMenu)
    {
        currentMenu.resetUi();
        currentMenu.loadXml("ui/menu/playermenu.xml");
    },
    loadMainMenu : function(currentMenu)
    {
        currentMenu.resetUi();
        currentMenu.loadXml("ui/menu/mainmenu.xml");
    },
    loadMainOptionMenu : function(currentMenu)
    {
        currentMenu.resetUi();
        currentMenu.loadXml("ui/menu/mainoptionmenu.xml");
    },
    loadMainSingleMenu : function(currentMenu)
    {
        currentMenu.resetUi();
        currentMenu.loadXml("ui/menu/mainsinglemenu.xml");
    },
    showMapSelection : function(currentMenu)
    {
        var filter = [".map"];
        currentMenu.enterSingleplayer(filter);
    },
    showCampaignSelection : function(currentMenu)
    {
        var filter = [".jsm"];
        currentMenu.enterSingleplayer(filter);
    },
    changeUsername : function(currentMenu)
    {
        currentMenu.showTextInputDialog(qsTr("Select Username"), false, settings.getUsername(), "MainMenu", "usernameChanged");
    },
    usernameChanged : function(currentMenu, username)
    {
        if (username !== "")
        {
            if (username !== settings.getUsername())
            {
                settings.setUsername(username);
                currentMenu.restart();
            }
        }
    },
};

