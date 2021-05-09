ZCOUNIT_ROYAL_GUARD.startOfTurn = function(unit)
{
    var owner = unit.getOwner();
    var playerId  = owner.getPlayerID();
    ZCOUNIT_ROYAL_GUARD.addBuff(unit, playerId);
    var fields = globals.getCircle(1, 1);
    var unitX = unit.getX();
    var unitY = unit.getY();
    for (var i = 0; i < fields.size(); i++)
    {
        var x = fields.at(i).x + unitX;
        var y = fields.at(i).y + unitY;
        if (map.onMap(x, y))
        {
            var targetUnit = map.getTerrain(x, y).getUnit();
            if (targetUnit !== null)
            {
                var targetOwner = targetUnit.getOwner();
                if (targetOwner === owner)
                {
                    ZCOUNIT_ROYAL_GUARD.addBuff(targetUnit, playerId);
                }
                else if (targetOwner.isAlly(owner))
                {
                    var variables = owner.getVariables();
                    var boostVariable = variables.createVariable("COOP_MOD_ROYAL_GUARD_UNITS");
                    var boosts = boostVariable.readDataListInt32();
                    var unitId = targetOwner.getUniqueID();
                    boosts.push(unitId);
                    boostVariable.writeDataListInt32(boosts);
                    ZCOUNIT_ROYAL_GUARD.addBuff(targetUnit, playerId, 2);
                }
            }
        }
    }
    fields.remove();
};
