#ifndef OCPARAMETER_H
#define OCPARAMETER_H

#include <QString>

#include <ocmodel.h>

namespace ormcore {

    class OCParameter : public OCModel
    {
    public:
        static QString _fullSchema;
        static QString _tableName;

        /*!
          * Construtor de OCParameter
          */
        OCParameter( );

    };
}

#endif // OCPARAMETER_H
