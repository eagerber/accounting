CREATE TABLE [ExchangeRate] ( 
[ID] INTEGER PRIMARY KEY AUTOINCREMENT, 
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, 
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY,
[Price] FLOAT NOT NULL ON CONFLICT FAIL);
