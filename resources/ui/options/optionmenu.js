var OptionMenu =
{
    getSupportedScreenSizes : function()
    {
        var baseSizes = [Qt.size(15360, 8640),
                         Qt.size(8192 , 8192),
                         Qt.size(10240, 4320),
                         Qt.size(8192 , 4608),
                         Qt.size(8192 , 4320),
                         Qt.size(7680 , 4800),
                         Qt.size(7680 , 4320),
                         Qt.size(6400 , 4800),
                         Qt.size(6400 , 4096),
                         Qt.size(5120 , 4096),
                         Qt.size(5120 , 3200),
                         Qt.size(5120 , 2880),
                         Qt.size(5120 , 2160),
                         Qt.size(4500 , 3000),
                         Qt.size(4096 , 3072),
                         Qt.size(4096 , 2160),
                         Qt.size(3840 , 2400),
                         Qt.size(3840 , 2160),
                         Qt.size(3840 , 1600),
                         Qt.size(3440 , 1440),
                         Qt.size(3240 , 2160),
                         Qt.size(3200 , 2400),
                         Qt.size(3200 , 2048),
                         Qt.size(3200 , 1800),
                         Qt.size(3000 , 2000),
                         Qt.size(2960 , 1440),
                         Qt.size(2880 , 1800),
                         Qt.size(2880 , 1620),
                         Qt.size(2880 , 1440),
                         Qt.size(2880 , 900 ),
                         Qt.size(2800 , 2100),
                         Qt.size(2736 , 1824),
                         Qt.size(2732 , 2048),
                         Qt.size(2560 , 2048),
                         Qt.size(2560 , 1920),
                         Qt.size(2560 , 1800),
                         Qt.size(2560 , 1700),
                         Qt.size(2560 , 1600),
                         Qt.size(2560 , 1440),
                         Qt.size(2560 , 1080),
                         Qt.size(2538 , 1080),
                         Qt.size(2436 , 1125),
                         Qt.size(2304 , 1728),
                         Qt.size(2256 , 1504),
                         Qt.size(2304 , 1440),
                         Qt.size(2280 , 1080),
                         Qt.size(2160 , 1440),
                         Qt.size(2160 , 1200),
                         Qt.size(2160 , 1080),
                         Qt.size(2048 , 1536),
                         Qt.size(2048 , 1280),
                         Qt.size(2048 , 1152),
                         Qt.size(2048 , 1080),
                         Qt.size(1920 , 1440),
                         Qt.size(1920 , 1400),
                         Qt.size(1920 , 1280),
                         Qt.size(1920 , 1200),
                         Qt.size(1920 , 1080),
                         Qt.size(1856 , 1392),
                         Qt.size(1800 , 1440),
                         Qt.size(1792 , 1344),
                         Qt.size(1776 , 1000),
                         Qt.size(1680 , 1050),
                         Qt.size(1600 , 1280),
                         Qt.size(1600 , 1200),
                         Qt.size(1600 , 1024),
                         Qt.size(1600 , 900 ),
                         Qt.size(1600 , 768 ),
                         Qt.size(1440 , 1440),
                         Qt.size(1440 , 1080),
                         Qt.size(1440 , 1024),
                         Qt.size(1440 , 960 ),
                         Qt.size(1440 , 900 ),
                         Qt.size(1400 , 1050),
                         Qt.size(1366 , 768 ),
                         Qt.size(1334 , 750 ),
                         Qt.size(1280 , 1024),
                         Qt.size(1280 , 960 ),
                         Qt.size(1280 , 854 ),
                         Qt.size(1280 , 800 ),
                         Qt.size(1280 , 768 ),
                         Qt.size(1280 , 720 ),
                         Qt.size(1152 , 900 ),
                         Qt.size(1152 , 864 ),
                         Qt.size(1152 , 768 ),
                         Qt.size(1152 , 720 ),
                         Qt.size(1136 , 640 ),
                         Qt.size(1120 , 832 ),
                         Qt.size(1080 , 1200),
                         Qt.size(1024 , 1024),
                         Qt.size(1024 , 800 ),
                         Qt.size(1024 , 768 ),
                         Qt.size(1024 , 640 ),
                         Qt.size(1024 , 600 ),
                         Qt.size(960 ,  720 ),
                         Qt.size(960 ,  640 )];
        var screenSize = settings.getScreenSize();
        var supportedSizes = [screenSize];
        var count = baseSizes.length;
        for (var i = 0; i < count; ++i)
        {
            var size = baseSizes[i];
            if (size.width <= screenSize.width &&
                size.height <= screenSize.height)
            {
                supportedSizes.push(size);
            }
        }
        return supportedSizes;
    },
    getSupportedScreenSizeItems : function()
    {
        var items = OptionMenu.getSupportedScreenSizes();
        var count = items.length;
        var stringItems = [];
        for (var i = 0; i < count; ++i)
        {
            var item = items[i];
            stringItems.push(item.width.toString() + " x " + item.height.toString());
        }
        return stringItems;
    },
    getCurrentScreenItem : function()
    {
        var items = OptionMenu.getSupportedScreenSizes();
        var dpiFactor = settings.getDpiFactor();
        var count = items.length;
        var width = Math.round(settings.getWidth() / dpiFactor);
        var height = Math.round(settings.getHeight() / dpiFactor);
        var currentItem = 0;
        for (var i = 0; i < count; ++i)
        {
            var item = items[i];
            if (item.width === width &&
                item.height === height)
            {
                currentItem = i;
                break;
            }
        }
        var stringItems  = OptionMenu.getSupportedScreenSizeItems()
        return stringItems[currentItem];
    },
    changeScreenSize : function(input)
    {
        var items = OptionMenu.getSupportedScreenSizes();
        currentMenu.changeScreenSize(items[input]);
    },
    changeScreen : function(input)
    {
        currentMenu.changeScreen(input);
    },
    getScreenModes : function()
    {
        return [qsTr("Window"), qsTr("Borderless"), qsTr("Fullscreen")];
    },
    getBattleAnimationModes : function()
    {
        return [qsTr("None"), qsTr("All"), qsTr("Own"), qsTr("Ally"), qsTr("Enemy")];
    },
    getBattleAnimationTypes : function()
    {
        return [qsTr("Detailed"), qsTr("Overworld"), qsTr("Detailed Fullscreen"), qsTr("Detailed Transparent"), qsTr("Fullscreen Transparent")];
    },
    getBattleAnimationTypeHelp : function()
    {
        return qsTr("Selects which battle animations are played when fighting an enemy.\n" +
                    "Detailed    - Normal battle animations\n" +
                    "Transparent - Colored background is semi-transparent\n" +
                    "Fullscreen  - Animations are upscaled based on the resolution\n");
    },
    getDialogAnimationModes : function()
    {
        return [qsTr("off"), qsTr("on")];
    },
    getCaptureAnimationModes : function()
    {
        return [qsTr("off"), qsTr("on")];
    },
    getDay2DayAnimationModes : function()
    {
        return [qsTr("off"), qsTr("on")];
    },
    getMovementAnimationModes : function()
    {
        return [qsTr("off"), qsTr("on")];
    },
    getCoInfoPositionModes : function()
    {
        return [qsTr("Flipping"), qsTr("Left"), qsTr("Right")];
    },
    getAutoCameraCenterModes : function()
    {
        return [qsTr("Last position"), qsTr("Owned HQ")];
    },
    showSelectDefaultRuleset : function()
    {
        currentMenu.showFileDialog(["*.grl"], settings.getUserPath() + "data/gamerules", false, "OptionMenu", "defaultRulesetSelected", "", false, qsTr("Load"));
    },
    defaultRulesetSelected : function(currentMenu, filename)
    {
        settings.setDefaultRuleset(filename);
        currentMenu.getObject("RulesetTextbox").setCurrentText(filename);
    },
    showSelectDefaultDannlist : function()
    {
        currentMenu.showFileDialog(["*.bl"], settings.getUserPath() + "data/unitbannlist", false, "OptionMenu", "defaultDannlistSelected", "", false, qsTr("Load"));
    },
    defaultDannlistSelected : function(currentMenu, filename)
    {
        settings.setDefaultBannlist(filename);
        currentMenu.getObject("BannlistTextbox").setCurrentText(filename);
    },
    changeLanguage : function(currentMenu, item)
    {
        var ids = settings.getLanguageIds();
        settings.setLanguage(ids[item]);
        currentMenu.markRestartNeeded();
        currentMenu.reloadSettings();
    },
};

