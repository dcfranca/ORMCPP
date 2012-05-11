#ifndef OCDBMANAGER_H
#define OCDBMANAGER_H

//Qt
#include <QSqlDatabase>

//ORMC
#include <ocsqlexception.h>
#include <ocobject.h>
#include <oclogger.h>

namespace ormcore {

    class OCDBManager : public OCObject
    {
    public:
        OCDBManager();

        static QSqlDatabase _db;

        /*!
          * Atribui o nome do banco de dados
          @param databaseName Nome do banco de dados
          */
        static void setDataBaseName(const QString databaseName);

        /*!
          * Conecta no banco de dados
          @return Valor booleano indicando se a conexão foi executada com sucesso ou não
          @throw OCSQLException
          */
        static void connect();

        /*!
          * Fecha a conexão com o banco de dados, caso exista uma aberta.
          */
        static void close();

        /*!
          * Dropa toda a base de dados
          */
        static void drop();
    };

}

#endif // OCDBMANAGER_H
