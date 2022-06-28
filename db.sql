CREATE SCHEMA bank ;

use bank;

CREATE TABLE details (
  acc_No INT UNSIGNED NOT NULL,
  acc_Name VARCHAR(45) NOT NULL,
  acc_MobileNo VARCHAR(11) NOT NULL,
  PRIMARY KEY (acc_No),
  UNIQUE INDEX acc_No_UNIQUE (acc_No ASC) );
  
  CREATE TABLE balance (
  acc_No INT UNSIGNED NOT NULL,
  acc_Balance INT UNSIGNED NOT NULL,
  acc_Pwd VARCHAR(45) NOT NULL,
  acc_Pin VARCHAR(45) NOT NULL,
  PRIMARY KEY (acc_No),
  UNIQUE INDEX acc_No_UNIQUE (acc_No ASC) );
