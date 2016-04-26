#ifndef DB_CONSTDEF_H
#define DB_CONSTDEF_H

#include <QString>


class Queries
{
public:
    static const QString createDB;

    static const QString purchasesTable;
    static const QString dbType;

    static const QString product;
    static const QString store;
    static const QString count;
    static const QString price;
    static const QString category;
    static const QString date;

    static const QString sumOnCategories;
    static const QString sumOnStores;
    static const QString prudctsPrice;
    static const QString consumptionByMonth;
    static const QString distinctProductCount;

    static const QString distinctField(QString field);
    static const QString accumulateSumByDate(double sum);

    static const QString insertIntoPurchases(QString productValue,
        QString storeValue,
        QString countValue,
        QString priceValue,
        QString categoryValue,
        QString dateValue);
};

#endif // DB_CONSTDEF_H
