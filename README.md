# Banking-System
This is a Banking-System Project which is a console application made using OOPs concept. It is connected with the local MYSQL server and the data is saved on the database of this server.  There are two major roles here :
### 1. Admin
### 2. User

Following operations are performed in this project:
1. Opening an account
2. Deleting an Account
3. Amount Deposit
4. Amount Withdrawal
5. Amount Trasfer

All the Essential Functionalities are password and pin protected. And hashing is also used to store the same in database in hashed format.



### Running the Project
#### Prerequisite :
1. `g++` Compiler
2. `MySQL Server`
3. Add `MySQL Server 8.0` in the project debugger to add additional header files for compilation of code
4. Add `libmysql.dll` in the debug folder of project

### Functionalities for Admin :
1. Account Creation
2. Account Deletion
3. Change details of Account Holder (only, Name and Mobile Number)
4. View account information (except user password and pin)

### Functionalities for User :
1. View account information
2. Update password
3. Update pin
4. Amount Withdrawal
5. Amount Deposit
6. Transfer Amount to another account
