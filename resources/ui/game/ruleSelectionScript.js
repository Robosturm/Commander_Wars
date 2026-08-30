var RuleSelectionScript =
{
    getWeatherLabelNames : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
            array.push(weather.getWeatherName() + ":");
        }
        return array;
    },
    getWeatherNames : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
            array.push(weather.getWeatherName());
        }
        return array;
    },
    getWeatherChances : function()
    {
        var map = currentMenu.getMap();
        var rules = map.getGameRules();
        var count = rules.getWeatherCount();
        var i = 0;
        var array = [];
        for (i = 0; i < count; ++i)
        {
            var weather = rules.getWeather(i);
            array.push(rules.getWeatherChance(weather.getWeatherId()));
        }
        return array;
    },
    getCoZoneOptions : function()
    {
        return [qsTr("Global"), qsTr("Global and CO Zone Bonus"), qsTr("Only CO Zone")];
    },
    getCoZoneMode : function()
    {
        return [qsTr("Money"), qsTr("Money only attacker"), qsTr("HP"), qsTr("HP only attacker")];
    },
    getFogOfWarRule : function()
    {
        return [qsTr("Off"), qsTr("Mist of War"), qsTr("Fog of War"), qsTr("Shroud of War")];
    },
    getDamageFormulaRule : function()
    {
        return [qsTr("Advance Wars 4"), qsTr("Advance Wars  1-3")];
    },
    getAiBehaviorRule : function()
    {
        return [qsTr("Standard"), qsTr("Counterpoint")];
    },
    // GameEnums.AiBehavior_Standard and _Counterpoint are 0 and 1, so the item index is the
    // rule value. The setter validates anything else back to Standard.
    getAiBehaviorValue : function()
    {
        return currentMenu.getMap().getGameRules().getAiBehaviorMode();
    },
    setAiBehaviorValue : function(input)
    {
        currentMenu.getMap().getGameRules().setAiBehaviorMode(input);
    },
    getFogValue : function()
    {
        var fogMode = currentMenu.getMap().getGameRules().getFogMode();
        if (fogMode === GameEnums.Fog_OfMist)
        {
            return 1;
        }
        else if (fogMode === GameEnums.Fog_Off)
        {
            return 0;
        }
        else
        {
            return fogMode + 1;
        }
    },
    setFogValue : function(input)
    {
        if (input === 1)
        {
            currentMenu.getMap().getGameRules().setFogMode(GameEnums.Fog_OfMist);
        }
        else if (input > 1)
        {
            currentMenu.getMap().getGameRules().setFogMode(input - 1);
        }
        else
        {
            currentMenu.getMap().getGameRules().setFogMode(input);
        }
    },
    getDayScreen : function()
    {
        return [qsTr("Default"), qsTr("Permanent")];
    },
    showScriptSelection : function()
    {
        return !currentMenu.getIsEditorMode() && currentMenu.getRuleChangeEabled();
    },
    currentVictoryRuleIndex : 0,
    currentVictoryRuleDisplayIndex : 0,
    victoryRuleEntries : null,

    compareVictoryRuleEntries : function(left, right)
    {
        if (left.categoryOrder !== right.categoryOrder)
        {
            return left.categoryOrder - right.categoryOrder;
        }
        if (left.categoryId !== right.categoryId)
        {
            if (left.categoryId < right.categoryId)
            {
                return -1;
            }
            return 1;
        }
        if (left.ruleOrder !== right.ruleOrder)
        {
            return left.ruleOrder - right.ruleOrder;
        }
        return left.managerIndex - right.managerIndex;
    },
    getVictoryRuleEntries : function()
    {
        if (RuleSelectionScript.victoryRuleEntries === null)
        {
            var rules = currentMenu.getMap().getGameRules();
            var entries = [];
            for (var index = 0; index < gameRuleManager.getVictoryRuleCount(); ++index)
            {
                var rule = rules.getVictoryRule(gameRuleManager.getVictoryRuleID(index));
                if (rule !== null)
                {
                    entries.push({
                        managerIndex: index,
                        rule: rule,
                        categoryId: rule.getRuleCategoryId(),
                        categoryOrder: rule.getRuleCategoryOrder(),
                        ruleOrder: rule.getRuleOrder()
                    });
                }
            }
            entries.sort(RuleSelectionScript.compareVictoryRuleEntries);
            RuleSelectionScript.victoryRuleEntries = entries;
        }
        return RuleSelectionScript.victoryRuleEntries;
    },
    initializeVictoryRuleEntries : function()
    {
        RuleSelectionScript.victoryRuleEntries = null;
        return RuleSelectionScript.getVictoryRuleCount();
    },
    getVictoryRuleCount : function()
    {
        return RuleSelectionScript.getVictoryRuleEntries().length;
    },
    getVictoryRuleId : function(loopIdx)
    {
        var entries = RuleSelectionScript.getVictoryRuleEntries();
        if (loopIdx >= 0 && loopIdx < entries.length)
        {
            return gameRuleManager.getVictoryRuleID(entries[loopIdx].managerIndex);
        }
        return "";
    },
    getVictoryRule : function()
    {
        return currentMenu.getMap().getGameRules().getVictoryRule(gameRuleManager.getVictoryRuleID(RuleSelectionScript.currentVictoryRuleIndex))
    },
    getVictoryRuleElementCount : function(loopIdx)
    {
        var entries = RuleSelectionScript.getVictoryRuleEntries();
        RuleSelectionScript.currentVictoryRuleDisplayIndex = loopIdx;
        if (loopIdx < 0 || loopIdx >= entries.length)
        {
            return 0;
        }
        RuleSelectionScript.currentVictoryRuleIndex = entries[loopIdx].managerIndex;
        return entries[loopIdx].rule.getRuleType().length;
    },
    getVictoryRuleElementType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRule().getRuleType()[loopIdx];
    },
    getIsVictoryRuleCheckboxType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRuleElementType(loopIdx) === "checkbox";
    },
    getIsVictoryRuleSpinboxType : function(loopIdx)
    {
        return RuleSelectionScript.getVictoryRuleElementType(loopIdx) === "spinbox";
    },
    getVictoryRuleCategory : function()
    {
        return RuleSelectionScript.getVictoryRule().getRuleCategory();
    },
    getVictoryRuleCategoryId : function()
    {
        return RuleSelectionScript.getVictoryRule().getRuleCategoryId();
    },
    getVictoryRuleCategoryColor : function()
    {
        return RuleSelectionScript.getVictoryRule().getRuleCategoryColor();
    },
    getVictoryRuleCategoryTooltip : function()
    {
        return RuleSelectionScript.getVictoryRule().getRuleCategoryTooltip();
    },
    getPreviousVictoryRule : function()
    {
        var displayIndex = RuleSelectionScript.currentVictoryRuleDisplayIndex - 1;
        var entries = RuleSelectionScript.getVictoryRuleEntries();
        if (displayIndex >= 0 && displayIndex < entries.length)
        {
            return entries[displayIndex].rule;
        }
        return null;
    },
    showVictoryRuleCategory : function(loopIdx)
    {
        if (loopIdx !== 0)
        {
            return false;
        }
        if (RuleSelectionScript.getVictoryRuleCategory().length === 0)
        {
            return false;
        }
        var categoryId = RuleSelectionScript.getVictoryRuleCategoryId();
        var previousRule = RuleSelectionScript.getPreviousVictoryRule();
        return previousRule === null || previousRule.getRuleCategoryId() !== categoryId;
    },
    getVictoryRuleCategoryY : function()
    {
        if (RuleSelectionScript.getPreviousVictoryRule() === null)
        {
            return 5;
        }
        return lastY + lastHeight + 20;
    },
    getVictoryRulename : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleName(loopIdx);
    },
    getVictoryRuleMaxValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getMaxValue(loopIdx);
    },
    getVictoryRuleDescription : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleDescription(loopIdx);
    },
    getVictoryRuleValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getRuleValue(loopIdx);
    },
    getVictoryRuleInfiniteValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getVictoryRule();
        return rule.getInfiniteValue(loopIdx);
    },
    setVictoryRuleValue : function(input, loopIdx)
    {
        var rule = currentMenu.getMap().getGameRules().getVictoryRule(objectId);
        return rule.setRuleValue(input, loopIdx);
    },
    getVictoryRuleX : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 5;
        }
        else
        {
            return lastX + 200 + 10;
        }
    },
    getVictoryRuleY : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            if (RuleSelectionScript.currentVictoryRuleDisplayIndex === 0 &&
                !RuleSelectionScript.showVictoryRuleCategory(loopIdx))
            {
                return 5;
            }
            else
            {
                return lastY + lastHeight + 10;
            }
        }
        else
        {
            return lastY;
        }
    },


    currentGameRuleIndex : 0,
    getGameRule : function()
    {
        return currentMenu.getMap().getGameRules().getGameRule(gameRuleManager.getGameRuleID(RuleSelectionScript.currentGameRuleIndex))
    },

    getGameRuleElementCount : function(loopIdx)
    {
        RuleSelectionScript.currentGameRuleIndex = loopIdx;
        return RuleSelectionScript.getGameRule().getRuleType().length;
    },
    getGameRuleElementType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRule().getRuleType()[loopIdx];
    },
    getIsGameRuleCheckboxType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRuleElementType(loopIdx) === "checkbox";
    },
    getIsGameRuleSpinboxType : function(loopIdx)
    {
        return RuleSelectionScript.getGameRuleElementType(loopIdx) === "spinbox";
    },
    getGameRulename : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleName(loopIdx);
    },
    getGameRuleDescription : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleDescription(loopIdx);
    },
    getGameRuleValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleValue(loopIdx);
    },
    getGameRuleInfiniteValue : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getInfiniteValue(loopIdx);
    },
    setGameRuleValue : function(input, loopIdx)
    {
        var rule = currentMenu.getMap().getGameRules().getGameRule(objectId);
        return rule.setRuleValue(input, loopIdx);
    },
    getRuleCategory : function(loopIdx)
    {
        var rule = RuleSelectionScript.getGameRule();
        return rule.getRuleCategory(loopIdx);
    },

    getGameRuleX : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 5;
        }
        else
        {
            return lastX + 300 + 10;
        }
    },
    getGameRuleY : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return lastY + lastHeight + 10;
        }
        else
        {
            return lastY;
        }
    },
    getGatewayOptionEnabled : function()
    {
        return currentMenu.getRuleChangeEabled() && !currentMenu.getIsMultiplayerOnlyGatewayMode() && !currentMenu.getIsMultiplayeLocalMode();
    },
    getGatewayDefaultValue : function()
    {
        if (currentMenu.getIsMultiplayerOnlyGatewayMode())
        {
            return true;
        }
        else
        {
            return currentMenu.getMap().getGameRules().getGatewayHosting();
        }
    },
    selectRandomPalette : function()
    {
        var mapPalette = currentMenu.getObject("MAP_PALETTE");
        var item = globals.randInt(0, mapPalette.getItemCount() - 1);
        mapPalette.setCurrentItem(item);
        RuleSelectionScript.applyPalette(item);
    },
    applyPalette : function(item)
    {
        currentMenu.getMap().getGameRules().setMapPalette(item);
        var paletteId = TERRAIN.getPaletteId(item, 0);
        currentMenu.getObject("PalettePreview").applyPalette(paletteId);
    },
    getSameCategory : function(category)
    {
        return RuleSelectionScript.getRuleCategory(loopIdx) === GAMERULE.ruleCategories[category];
    },
};
