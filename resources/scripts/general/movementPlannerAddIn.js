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
    nextInputType : function(addIn, map, planner)
    {
        return GameEnums.AddinStepType_Field;
    },
    onFieldSelected : function(addIn, x, y, map, planner)
    {
    },
    onMenuInputDone : function(addIn, map, planner)
    {
    },
    readyToExecute : function(addIn, map, planner)
    {
        return true;
    },
    execute : function(addIn, map, planner)
    {
        return true;
    },
    getStepCursor : function(addIn, cursorData, map, planner)
    {
        return "cursor+default";
    },
    getUiXml : function(addIn, map, planner)
    {
        return "";
    },
};
