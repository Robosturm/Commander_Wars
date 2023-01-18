UNIT.canAttackStealthedUnit = function(attacker, defender, map)
{
    if (attacker.getBaseMaxRange() === 1)
    {
        var attackerType = attacker.getUnitType();
        attackerType = UNIT.unitTypeToGround(attackerType);
        var defenderType = defender.getUnitType();
        defenderType = UNIT.unitTypeToGround(defenderType);
        if (attackerType === defenderType)
        {
            return true;
        }
    }
    if (defender.getCloaked() && !defender.getHidden())
    {
        return true;
    }
    return false;
};
