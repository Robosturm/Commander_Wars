var ForgotPasswordDialog =
{
    emailAdress : function(input)
    {
        var variables = forgotPassword.getVariables();
        var emailVariable = variables.createVariable("email");
        emailVariable.writeDataString(input);
    },
    resetPassword : function()
    {
        var variables = forgotPassword.getVariables();
        var emailVariable = variables.createVariable("email");
        var email =  emailVariable.readDataString();
        var menu = forgotPassword.getBaseMenu();
        if (menu.isValidEmailAdress(email) === false)
        {
            forgotPassword.showMessageBox(qsTr("You entered an invalid email adress."));
        }
        else
        {
            ForgotPasswordDialog.changeEnableForItems(false);
            menu.resetPasswordOnServerAccount(email);
        }
    },
    onAccountMessage : function(errorCode)
    {
        if (errorCode === GameEnums.LoginError_None)
        {
            forgotPassword.showMessageBox(qsTr("An e-mail with a new password was sent to your e-mail account."));
            forgotPassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_InvalidPasswordReset)
        {
            forgotPassword.showMessageBox(qsTr("Invalid e-mail adress which is linked to your account."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_SendingMailFailed)
        {
            forgotPassword.showMessageBox(qsTr("Password has been reset, but the server failed to send a mail with the new password."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
        else
        {
            forgotPassword.showMessageBox(qsTr("Unknown error happened."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
    },
    changeEnableForItems : function(value)
    {
        forgotPassword.setObjectEnabled("EmailBox", value);
        forgotPassword.setObjectEnabled("AbortButton", value);
        forgotPassword.setObjectEnabled("ResetButton", value);
    },
};
