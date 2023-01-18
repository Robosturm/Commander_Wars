var UserLoginDialog =
{
    createAccountText : function()
    {
        return qsTr("Create account");
    },
    createAccountTooltipText : function()
    {
        return qsTr("Creates a new account on the server.");
    },

    loginText : function()
    {
        return qsTr("Login");
    },
    loginTooltipText : function()
    {
        return qsTr("Logs you onto the server with the given userdata.");
    },
    forgotPassword : function()
    {
        return qsTr("Forgot password");
    },
    forgotPasswordTooltip : function()
    {
        return qsTr("Allows you to reset your password.");
    },
    back : function()
    {
        return qsTr("Back");
    },
    backTooltip : function()
    {
        return qsTr("Goes back to the main menu.");
    },
    local : function()
    {
        return qsTr("Local");
    },
    localTooltip : function()
    {
        return qsTr("Play a game in your local network.");
    },
    changePassword : function()
    {
        return qsTr("Change password");
    },
    changePasswordTooltip : function()
    {
        return qsTr("Allows you to change your password.");
    },
    deleteText : function()
    {
        return qsTr("Delete");
    },
    deleteTooltip : function()
    {
        return qsTr("Let's you delete your account on the servers.");
    },
    loginOnServerText : function()
    {
        return qsTr("Login on Server");
    },
    usernameText : function()
    {
        return qsTr("Username:");
    },
    passwordText : function()
    {
        return qsTr("Password:");
    },
    passwordHelpText : function()
    {
        return qsTr("Password of your account.");
    },

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
        UserLoginDialog.changeEnableForItems(false);
        menu.loginToServerAccount(password);
        settings.setServerPassword(password);
    },    
    onAccountMessage : function(errorCode)
    {
        var menu = userLogin.getBaseMenu();
        if (errorCode === GameEnums.LoginError_None)
        {
            userLogin.showMessageBox(qsTr("Logged onto the server."));
            menu.onLogin();
            userLogin.exit();
        }
        else if (errorCode === GameEnums.LoginError_WrongPassword)
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("Your password isn't correct."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_AccountDoesntExist)
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("No account with your username was found."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_PasswordOutdated)
        {
            settings.setServerPassword("");
            menu.setServerRequestNewPassword(true);
            userLogin.createDialog("changePassword", "ui/serverLogin/changePasswordDialog.xml", menu);
            userLogin.exit();
        }
        else
        {
            settings.setServerPassword("");
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
