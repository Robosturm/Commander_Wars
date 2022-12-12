var DeleteAccountDialog =
{
    primaryPassword : function(input)
    {
        var variables = deleteAccount.getVariables();
        var passwordVariable = variables.createVariable("password");
        passwordVariable.writeDataString(input);
    },
    confirmPassword : function(input)
    {
        var variables = deleteAccount.getVariables();
        var confirmPasswordVariable = variables.createVariable("confirmPassword");
        confirmPasswordVariable.writeDataString(input);
    },
    emailAdress : function(input)
    {
        var variables = deleteAccount.getVariables();
        var emailVariable = variables.createVariable("email");
        emailVariable.writeDataString(input);
    },
    deleteAccount : function()
    {
        var variables = deleteAccount.getVariables();
        var passwordVariable = variables.createVariable("password");
        var confirmPasswordVariable = variables.createVariable("confirmPassword");
        var emailVariable = variables.createVariable("email");
        var password =  passwordVariable.readDataString();
        var confirmPassword =  confirmPasswordVariable.readDataString();
        var email =  emailVariable.readDataString();
        var menu = deleteAccount.getBaseMenu();
        if (menu.isValidPassword(password) === false)
        {
            deleteAccount.showMessageBox(qsTr("Invalid password entered. The password needs to contain 8 letters, one capital one small letter a number and a special char."));
        }
        else if (password !== confirmPassword)
        {
            deleteAccount.showMessageBox(qsTr("Your passwords don't match."));
        }
        else if (menu.isValidEmailAdress(email) === false)
        {
            deleteAccount.showMessageBox(qsTr("You entered an invalid email address."));
        }
        else
        {
            DeleteAccountDialog.changeEnableForItems(false);
            menu.deleteServerAccount(password, email);
        }
    },
    changeEnableForItems : function(value)
    {
        deleteAccount.setObjectEnabled("AbortButton", value);
        deleteAccount.setObjectEnabled("AccountButton", value);
        deleteAccount.setObjectEnabled("PasswordBox", value);
        deleteAccount.setObjectEnabled("ConfirmBox", value);
        deleteAccount.setObjectEnabled("EmailBox", value);
    },
    onAccountMessage : function(errorCode)
    {
        if (errorCode === GameEnums.LoginError_None)
        {
            deleteAccount.showMessageBox(qsTr("Account successfully deleted."));
            deleteAccount.exit();
        }
        else if (errorCode === GameEnums.LoginError_WrongPassword)
        {
            deleteAccount.showMessageBox(qsTr("Password is incorrect didn't delete account."));
            DeleteAccountDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_WrongEmailAdress)
        {
            deleteAccount.showMessageBox(qsTr("E-mail address is incorrect didn't delete account."));
            DeleteAccountDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_AccountDoesntExist)
        {
            deleteAccount.showMessageBox(qsTr("An account with the current username doesn't exists."));
            DeleteAccountDialog.changeEnableForItems(true);
        }
        else
        {
            deleteAccount.showMessageBox(qsTr("Unknown error happened while deleting account. Account wasn't deleted."));
            DeleteAccountDialog.changeEnableForItems(true);
        }
    },
};
