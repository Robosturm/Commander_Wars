UNIT.repairUnit = function (unit, repairAmount, map)
{
    // repair it
    var costs = unit.getUnitCosts();
    var hp = unit.getHpRounded();
    var healingDone = 0;
    if (hp + repairAmount <= 10)
    {
        healingDone = repairAmount;
    }
    else
    {
        // we could heal more than we need
        healingDone = 10 - hp;
    }
    var funds = map.getCurrentPlayer().getFunds();
    // check if we can pay for all healing
    for (var i = healingDone; i >= 0; i--)
    {
        if (i * costs / 10  <= funds)
        {
            healingDone = i;
            break;
        }
        else if (i === 0)
        {
            healingDone = 0;
            break;
        }
    }
    // heal unit
    unit.setHp(hp + healingDone);
    // pay for healing
    map.getCurrentPlayer().addFunds(-healingDone / 10 * costs);
};
