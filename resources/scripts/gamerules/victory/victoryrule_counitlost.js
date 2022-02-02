var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        return qsTr("A player loses if he lost the given amount of co units.")
    };
    this.getRuleName = function(itemNumber)
    {
        return qsTr("Co Units lost");
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return "spinbox"
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        return 0;
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
        var playerCount = map.getPlayerCount();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            // get variable names
            var variableName = "COUNITCOUNT" + playerID.toString();
            var variableCOUnit0ActiveName = "COUNIT0ACTIVE" + playerID.toString();
            var variableCOUnit1ActiveName = "COUNIT1ACTIVE" + playerID.toString();
            // get variables
            var variables = rule.getVariables();
            var variable = variables.createVariable(variableName);
            var variableCOUnit0Active = variables.createVariable(variableCOUnit0ActiveName);
            var variableCOUnit1Active = variables.createVariable(variableCOUnit1ActiveName);
            // no player starts with a lost co unit
            variable.writeDataInt32(0);
            var co0 = player.getCO(0);
            var co1 = player.getCO(1);
            // keep track of co 0 units
            if (co0 !== null && co0.getCOUnit() !== null)
            {
                variableCOUnit0Active.writeDataBool(true);
            }
            else
            {
                variableCOUnit0Active.writeDataBool(false);
            }
            // keep track for co 1 units
            if (co1 !== null && co1.getCOUnit() !== null)
            {
                variableCOUnit1Active.writeDataBool(true);
            }
            else
            {
                variableCOUnit1Active.writeDataBool(false);
            }
        }
    };
    this.checkCOUnitCount = function(rule, player, map)
    {
        var playerID = player.getPlayerID();
        // get variable names
        var variableName = "COUNITCOUNT" + playerID.toString();
        var variableCOUnit0ActiveName = "COUNIT0ACTIVE" + playerID.toString();
        var variableCOUnit1ActiveName = "COUNIT1ACTIVE" + playerID.toString();
        // get variables
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        var variableCOUnit0Active = variables.createVariable(variableCOUnit0ActiveName);
        var variableCOUnit1Active = variables.createVariable(variableCOUnit1ActiveName);

        var count =  variable.readDataInt32();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        // check if co unit 0 got destroyed or created
        if (co0 !== null)
        {
            if (co0.getCOUnit() === null &&
                    variableCOUnit0Active.readDataBool() === true)
            {
                // he lost a co unit track it
                count++;
                variableCOUnit0Active.writeDataBool(false);
            }
            else if (co0.getCOUnit() !== null)
            {
                variableCOUnit0Active.writeDataBool(true);
            }
        }
        // check if co unit 1 got destroyed or created
        if (co1 !== null)
        {
            if (co1.getCOUnit() === null &&
                    variableCOUnit1Active.readDataBool() === true)
            {
                // he lost a co unit track it
                count++;
                variableCOUnit1Active.writeDataBool(false);
            }
            else if (co1 !== null && co1.getCOUnit() !== null)
            {
                variableCOUnit1Active.writeDataBool(true);
            }
        }
        // store current count and return current count
        variable.writeDataInt32(count);
        return count;
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        // get co unit limit
        var coUnitLimit = VICTORYRULE_COUNITLOST.getRuleValue(rule, map);
        // check limit against current lost count
        if (VICTORYRULE_COUNITLOST.checkCOUnitCount(rule, player, map) >= coUnitLimit &&
            coUnitLimit > 0)
        {
            return GameEnums.DefeatType_Defeated;
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map)
    {
        // return co unit count
        return VICTORYRULE_COUNITLOST.checkCOUnitCount(rule, player, map);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_COUNITLOST = new Constructor();
