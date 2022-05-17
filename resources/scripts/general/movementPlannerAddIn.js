var MOVEMENTPLANNERADDIN =
{
    getName : function()
    {
        return qsTr("Unknown");
    },
    getGroupName : function()
    {
        return qsTr("Unknown");
    },
    getDescription : function()
    {
        return qsTr("Unknown");
    },
    startAddIn : function(addIn)
    {
    },
    nextInputType : function(addIn, map)
    {
        return GameEnums.AddinStepType_Field;
    },
    onFieldSelected : function(addIn, x, y, map)
    {
    },
    onMenuInputDone : function(addIn, map)
    {
    },
    readyToExecute : function(addIn, map)
    {
        return true;
    },
    execute : function(addIn, map)
    {
        return true;
    },
};
