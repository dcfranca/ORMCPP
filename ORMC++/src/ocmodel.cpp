#include "ocmodel.h"

using namespace ormcore;

QString OCModel::_tableName = "";
QString OCModel::_fullSchema = "";

OCModel::OCModel()
{
    _id = 0;
    //loadFields();
}

