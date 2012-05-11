#ifndef OCSLEXCEPTION_H
#define OCSLEXCEPTION_H

//Qt
#include <QString>

//SQL
#include <QtSql/QSqlError>

//Meus includes
#include <ocexception.h>

namespace ormcore {

    /*!
      * Classe de exceções relacionadas a comandos SQL e acesso ao banco de dados.
      */
    class OCSQLException : public OCException
    {
    private:
        QString _dbName;
        QString _sqlQuery;
        QSqlError _sqlError;

    public:
        OCSQLException( QString fileName, QString methodName, QString message, quint32 lineNo,
                        QString dbName, QString sqlQuery, QSqlError sqlError ):OCException( fileName, methodName, message, lineNo )
        {
            OC_DEBUG_BEGIN;

            _sqlQuery = sqlQuery;
            _sqlError = sqlError;
            _dbName   = dbName;

            OC_DEBUG_END;
        }

        /*!
          * @return Nome do banco de dados que está conectado.
            @return Query SQL que estava sendo executada.
            @return Ultimo erro de SQL ocorrido.
          */
        QString   dbName(){ return _dbName; }
        QString   sqlQuery(){ return _sqlQuery; }
        QSqlError sqlError(){ return _sqlError; }
    };

}

#endif // OCSLEXCEPTION_H
