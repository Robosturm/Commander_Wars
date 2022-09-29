var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Show missile targets");
    };
    this.getGroupName = function()
    {
        return qsTr("Player");
    };
    this.getDescription = function()
    {
        return qsTr("Allows you to see where missiles and meteors are going to hit.");
    };
    this.startAddIn = function(addIn)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");

        var targetCountInputVariable = variables.getVariable("targetCount");
        if (targetCountInputVariable === null)
        {
            targetCountInputVariable = variables.createVariable("targetCount");
            targetCountInputVariable.writeDataInt32(1);
        }
        stepInputVariable.writeDataInt32(0);
    };
    this.nextInputType = function(addIn, map, planner)
    {

        return GameEnums.AddinStepType_Menu;
    };
    this.getUiXml = function(addIn, map, planner)
    {
        return "scripts/movementPlannerAddIns/movementplanneraddin_showmissiletargets.xml";
    };
    this.onMenuInputDone = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(1);
    };
    this.readyToExecute = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        var stepInput = stepInputVariable.readDataInt32();
        if (stepInput === 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.execute = function(addIn, map, planner)
    {
        addIn.hide();
        MOVEMENTPLANNERADDIN_SHOWMISSILETARGETS.update(addIn, map, planner);
        addIn.getPlanner().addActiveAddInToUpdateAddIns();
        return true;
    };
    this.update = function(addIn, map, planner)
    {
        addIn.removeAllSprites();
        var variables = addIn.getVariables();
        var targetCountInputVariable = variables.createVariable("targetCount");
        var count = targetCountInputVariable.readDataInt32();
        for (var i = 0; i < count; i++)
        {
            var colorInputVariable = variables.createVariable("color" + i.toString());
            var targetTypeInputVariable = variables.createVariable("targetType" + i.toString());
            var rangeInputVariable = variables.createVariable("targetRange" + i.toString());
            var ownerInputVariable = variables.createVariable("owner" + loopIdx.toString());
            var damageInputVariable = variables.createVariable("damage" + loopIdx.toString());
            var color = colorInputVariable.readDataString();
            var type = targetTypeInputVariable.readDataInt32();
            var range = rangeInputVariable.readDataInt32();
            var owner = ownerInputVariable.readDataInt32();
            var damage = damageInputVariable.readDataInt32();
            var player = map.getPlayer(owner - 1);
            var target = player.getRockettarget(range, damage, 1.2, type);
            var fields = globals.getCircle(0, range);
            var size = fields.size();
            for (var i2 = 0; i2 < size; i2++)
            {
                var x = fields.at(i2).x + target.x;
                var y = fields.at(i2).y + target.y;
                if (map.onMap(x, y))
                {
                    addIn.addSprite("marked+field", x, y, 0, 0, color);
                }
            }
        }
    };
    this.stopAddIn = function(addIn)
    {
        var planner = addIn.getPlanner();
        planner.removeActiveAddInFromUpdateAddIns();
        addIn.hide();
        planner.stopAddIn();
    };
    this.getTargetCount = function(addIn)
    {
        var variables = addIn.getVariables();
        var targetCountInputVariable = variables.createVariable("targetCount");
        var count = targetCountInputVariable.readDataInt32();
        return count;
    };
    this.changeTargetCount = function(addIn, input)
    {
        var variables = addIn.getVariables();
        var targetCountInputVariable = variables.createVariable("targetCount");
        targetCountInputVariable.writeDataInt32(input);
        addIn.hide();
        addIn.getPlanner().showAddInUi();
    };
    this.getColors = function()
    {
        return ["#ff0000", "#00ff00", "#0000ff"]
    };
    this.getStartColor = function(addIn, loopIdx)
    {
        var variables = addIn.getVariables();
        var colorInputVariable = variables.getVariable("color" + loopIdx.toString());
        var startColor = "#ff0000";
        if (colorInputVariable === null)
        {
            colorInputVariable = variables.createVariable("color" + loopIdx.toString());
            colorInputVariable.writeDataString(startColor);
        }
        else
        {
            startColor = colorInputVariable.readDataString();
        }
        return startColor;
    };
    this.changeColor = function(addIn, input, loopIdx)
    {
        var variables = addIn.getVariables();
        var colorInputVariable = variables.createVariable("color" + loopIdx.toString());
        colorInputVariable.writeDataString(input);
    };
    this.getTargetTypes = function()
    {
        return [qsTr("Funds"), qsTr("HP low funds"), qsTr("HP high funds")];
    };
    this.getStartType = function(addIn, loopIdx)
    {
        var variables = addIn.getVariables();
        var targetTypeInputVariable = variables.getVariable("targetType" + loopIdx.toString());
        var startType = GameEnums.RocketTarget_Money;
        if (targetTypeInputVariable === null)
        {
            targetTypeInputVariable = variables.createVariable("targetType" + loopIdx.toString());
            targetTypeInputVariable.writeDataInt32(startType);
        }
        else
        {
            startType = targetTypeInputVariable.readDataInt32();
        }
        return MOVEMENTPLANNERADDIN_SHOWMISSILETARGETS.getTargetTypes()[startType];
    };
    this.changeTargetType = function(addIn, input, loopIdx)
    {
        var variables = addIn.getVariables();
        var targetTypeInputVariable = variables.createVariable("targetType" + loopIdx.toString());
        targetTypeInputVariable.writeDataInt32(input);
    };
    this.getStartRange = function(addIn, loopIdx)
    {
        var variables = addIn.getVariables();
        var startRangeInputVariable = variables.getVariable("targetRange" + loopIdx.toString());
        var startRange = 2;
        if (startRangeInputVariable === null)
        {
            startRangeInputVariable = variables.createVariable("targetRange" + loopIdx.toString());
            startRangeInputVariable.writeDataInt32(startRange);
        }
        else
        {
            startRange = startRangeInputVariable.readDataInt32();
        }
        return startRange;
    };
    this.changeTargetRange = function(addIn, input, loopIdx)
    {
        var variables = addIn.getVariables();
        var startRangeInputVariable = variables.createVariable("targetRange" + loopIdx.toString());
        startRangeInputVariable.writeDataInt32(input);
    };
    this.getStartDamage = function(addIn, loopIdx)
    {
        var variables = addIn.getVariables();
        var damageInputVariable = variables.getVariable("damage" + loopIdx.toString());
        var damage = 3;
        if (damageInputVariable === null)
        {
            damageInputVariable = variables.createVariable("damage" + loopIdx.toString());
            damageInputVariable.writeDataInt32(damage);
        }
        else
        {
            damage = damageInputVariable.readDataInt32();
        }
        return damage;
    };
    this.changeTargetDamage = function(addIn, input, loopIdx)
    {
        var variables = addIn.getVariables();
        var damageInputVariable = variables.createVariable("damage" + loopIdx.toString());
        damageInputVariable.writeDataInt32(input);
    };
    this.getStartOwner = function(addIn, loopIdx)
    {
        var variables = addIn.getVariables();
        var ownerInputVariable = variables.getVariable("owner" + loopIdx.toString());
        var owner = 1;
        if (ownerInputVariable === null)
        {
            ownerInputVariable = variables.createVariable("owner" + loopIdx.toString());
            ownerInputVariable.writeDataInt32(owner);
        }
        else
        {
            owner = ownerInputVariable.readDataInt32();
        }
        return owner;
    };
    this.changeTargetOwner = function(addIn, input, loopIdx)
    {
        var variables = addIn.getVariables();
        var ownerInputVariable = variables.createVariable("owner" + loopIdx.toString());
        ownerInputVariable.writeDataInt32(input);
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_SHOWMISSILETARGETS = new Constructor();
