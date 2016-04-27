#include "db-constdef.h"


const QString Queries::createDB =
"CREATE TABLE [Purchases] ( \n\
[ID] INTEGER PRIMARY KEY AUTOINCREMENT, \n\
[Product] TEXT NOT NULL ON CONFLICT FAIL, \n\
[Store] TEXT NOT NULL ON CONFLICT FAIL, \n\
[Count] FLOAT NOT NULL ON CONFLICT FAIL DEFAULT 1, \n\
[Price] FLOAT NOT NULL ON CONFLICT FAIL, \n\
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, \n\
[Discount] BOOL NOT NULL ON CONFLICT FAIL DEFAULT FALSE , \n\
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY, \n\
[Category] TEXT NOT NULL ON CONFLICT FAIL DEFAULT Other);";

const QString Queries::purchasesTable = "Purchases";

const QString Queries::dbType = "QSQLITE";

const QString Queries::product = "Product";
const QString Queries::store = "Store";
const QString Queries::count = "Count";
const QString Queries::price = "Price";
const QString Queries::category = "Category";
const QString Queries::date = "Date";

const QString Queries::sumOnCategories =
"SELECT Category, SUM(Count * Price) sumField\n\
FROM Purchases \n\
GROUP BY Category\n\
ORDER BY sumField DESC;";

const QString Queries::sumOnStores =
"SELECT Store, SUM(Count * Price) sumField\n\
FROM Purchases \n\
GROUP BY Store\n\
ORDER BY sumField DESC;";

const QString Queries::prudctsPrice =
"SELECT Product, Store, Price, Date\n\
FROM Purchases\n\
GROUP BY Product, Store, Price;";

const QString Queries::consumptionByMonth =
"SELECT Sum(Count * Price), strftime('%Y', Date), strftime('%m', Date)\n\
FROM Purchases\n\
GROUP BY strftime('%Y', Date), strftime('%m', Date);";

const QString Queries::distinctProductCount =
"SELECT COUNT(DISTINCT Product)\n\
FROM Purchases;";

const QString Queries::distinctField(QString field)
{
    QString queryString = "SELECT DISTINCT [%1] FROM " + purchasesTable + ";";

    return queryString.arg(field);
}

const QString Queries::accumulateSumByDate(double sum)
{
    QString accumulateSumByDate =
    "SELECT t1.*, \
    (\
      SELECT SUM(t2.Price * t2.Count)\
      FROM Purchases t2\
      WHERE t2.[ID] >= t1.[ID]\
    ) accumulatedSum\
    FROM Purchases t1\
    WHERE accumulatedSum < %1\
    ORDER BY accumulatedSum;";

    return accumulateSumByDate.arg(sum);
}

const QString Queries::insertIntoPurchases(
        QString productValue,
        QString storeValue,
        QString countValue,
        QString priceValue,
        QString categoryValue,
        QString dateValue)
{
    QString queryString =
"INSERT INTO Purchases (%1, %2, %3, %4, %5, %6)\n\
VALUES (\"%7\", \"%8\", \"%9\", \"%10\", \"%11\", \"%12\");";

    return queryString.arg(product)
            .arg(store)
            .arg(count)
            .arg(price)
            .arg(category)
            .arg(date)
            .arg(productValue)
            .arg(storeValue)
            .arg(countValue)
            .arg(priceValue)
            .arg(categoryValue)
            .arg(dateValue);
}
