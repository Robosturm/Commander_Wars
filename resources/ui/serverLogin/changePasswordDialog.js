var ChangePasswordDialog =
{
    oldPassword : function(input)
    {
        var variables = changePassword.getVariables();
        var passwordVariable = variables.createVariable("oldPassword");
        passwordVariable.writeDataString(input);
    },
    newPassword : function(input)
    {
        var variables = changePassword.getVariables();
        var passwordVariable = variables.createVariable("password");
        passwordVariable.writeDataString(input);
    },
    confirmPassword : function(input)
    {
        var variables = changePassword.getVariables();
        var confirmPasswordVariable = variables.createVariable("confirmPassword");
        confirmPasswordVariable.writeDataString(input);
    },
    changePassword : function()
    {
        var variables = changePassword.getVariables();
        var oldPasswordVariable = variables.createVariable("oldPassword");
        var passwordVariable = variables.createVariable("password");
        var confirmPasswordVariable = variables.createVariable("confirmPassword");
        var oldPassword =  oldPasswordVariable.readDataString();
        var password =  passwordVariable.readDataString();
        var confirmPassword =  confirmPasswordVariable.readDataString();
        var menu = changePassword.getBaseMenu();
        if (menu.isValidPassword(password) === false)
        {
            changePassword.showMessageBox(qsTr("Invalid password entered. The password needs to contain 8 letters, one capital one small letter a number and a special char."));
        }
        else if (password !== confirmPassword)
        {
            changePassword.showMessageBox(qsTr("Your passwords don't match."));
        }
        else
        {
            ChangePasswordDialog.changeEnableForItems(false);
            menu.changePasswordOnServerAccount(oldPassword, password);
        }
    },
    changeEnableForItems : function(value)
    {
        changePassword.setObjectEnabled("AbortButton", value);
        changePassword.setObjectEnabled("ChangeButton", value);
        changePassword.setObjectEnabled("PasswordBox", value);
        changePassword.setObjectEnabled("ConfirmBox", value);
        changePassword.setObjectEnabled("OldPasswordBox", value);
    },
    onAccountMessage : function(errorCode)
    {
        if (errorCode === GameEnums.LoginError_None)
        {
            changePassword.showMessageBox(qsTr("You're password was changed successfully."));
            var menu = changePassword.getBaseMenu();
            menu.enableServerButtons(true);
            menu.setServerRequestNewPassword(false);
            changePassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_WrongPassword)
        {
            changePassword.showMessageBox(qsTr("Your old password isn't correct."));
            ChangePasswordDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_AccountDoesntExist)
        {
            changePassword.showMessageBox(qsTr("No account with your username was found."));
            ChangePasswordDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_InvalidPasswordReset)
        {
            changePassword.showMessageBox(qsTr("Unable to change password on the server."));
            ChangePasswordDialog.changeEnableForItems(true);
        }
        else
        {
            changePassword.showMessageBox(qsTr("Unknown error happened."));
            ChangePasswordDialog.changeEnableForItems(true);
        }
    },
    onAbort : function()
    {
        var menu = changePassword.getBaseMenu();
        changePassword.exit();
    },
};
