var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Activate power");
    };
    this.getGroupName = function()
    {
        return qsTr("Player");
    };
    this.getDescription = function()
    {
        return qsTr("Activates the power of one player.");
    };
    this.getPlayerLabel = function()
    {
        return qsTr("Player:");
    };
    this.getPlayerTooltip = function()
    {
        return qsTr("Player of which the CO power gets activated.");
    };
    this.getCOLabel = function()
    {
        return qsTr("CO:");
    };
    this.getCOTooltip = function()
    {
        return qsTr("CO that should activate its power.");
    };
    this.getPowerLabel = function()
    {
        return qsTr("Power:");
    };
    this.getPowerTooltip = function()
    {
        return qsTr("Power which should be activated.");
    };
    this.getApplyLabel = function()
    {
        return qsTr("Apply");
    };
    this.getApplyTooltip = function()
    {
        return qsTr("Activates the CO power.");
    };
    this.getCancelLabel = function()
    {
        return qsTr("Cancel");
    };
    this.getCancelTooltip = function()
    {
        return qsTr("Cancels the dialog.");
    };

    this.startAddIn = function(addIn)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(0);
    };
    this.nextInputType = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var ownerVariable = variables.createVariable("owner");
        ownerVariable.writeDataInt32(1);
        var coVariable = variables.createVariable("co");
        coVariable.writeDataInt32(0);
        var powerVariable = variables.createVariable("power");
        powerVariable.writeDataInt32(0);
        return GameEnums.AddinStepType_Menu;
    };
    this.getUiXml = function(addIn, map, planner)
    {
        return "scripts/movementPlannerAddIns/movementplanneraddin_activatepower.xml";
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
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(0);
        var ownerVariable = variables.createVariable("owner");
        var coVariable = variables.createVariable("co");
        var powerVariable = variables.createVariable("power");
        var coIdx = coVariable.readDataInt32();
        var owner = ownerVariable.readDataInt32();
        var power = powerVariable.readDataInt32();
        var player = map.getPlayer(owner - 1);
        if (power === 2)
        {
            var co1 = player.getCO(0);
            var co2 = player.getCO(1);
            if (co1 !== null &&
                co2 !== null)
            {
                co1.activateSuperpower(GameEnums.PowerMode_Tagpower);
                co2.activateSuperpower(GameEnums.PowerMode_Tagpower);
            }
        }
        else
        {
            var co = player.getCO(coIdx);
            if (co !== null)
            {
                if (power === 0 && co.getPowerStars() > 0)
                {
                    co.activatePower();
                }
                else if (power === 0 && co.getSuperpowerStars() > 0)
                {
                    co.activateSuperpower(GameEnums.PowerMode_Superpower);
                }
            }
        }
        GameAnimationFactory.skipAllAnimations();
        return true;
    };
    this.changeOwner = function(addIn, owner)
    {
        var variables = addIn.getVariables();
        var ownerVariable = variables.createVariable("owner");
        ownerVariable.writeDataString(owner)
    };
    this.changeCo = function(addIn, co)
    {
        var variables = addIn.getVariables();
        var coVariable = variables.createVariable("co");
        coVariable.writeDataInt32(co)
    };
    this.changePower = function(addIn, power)
    {
        var variables = addIn.getVariables();
        var powerVariable = variables.createVariable("power");
        powerVariable.writeDataInt32(power)
    };
    this.getCoList = function()
    {
        return [qsTr("CO 1"), qsTr("CO 2")];
    };
    this.getPowerList = function()
    {
        return [qsTr("Power"), qsTr("Superpower"), qsTr("Tagpower")];
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_ACTIVATEPOWER = new Constructor();
