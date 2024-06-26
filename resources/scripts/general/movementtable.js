var MOVEMENTTABLE =
{
    movementpointsTable = [],
    getName : function()
    {
        return qsTr("Unknown");
    },
    getMovementpoints : function(terrain, unit, currentTerrain, trapChecking = false)
    {
        return -1;
    },

    getMovementpointsFromTable : function(terrain, movementpointsTable)
    {
        var terrainID = terrain.getID();
        for (var i = 0; i < movementpointsTable.length; i++)
        {
            if (movementpointsTable[i][0] === terrainID)
            {
               return movementpointsTable[i][1];
            }
        }
        return -1;
    },
    getSupportsFastPfs : function()
    {
        // return false if movepoints aren't the same for the same tile e.g. for hovercrafts
        return true;
    },
};
