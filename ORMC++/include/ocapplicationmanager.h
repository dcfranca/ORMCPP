#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <ocmodel.h>

namespace ormcore
{
    class OCApplicationManager : public OCModel
    {
    public:
        static QString _fullSchema;
        static QString _tableName;

        /*!
          * Construtor de OCApplicationManager
          */
        OCApplicationManager();
    };
}

#endif // APPLICATIONMANAGER_H

