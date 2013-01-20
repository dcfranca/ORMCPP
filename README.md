ORMC++
======
An object-relational mapping for C++/Qt and SQLite.

Usage
-----------
First of all you've to tell what is the database name with a single line:
```C++
OCDBManager::setDataBaseName( "database_name.db" );
```

Then create a class that inherits from OCModel,(use OCParameter.h/OCParameter.cpp as example).

header
-------
```C++
#ifndef OCPARAMETER_H
#define OCPARAMETER_H

#include <QString>

#include <ocmodel.h>

namespace ormcore { //Main namespace

    class OCParameter : public OCModel
    {
    public:
        //Required members
	   static QString _fullSchema;
        static QString _tableName;

        /*!
          * Construtor de OCParameter
          */
        OCParameter( );

    };
}

#endif // OCPARAMETER_H
```

source
------
```C++
#include <ocparameter.h>

using namespace ormcore;

QString OCParameter::_tableName = "parameter";
QString OCParameter::_fullSchema =
"settingsItem VARCHAR(50),"
"name VARCHAR(50),"
"label VARCHAR(300),"
"widgetType VARCHAR(20),"
"options VARCHAR(1024),"
"defaultValue VARCHAR(255),"
"value VARCHAR(255),"
"permissions VARCHAR(1024),"
"extensionName  VARCHAR(100),"
"acceptNull BOOLEAN,"
"regexValidator VARCHAR(250),"
"itemOrder INTEGER";

OCParameter::OCParameter()
    :OCModel()
{
}
```

As you can see, you have to initialize <i>_tableName</i> and <i>_fullSchema</i> members, assign the <i>_fullschema</i> member with the names and types of the table.

To synchronize the table with the database simply call: <b>OCDBObject<OCParameter>::syncDatabase();</b>

<b>Important</b>: The template argument for OCDBObject must be a OCModel subclass.

Now when you want to create a new object in  database use this:
```C++
OCDBObject<OCParameter> parameter;
```

<b>Important</b>: The argument for OCDBObject template must be a subclass model.

Use the table column as a index-syntax way for the object, like:

```C++
parameter["settingsItem"] = 1;
parameter["widgetType"] = "RADIO";
```

To save(insert or update) the item use the save method:
```C++
parameter.save(); //When you create the object in the database, the Id() member is updated with the real value
```
Related: <b>saveIfNotExists</b>, <b>saveIfNotExistsExcept</b>

To retrieve a parameter by Id, use:
```C++
parameter.get( myId )
```

To list all the records in the database:
```C++
QList< OCDBObject<OCParameter> > parameters = OCDBObject<OCParameter>::objects();
```
You can pass a where clause and order by clause as argument

You can check if there's the record in database using
existsSameValues //Same values for fields according to a QStringList as argument

<b>existsSameValuesExcept</b> //Same values except a QStringList of fields to ignore in the comparison

Related: <b>getSameValues</b>, <b>getSameValuesExcept</b>


