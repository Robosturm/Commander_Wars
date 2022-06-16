var CreateAccountDialog =
{
    primaryPassword : function(value)
    {
        var variables = createAccount.getVariables();
        var passwordVariable = variables.createVariable("password");
        passwordVariable.writeDataString(value);
    },
    confirmPassword : function(value)
    {
        var variables = createAccount.getVariables();
        var passwordVariable = variables.createVariable("primaryPassword");
        passwordVariable.writeDataString(value);
    },
    emailAdress : function(value)
    {
        var variables = createAccount.getVariables();
        var passwordVariable = variables.createVariable("email");
        passwordVariable.writeDataString(value);
    },
    createAccount : function()
    {

    },
};
