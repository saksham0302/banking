//Admin password = Bank@Admin

#include <bits/stdc++.h>
#include <mysql.h>                 //These libraries were added by including MySQL server folders to project options
#include <mysqld_error.h>          //and this library by pasting the "libmysql.dll" to the project folder

using namespace std;


long long int stringHashing(string s) {           //password hashing to store the password in secured format

    hash<string> mystdhash;
    return mystdhash(s);

}


class Customer {
    private:

        int choice = 0;
        bool check = true;
        string adminPwd;

        long int No = 0, Balance = 0;
        string accNo= "", accName = "", accPwd = "User@1234", accMobileNo = "", accBalance = "";

        string userAccNo, userAccPwd;

        int qstate = 0;                 //used take the query state (if it was successful or not)

        stringstream ss;                //will be used to store and manipulate the string data members before making queries
        string query = "";

    public:

        //connecting to SQL Server
        MYSQL* connectingToDB() {

            MYSQL* obj;                 //One to manage MySQL session.
            obj = mysql_init(0);
            //host name = localhost, user = root, password = Banking@Admin, database = bank, port = 3306, unix_Socket = NULL, clientflag = 0
            obj = mysql_real_connect(obj, "localhost" , "root", "Banking@Admin", "bank", 3306, NULL, 0);

            if (obj) {
                cout << "*************Logged into database***************\n" <<endl;
                return obj;
            }

            else {
                cout << "Failed connection to database!!!!" <<endl;
                return obj;
            }
        }


        //user option
        void mainMenu() {

            do {

                system("CLS");

                cout << "**********************MENU**********************" << endl;
                cout << "\n1. Admin" << endl;
                cout << "2. User" << endl;
                cout << "3. Exit" << endl;
                cout << "\nEnter your choice: ";
                cin >> choice;

                switch(choice) {
                    case 1 : adminLogin();
                             break;

                    case 2 : userLogin();
                             break;

                    default : exit(0);
                }

            } while(choice);
        }


        //verify details for admin login
        void adminLogin() {

            //limiting attempt to enter password to 3
            int limit = 3;
            check = false;

            system("CLS");

            cout << "******************Admin Login*******************" <<endl;

            while (limit) {

                cout << "\nEnter admin password: ";
                cin >> adminPwd;
                cout << endl;

                if (stringHashing(adminPwd) == 7468711175957342984) {

                    check = true;
                    system("CLS");
                    MYSQL* obj = connectingToDB();
                    adminMenu(obj);
                    break;
                }

                else {

                    limit--;
                    cout << "Wrong password" <<endl;
                    cout << "You only have " << limit << " attempts left" <<endl;
                }
            }

            if (limit <= 0)
                check = true;

            if (check)
                mainMenu();
        }


        //used to make queries to sql server
        int queryToSql (MYSQL* obj, string query) {

            const char* q = query.c_str();       //used to convert the string in c type char pointer
            mysql_query(obj, q);                 //sending the queries to server
            return qstate;
        }


        //used to verify the mobile length whether it is 10 digit or not
        string verifyMobileNoLength() {

            check = true;

            while(check) {

                accMobileNo.clear();
                cin >> accMobileNo;

                bool isNumber = verifyMobileIsNumber(accMobileNo);

                if(accMobileNo.length() == 10 && isNumber) {
                    check = false;
                }

                else
                    cout << "\nPlease enter the mobile number again: ";
            }
            return accMobileNo;
        }


        //used to check whether the input entered in string is a number
        bool verifyMobileIsNumber (string &accMobileNo) {
            for (char &ch : accMobileNo) {
                if (isdigit(ch) == 0) {
                    return false;
                }
            }
            return true;
        }


        //used to verify whether the account number exists in database or not
        string verifyAccNo(MYSQL* obj) {

            MYSQL_RES* res;                 //access different attributes of header mysql.h

            check = true;
            while (check) {

                cin >> No;
                accNo = to_string(No);      //convert numerical account no to string to manipulate for queries
                ss.str("");
                ss << "SELECT acc_No FROM details WHERE acc_No = '" + accNo + "' ";      //sql query
                query.clear();
                query = ss.str();                           //convert to str
                qstate = queryToSql(obj, query);
                res = mysql_store_result(obj);              //store the result of queries

                if (mysql_num_rows(res) == 0)               //to receive the number of rows in query result
                    cout << "\nAccount number does not exist, please enter valid account number: " ;

                else {
                    mysql_free_result(res);
                    check = false;
                }
            }
            return accNo;
        }


        //used to verify whether the passsword matches with account number or not
        string verifyAccPwd(MYSQL* obj) {

            MYSQL_RES* res;
            check = true;

            while (check) {

                cin >> accPwd;
                accPwd = to_string(stringHashing(accPwd));

                ss.str("");
                ss << "SELECT acc_No FROM balance WHERE acc_No = '" + accNo + "' AND acc_Pwd = '" + accPwd + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);
                res = mysql_store_result(obj);

                if (mysql_num_rows(res) == 0)
                    cout << "\nPlease enter correct password of your account: " ;

                else {
                    mysql_free_result(res);                     //used to empty the stored result of queries
                    check = false;
                }
            }
            return accPwd;
        }


        //features for admin
        void adminMenu(MYSQL* obj) {

            if (obj) {

                cout << "*****************Welcome Admin******************" << endl;
                cout << "\n1. Account Creation" <<endl;
                cout << "2. Account Deletion" <<endl;
                cout << "3. Change details of an account" <<endl;
                cout << "4. Check details of an account" <<endl;
                cout << "5. Go back to Main Menu" <<endl;
                cout << "\nEnter your choice: ";

                cin >> choice;
                switch(choice) {

                    case 1 : system("CLS");
                             accountCreation(obj);
                             break;

                    case 2 : system("CLS");
                             accountDeletion(obj);
                             break;

                    case 3 : system("CLS");
                             changeAccountDetailsMenu(obj);
                             break;

                    case 4 : system("CLS");
                             checkAccountDetails(obj);
                             break;

                    default : mysql_close(obj);
                              mainMenu();
                              break;
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //create new accounts
        void accountCreation(MYSQL* obj) {

            if (obj) {

                cout << "Enter the customer's name: ";
                cin.ignore(1, '\n');
                getline(cin, accName, '\n');

                cout << "\nEnter the customer's account number: ";
                MYSQL_RES* res;
                check = true;

                //checking account number existence
                while (check) {

                    cin >> No;
                    accNo = to_string(No);
                    ss.str("");
                    ss << "SELECT acc_No FROM details WHERE acc_No = '" + accNo + "' ";
                    query.clear();
                    query = ss.str();
                    qstate = queryToSql(obj, query);
                    res = mysql_store_result(obj);

                    if (mysql_num_rows(res) != 0)           //to check if the result had any rows in it
                        cout << "\nAccount Number already exists please provide a new one: " ;

                    else {
                        mysql_free_result(res);
                        check = false;
                    }
                }

                cout << "\nEnter the customer's 10 digit mobile number: ";
                accMobileNo = verifyMobileNoLength();

                cout << "\nPress enter to create the account!!!" ;
                cin.ignore();
                cin.get();

                accPwd = to_string(stringHashing(accPwd));        //default password is stored in account

                ss.str("");
                //query to insert in table
                ss << "INSERT INTO details (acc_No, acc_Name, acc_MobileNo) VALUES ('"+ accNo +"','"+ accName +"','"+ accMobileNo +"')";
                query.clear();
                query = ss.str();
                const char* q1 = query.c_str();
                qstate = mysql_query(obj, q1);

                if (qstate == 0) {

                    accBalance = to_string(Balance);               //default balance is stored

                    ss.str("");
                    //as the account is created in table details this query is used to add the rest details in balance table
                    ss << "INSERT INTO balance (acc_No, acc_Balance, acc_Pwd) VALUES ('"+ accNo +"','"+ accBalance +"','"+ accPwd +"')";
                    query.clear();
                    query = ss.str();
                    qstate = queryToSql(obj, query);

                    if (qstate != 0 ) {
                        ss.str("");
                        //if the rest of the details from new account are not added in balance
                        //this query is to remove the added details from details table
                        ss << "DELETE FROM details where acc_No = '"+ accNo +"')";
                        query.clear();
                        query = ss.str();
                        qstate = queryToSql(obj, query);
                        system("CLS");
                        cout << "Failed to insert record!!!\n" <<endl;
                        cout << mysql_error(obj) << "\n" <<endl;
                        cout << "Please enter the details again!!!\n" <<endl;
                        accountCreation(obj);
                    }

                    else {
                        system("CLS");
                        cout << "Account Created!!!\n" <<endl;
                        adminMenu(obj);
                    }
                }

                else {
                    system("CLS");
                    cout << "Failed to insert record!!!\n" <<endl;
                    cout << mysql_error(obj) << "\n" <<endl;
                    cout << "Please enter the details again!!!\n" <<endl;
                    accountCreation(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //delete existing accounts
        void accountDeletion(MYSQL* obj) {

            if (obj) {

                cout << "Enter the account number to delete the account: " ;
                accNo = verifyAccNo(obj);

                cout << "\nPlease ask the customer to enter the account password: " ;
                cin >> accPwd;
                accPwd = to_string(stringHashing(accPwd));

                cout << "\nPress enter to delete the account!!!" ;
                cin.ignore();
                cin.get();

                ss.str("");
                //query to delete account from tables detail and balance
                ss << "DELETE details, balance FROM details inner join balance on details.acc_No = balance.acc_No WHERE (details.acc_No = '" + accNo + "' and balance.acc_Pwd = '" + accPwd + "')";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                if(qstate == 0 && mysql_affected_rows(obj) != 0) {
                    system("CLS");
                    cout << "Account Deleted!!!\n" <<endl;
                    adminMenu(obj);
                }

                else {
                    system("CLS");
                    cout << "Please verify your account password!!!\n" <<endl;
                    accountDeletion(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //options to change details in existing account
        void changeAccountDetailsMenu(MYSQL* obj) {

            if (obj) {
                cout << "*************Change Details*********************" <<endl;
                cout << "\n1. Change name" <<endl;
                cout << "2. Change mobile number" <<endl;
                cout << "3. Go back to admin menu" <<endl;
                cout << "\nEnter your choice: " ;

                cin >> choice;
                switch (choice) {

                    case 1 : system("CLS");
                             changeName(obj);
                             break;

                    case 2 : system("CLS");
                             changeMobileNo(obj);
                             break;

                    default : system("CLS");
                              adminMenu(obj);
                              break;
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //change name in existing account
        void changeName(MYSQL* obj) {

            if (obj) {

                cout << "Enter the account number of customer: " ;
                accNo = verifyAccNo(obj);

                cout << "\nPlease ask the customer to enter the account password: " ;
                accPwd = verifyAccPwd(obj);

                cout << "\nEnter the new name of customer: " <<endl;
                cin.ignore(1, '\n');
                getline(cin, accName, '\n');

                ss.str("");
                //update name in table details
                ss << "UPDATE details, balance SET details.acc_Name = '" + accName + "' WHERE (details.acc_No = '" + accNo + "' and balance.acc_No = '" + accNo + "' and balance.acc_Pwd = '" + accPwd + "')";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                if(qstate == 0 && mysql_affected_rows(obj) != 0) {
                    system("CLS");
                    cout << "Changes made in account!!!\n" <<endl;
                    adminMenu(obj);
                }

                else {
                    system("CLS");
                    cout << "Please verify your account password!!!\n" <<endl;
                    changeName(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //change mobile number in existing account
        void changeMobileNo(MYSQL* obj) {

            if (obj) {

                cout << "Enter the account number of customer: " ;
                accNo = verifyAccNo(obj);

                cout << "\nPlease ask the customer to enter the account password: " ;
                accPwd = verifyAccPwd(obj);

                cout << "\nEnter the new 10 digit mobile number of customer: " <<endl;
                accMobileNo = verifyMobileNoLength();

                ss.str("");
                //update details in table details
                ss << "UPDATE details, balance SET details.acc_MobileNo = '" + accMobileNo + "' WHERE (details.acc_No = '" + accNo + "' and balance.acc_No = '" + accNo + "' and balance.acc_Pwd = '" + accPwd + "')";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                if (qstate == 0 && mysql_affected_rows(obj) != 0) {
                    system("CLS");
                    cout << "Changes made in account!!!\n" <<endl;
                    adminMenu(obj);
                }

                else {
                    system("CLS");
                    cout << "Please verify your account password!!!\n" <<endl;
                    changeMobileNo(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //display details of existing account
        void checkAccountDetails(MYSQL* obj) {

            if (obj) {

                cout << "Enter the account number you want to check details of: " ;
                accNo = verifyAccNo(obj);

                ss.str("");
                //query to receive the details
                ss << "SELECT * FROM details WHERE acc_No = '" + accNo + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                MYSQL_ROW row;
                MYSQL_RES* res;
                res = mysql_store_result(obj);

                if (qstate == 0 && mysql_num_rows(res) != 0) {

                    int Count = mysql_num_fields(res);
                    cout << "\n\nAcc No.\tName\t\tMobile Number\n" <<endl;

                    //used to display each row received as a result in sql query
                    while ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < Count; i++) {
                            cout  << row[i] << "\t";
                        }
                        cout << endl;
                    }

                    cout << "\n\nPress any key to continue." <<endl;
                    cin.ignore();
                    cin.get();
                    system("CLS");
                    mysql_free_result(res);
                    adminMenu(obj);
                }

                else {
                    cout << "\nPlease check the account Number!!!\n" <<endl;
                    checkAccountDetails(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //verify login details for an account
        void userLogin() {

            system("CLS");

            MYSQL* obj = connectingToDB();

            if (obj) {

                cout << "******************User Login********************" <<endl;
                cout << "\nIf you are logging in for the first time the default password is \"User@1234\". \nWe strongly recommend you to change it. " <<endl;
                cout << "\nEnter your account number: " ;

                userAccNo = verifyAccNo(obj);

                cout << "\nEnter your account password: " ;
                userAccPwd = verifyAccPwd(obj);

                cout << "****************Login Successful****************" <<endl;

                userMenu(obj);
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //features for user
        void userMenu(MYSQL* obj) {

            system("CLS");

            cout << "*****************Welcome User*******************" << endl;
            cout << "\n1. Check details of your account" <<endl;
            cout << "2. Change password of your account" <<endl;
            cout << "3. Withdraw money from the account" <<endl;
            cout << "4. Deposit money in the account" <<endl;
            cout << "5. Transfer money to another account" <<endl;
            cout << "6. Go back to Main Menu" <<endl;
            cout << "\nEnter your choice: ";

            cin >> choice;
            switch(choice) {

                case 1 : system("CLS");
                         checkDetails(obj);
                         break;

                case 2 : system("CLS");
                         changePassword(obj);
                         break;

                case 3 : system("CLS");
                         withdrawMoney(obj);
                         break;

                case 4 : system("CLS");
                         depositMoney(obj);
                         break;

                case 5 : system("CLS");
                         transferMoney(obj);
                         break;

                default : mysql_close(obj);
                          mainMenu();
                          break;
            }
        }


        //display the details and balance of the logged in user
        void checkDetails(MYSQL *obj) {

            if (obj) {

                ss.str("");
                //query to receive the details and balance
                ss << "SELECT d.acc_No, d.acc_Name, d.acc_MobileNo, b.acc_Balance FROM details d INNER JOIN balance b ON d.acc_No = b.acc_No  WHERE d.acc_No = '" + userAccNo + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                MYSQL_ROW row;
                MYSQL_RES* res;
                res = mysql_store_result(obj);

                if (qstate == 0 && mysql_num_rows(res) != 0) {

                    int Count = mysql_num_fields(res);
                    cout << "\nAcc No.\tName\t\tMobile Number\tBalance\n" <<endl;

                    //used to display each row received as a result in sql query
                    while ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < Count; i++) {
                            cout  << row[i] << "\t";
                        }
                        cout << endl;
                    }
                    cout << "\n\nPress any key to continue." <<endl;
                    cin.ignore();
                    cin.get();
                    system("CLS");
                    mysql_free_result(res);
                    userMenu(obj);
                }
                else {
                    cout << "Trouble fetching details!!!!" <<endl;
                    cout << "\nPress any key to go back to User Login and please login again." <<endl;
                    cin.ignore();
                    cin.get();
                    mysql_close(obj);
                    userLogin();
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //change the password of the logged in user
        void changePassword(MYSQL *obj) {

            if (obj) {

                string Pwd = "";

                cout << "Please enter new password: " ;
                accPwd.clear();
                cin >> accPwd;
                accPwd = to_string(stringHashing(accPwd));

                cout << "\nConfirm new password: " ;
                cin >> Pwd;
                Pwd = to_string(stringHashing(Pwd));

                if (accPwd == Pwd) {

                    ss.str("");
                    //query to update the password
                    ss << "UPDATE balance SET acc_Pwd = '" + accPwd + "' WHERE acc_No = '" + userAccNo + "' ";
                    query.clear();
                    query = ss.str();
                    qstate = queryToSql(obj, query);

                    if(qstate == 0 && mysql_affected_rows(obj) != 0) {
                        system("CLS");
                        cout << "Password changed!!!\n" <<endl;
                        cout << "\nPress any key to go back to user menu. " <<endl;
                        cin.ignore();
                        cin.get();
                        userMenu(obj);
                    }

                    else {
                        system("CLS");
                        cout << "Couldn't update password. New password same as old password.\n" <<endl;
                        cin.ignore();
                        cin.get();
                        userMenu(obj);
                    }
                }

                //to check if the new password mathces old password
                else {
                    cout << "\nBoth passwords don't match. Please enter the values again.\n" <<endl;
                    changePassword(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }

        }


        //withdraw money from the logged in account
        void withdrawMoney(MYSQL *obj) {

            if (obj) {

                MYSQL_RES* res;

                cout << "Please enter the amount you want to withdraw: " ;
                cin >> Balance;
                accBalance.clear();
                accBalance = to_string(Balance);
                ss.str("");
                //query to check if there is sufficient balance
                ss << "SELECT acc_Balance FROM balance WHERE acc_No = '" + userAccNo + "' AND acc_Balance <= '" + accBalance + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);
                res = mysql_store_result(obj);

                if (qstate == 0 && mysql_num_rows(res) != 0) {

                    cout << "\nInsufficient balance!! Check details of your account." <<endl;
                    cin.ignore();
                    cin.get();
                    mysql_free_result(res);
                    userMenu(obj);
                }

                else {

                    ss.str("");
                    //query to update the balance in balance table
                    ss << "UPDATE balance SET acc_Balance = acc_Balance - '" + accBalance + "' WHERE acc_No = '" + userAccNo + "' ";
                    query.clear();
                    query = ss.str();
                    qstate = queryToSql(obj, query);

                    if (qstate == 0) {
                        cout << "\nRs. " << accBalance << " withdrawn from your account. Press enter to go back to user menu. " <<endl;
                        cin.ignore();
                        cin.get();
                        userMenu(obj);
                    }
                    else {
                        cout << "\nUnable to withdraw money!! Please try again!" <<endl;
                        withdrawMoney(obj);
                    }
                }
            }


            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }

        }


        //deposit money to the logged in account
        void depositMoney(MYSQL *obj) {

            if (obj) {

                cout << "Please enter the amount you want to deposit: " ;
                cin >> Balance;
                accBalance.clear();
                accBalance = to_string(Balance);
                ss.str("");
                //query to update the balance in balance table
                ss << "UPDATE balance SET acc_Balance = acc_Balance + '" + accBalance + "' WHERE acc_No = '" + userAccNo + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);

                if (qstate == 0) {
                    cout << "\nRs. " << accBalance << " deposited to your account. Press enter to go back to user menu. " <<endl;
                    cin.ignore();
                    cin.get();
                    userMenu(obj);
                }
                else {
                    cout << "\nUnable to deposit money!! Please try again!" <<endl;
                    depositMoney(obj);
                }
            }

            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


        //transfer money from logged in account to existing account
        void transferMoney(MYSQL *obj) {

            if (obj) {

                cout << "Enter the account number you want to transfer money in: " ;
                accNo = verifyAccNo(obj);

                cout << "\nPlease enter the amount you want to transfer: " ;
                cin >> Balance;
                accBalance.clear();
                accBalance = to_string(Balance);

                ss.str("");
                //query to check if there is sufficient balance in balance table
                ss << "SELECT acc_Balance FROM balance WHERE acc_No = '" + userAccNo + "' AND acc_Balance < '" + accBalance + "' ";
                query.clear();
                query = ss.str();
                qstate = queryToSql(obj, query);
                MYSQL_RES* res;
                res = mysql_store_result(obj);

                if (qstate == 0 && mysql_num_rows(res) != 0) {

                    cout << "\nTransaction not possible!! " <<endl;
                    cout << "\nInsufficient balance!! Please check your balance deatails." <<endl;
                    cout << "\nPress enter to go back to the user menu." ;
                    cin.ignore();
                    cin.get();
                    mysql_free_result(res);
                    userMenu(obj);
                }

                else {

                    ss.str("");
                    //update the balance in logged in account
                    ss << "UPDATE balance SET acc_Balance = acc_Balance - '" + accBalance + "' WHERE acc_No = '" + userAccNo + "' ";
                    query.clear();
                    query = ss.str();
                    qstate = queryToSql(obj, query);

                    if (qstate == 0) {

                        ss.str("");
                        //update the balance in receiving account
                        ss << "UPDATE balance SET acc_Balance = acc_Balance + '" + accBalance + "' WHERE acc_No = '" + accNo + "' ";
                        query.clear();
                        query = ss.str();
                        int qstate1 = queryToSql(obj, query);

                        if (qstate1 == 0) {

                            cout << "\nRs. " << accBalance << " transferred to account " << accNo << ". Press enter to go back to user menu. " <<endl;
                            cin.ignore();
                            cin.get();
                            userMenu(obj);
                        }
                    }

                    else {
                        cout << "\nUnable to transfer money!! Please try again!" <<endl;
                        transferMoney(obj);
                    }
                }
            }


            else {
                cout << "Failed to fetch database!!1" <<endl;
                cout << "\n" << mysql_error(obj) <<endl;
                exit(0);
            }
        }


};


int main() {

    Customer C;
	C.mainMenu();

	return 0;
}
