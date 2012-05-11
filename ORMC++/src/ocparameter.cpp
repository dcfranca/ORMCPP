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
