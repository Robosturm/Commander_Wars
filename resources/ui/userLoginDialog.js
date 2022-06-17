var UserLoginDialog =
{
    primaryPassword : function(input)
    {
        var variables = userLogin.getVariables();
        var passwordVariable = variables.createVariable("password");
        passwordVariable.writeDataString(input);
    },
    login : function()
    {
        var variables = userLogin.getVariables();
        var passwordVariable = variables.createVariable("password");
        var password =  passwordVariable.readDataString();
        var menu = userLogin.getBaseMenu();
        if (menu.isValidPassword(password) === false)
        {
            userLogin.showMessageBox(qsTr("Invalid password entered. The password needs to contain 8 letters, one capital one small letter a number and a special char."));
        }
        else
        {
            UserLoginDialog.changeEnableForItems(false);
            menu.loginToServerAccount(password);
        }
    },
    onAccountMessage : function(errorCode)
    {
        if (errorCode === GameEnums.LoginError_None)
        {
            userLogin.showMessageBox(qsTr("Logged onto the server."));
            var menu = userLogin.getBaseMenu();
            menu.enableServerButtons(true);
            userLogin.exit();
        }
        else if (errorCode === GameEnums.LoginError_WrongPassword)
        {
            userLogin.showMessageBox(qsTr("Your password isn't correct."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_AccountDoesntExist)
        {
            userLogin.showMessageBox(qsTr("No account with your username was found."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else
        {
            userLogin.showMessageBox(qsTr("Unknown error happened."));
            UserLoginDialog.changeEnableForItems(true);
        }
    },
    changeEnableForItems : function(value)
    {
        userLogin.setObjectEnabled("PasswordBox", value);
        userLogin.setObjectEnabled("CreateAccountButton", value);
        userLogin.setObjectEnabled("LoginButton", value);
        userLogin.setObjectEnabled("ForgotPasswordButton", value);
        userLogin.setObjectEnabled("LocalButton", value);
    },
    exit : function()
    {
        userLogin.getBaseMenu().leaveServer();
        userLogin.exit();
    },
};
