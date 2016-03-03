CREATE TABLE [PurchasesTemp] ( 
[ID] INTEGER PRIMARY KEY AUTOINCREMENT, 
[ProductName] TEXT NOT NULL ON CONFLICT FAIL, 
[StoreName] TEXT NOT NULL ON CONFLICT FAIL, 
[Count] INT NOT NULL ON CONFLICT FAIL DEFAULT 1, 
[Price] FLOAT NOT NULL ON CONFLICT FAIL, 
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, 
[Discount] BOOL NOT NULL ON CONFLICT FAIL DEFAULT FALSE , 
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY, 
[Category] TEXT NOT NULL ON CONFLICT FAIL DEFAULT Other);
INSERT INTO PurchasesTemp SELECT * FROM Purchases;
DROP TABLE Purchases;
ALTER TABLE PurchasesTemp RENAME TO Purchases;