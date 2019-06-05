RECON.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_RECON_MG");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(80);
    unit.setMaxFuel(80);
    unit.setBaseMovementPoints(8);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(5);
};
RECON.getBaseCost = function()
{
    return 4000;
};
