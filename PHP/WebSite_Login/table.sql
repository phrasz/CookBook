CREATE DATABASE CustomerDirectory;

CREATE TABLE Customer(
	user_name	VARCHAR(20)		NOT NULL,
	create_date	DATE			NOT NULL,
	password	VARCHAR(255)	NOT NULL,
	last_name	VARCHAR(50),
	first_name	VARCHAR(50),
	street		VARCHAR(50),
	city		VARCHAR(50),
	state		CHAR(2),
	email		VARCHAR(50),
PRIMARY KEY(user_name);)