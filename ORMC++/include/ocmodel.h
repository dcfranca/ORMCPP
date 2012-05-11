#ifndef OCMODEL_H
#define OCMODEL_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QVariant>
#include <QRegExp>

#include <ocobject.h>
#include <oclogger.h>

namespace ormcore {

    /*!
      * Classe manipulação de objetos do banco de dados
      */
    class OCModel : public OCObject
    {
    private:
        quint64 _id;
        QHash<QString, QVariant> _tableFields;

    public:
        static QString _tableName;
        static QString _fullSchema;

        /*!
          * Construtor da classe
          */
        OCModel();

        /*!
          * @return id do objeto
          */
        quint64 id(){ return _id; }

        /*!
          * Atribui o id do objeto
          @param id Id do objeto
          */
        void setId( quint64 id ){ _id = id; }

        /*!
          * @return Campos da tabela na forma de hash table
          */
        QHash<QString, QVariant>& tableFields(){ return _tableFields; }
    };

};

#endif // OCMODEL_H
