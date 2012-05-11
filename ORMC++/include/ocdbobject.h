#ifndef OCDBOBJECT_H
#define OCDBOBJECT_H

//Containers
#include <QString>
#include <QHashIterator>
#include <QList>
#include <QVariant>

//SQL
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

//ORMC
#include <ocobject.h>
#include <ocmodel.h>
#include <ocutils.h>
#include <oclogger.h>
#include <ocdbmanager.h>

//Exceptions
#include "ocsqlexception.h"

namespace ormcore {

    /*!
      * Template base dos objetos do banco de dados.
      */
    template<class T>
    class OCDBObject : public OCObject
    {
    private:
        T _model;

        /*!
          * Atualiza informações do objeto na base de dados
            É necessário que o objeto exista previamente na base
            Método de uso interno da classe
            @throw OCSQLException
          */
        void update()
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            QString fieldsSet;

            bool first = true;

            QHashIterator<QString, QVariant> it(_model.tableFields());
            while (it.hasNext())
            {
                it.next();
                if ( first )
                    first = false;
                else
                    fieldsSet.append( ", " );

                fieldsSet.append( QString( " %1 = '%2' " ).arg( it.key() ).arg( it.value().toString() ) );

            }

            QString strUpdate = QString( "UPDATE %1 SET %2 WHERE id = %3 " ).arg( _model._tableName ).arg( fieldsSet ).arg( _model.id() );

            OC_DEBUG( OC_LOG_INFO, QString( "Update SQL: %1" ).arg( strUpdate ) );

            QSqlQuery sqlQuery( OCDBManager::_db );
            if ( !sqlQuery.exec( strUpdate ) )
            {
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error updating database" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            }

            OCDBManager::close();

            OC_DEBUG_END;
        }


        /*!
          * Cria o objeto no banco de dados (ignora existência prévia)
            Método de uso interno da classe
            @throw OCSQLException
          */
        void create()
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            QString fieldsInsert;
            QString valuesInsert;

            bool first = true;

            QHashIterator<QString, QVariant> it(_model.tableFields());
            while (it.hasNext()) {
                it.next();
                
                if ( first )
                    first = false;
                else
                {
                    fieldsInsert.append( ", " );
                    valuesInsert.append( ", " );
                }

                fieldsInsert.append( it.key() );
                valuesInsert.append( QString("'%1'").arg( it.value().toString() ) );
            }

            QString strQuery = QString( "INSERT INTO %1 ( %2 ) VALUES ( %3 )").arg( _model._tableName ).arg( fieldsInsert ).arg( valuesInsert );

            OC_DEBUG( OC_LOG_INFO, QString( "Query: %1" ).arg( strQuery ) );

            QSqlQuery sqlQuery(OCDBManager::_db);
            if ( !sqlQuery.exec( strQuery ) )
            {
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error inserting in database" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            }

            _model.setId( sqlQuery.lastInsertId().toInt() );

            OCDBManager::close();

            OC_DEBUG_END;
        }

        /*!
          * @return Campos da tabela relativa ao objeto
          */
        QHash<QString, QVariant>& tableFields(){ return _model.tableFields(); }


    public:
        /*!
          * Construtor da classe
          Testa se o objeto passado para o template é o correto (precisa herdar de OCModel).
          @see OCModel
          */
        OCDBObject( ){ LoadFields(); }

        /*!
          * Destrutor da classe
          */
        virtual ~OCDBObject(){ }

        enum
        {
            OC__PARSER_COLUMN = 0,
            OC_PARSER_TYPE
        };

        static QHash<QString, QString> parseFields(QString schema)
        {
            schema = schema.trimmed();
            QString schemaParsed;

            bool token = OC__PARSER_COLUMN;
            bool ignoreNewSpaces = false;
            bool doneField = false;

            for( int ind = 0; ind < schema.length(); ind++ )
            {
                if ( schema[ind] == ' ' && token == OC__PARSER_COLUMN )
                {
                    schemaParsed.append( schema[ind] );
                    ignoreNewSpaces = true;
                    token = OC_PARSER_TYPE;
                }
                else if ( schema[ind] == ',' )
                {
                    doneField = true;
                    schemaParsed.append( schema[ind] );
                }
                else if ( schema[ind] != ' ' )
                {
                    if ( doneField )
                    {
                        token = OC__PARSER_COLUMN;
                        ignoreNewSpaces = false;
                        doneField = false;
                    }

                    schemaParsed.append( schema[ind] );
                }
            }

            QStringList lines = schemaParsed.split(",");
            QHash<QString, QString> parsedFields;

            foreach( QString line, lines )
            {
                QString field = line.split(" ")[0];
                QString type  = line.split(" ")[1];
                parsedFields[field] = type;
            }

            return parsedFields;
        }

        /*!
          * Adiciona colunas à tabela
          * @param oldFields Campos da tabela antiga
          * @param newFields Campos da tabela nova
          * @throw OCSQLException
          */
        static void addColumns( QHash<QString, QString> oldFields, QHash<QString, QString> newFields )
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            QHashIterator<QString, QString> itNew(newFields);
            while (itNew.hasNext())
            {
                itNew.next();

                if ( !oldFields.contains( itNew.key() ) )
                {
                    //Adiciona coluna
                    QString strAddColumn = QString( "ALTER TABLE %1 ADD COLUMN %2 %3" ).arg( T::_tableName ).arg( itNew.key() ).arg( itNew.value() );
                    //OC_DEBUG( OC_LOG_INFO, OC_THROW_PARAMETERS( QObject::trUtf8( "Adicionando coluna %1 na tabela %2" ).arg( itNew.key() ).arg( T::_tableName ) ) );

                    QSqlQuery sqlAddColumn(OCDBManager::_db);

                    if ( !sqlAddColumn.exec( strAddColumn ) )
                    {
                        throw new OCSQLException( OC_THROW_PARAMETERS( "Error adding column" ),
                                                  OCDBManager::_db.databaseName(), sqlAddColumn.lastQuery(), sqlAddColumn.lastError() );

                    }
                }
            }

            OCDBManager::close();

            OC_DEBUG_END;
        }

        /*!
          * Recria tabela para remover campos
          * @param oldFields Campos da tabela antiga
          * @param newFields Campos da tabela nova
          * @throw OCSQLException
          */
        static void recreateTable( QHash<QString, QString> oldFields, QHash<QString, QString> newFields )
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            QString fields = " id ";
            qint16  numDropColumns = 0;
            QString fullSql;

            QHashIterator<QString, QString> itOld(oldFields);
            while (itOld.hasNext())
            {
                itOld.next();

                //Coluna permanece
                if ( newFields.contains( itOld.key() ) )
                    fields = fields.append( ", %1" ).arg( itOld.key() );
                else
                    numDropColumns++;
            }

            if ( numDropColumns == 0 )
            {
                OC_DEBUG( OC_LOG_INFO, "Não existem colunas pra remover" );
                return;
            }

            QSqlQuery sqlQuery(OCDBManager::_db);

            fullSql = "BEGIN TRANSACTION;";
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error to begin transaction" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "CREATE TEMPORARY TABLE t_backup( %1 );" ).arg( fields );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error creating temporary table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "INSERT INTO t_backup SELECT %1 FROM %2;" ).arg( fields ).arg( T::_tableName );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error inserting data in temporary table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );


            fullSql = QString( "DROP TABLE %1;" ).arg( T::_tableName );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( "Error droping old table" ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "CREATE TABLE %1 (  id INTEGER PRIMARY KEY AUTOINCREMENT, %2 );" ).arg( T::_tableName ).arg( T::_fullSchema );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error recreating table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "INSERT INTO %1 SELECT %2 FROM t_backup;" ).arg( T::_tableName ).arg( fields );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error inserting data from temporary table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "DROP TABLE t_backup;" );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error droping temporary table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            fullSql = QString( "COMMIT;" );
            if ( !sqlQuery.exec( fullSql ) )
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::trUtf8( "Error to commit transaction" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            sqlQuery.finish();

            OCDBManager::close();

            OC_DEBUG_END;
        }

        /*!
          * Criar tabela no banco de dados
          * @throw OCSQLException
          */
        static void createTable()
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            OC_DEBUG(OC_LOG_INFO,QString("Creating table: %1").arg( T::_tableName ));

            QString strQuery = QString("CREATE TABLE %1 ( id INTEGER PRIMARY KEY AUTOINCREMENT, %2 )").arg( T::_tableName ).arg( T::_fullSchema );

            QSqlQuery sqlQuery(OCDBManager::_db);
            if ( !sqlQuery.exec( strQuery ) )
            {
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error creating table" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );
            }

            OCDBManager::close();

            OC_DEBUG_END;
        }


        /*!
         * Retornar nomes das colunas do banco de dados
         * @return nomes das colunas em uma QHash
         * @throw OCSQLException
         */
        static QHash<QString, QString> getTableColumns( )
        {
            OC_DEBUG_BEGIN;

            QHash<QString, QString> tableFields;

            OCDBManager::connect();

            QString strExists = QString( "SELECT sql FROM sqlite_master WHERE type = 'table' AND name = '%1'" ).arg( T::_tableName );
            QSqlQuery sqlQuery(OCDBManager::_db);

            if ( !sqlQuery.exec( strExists ) )
            {
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error checking if table exists" ) ),
                                          OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );

            }

            //Se tabela existe
            if (sqlQuery.next())
            {
                QString schema = sqlQuery.value(0).toString();

                int firstIndex = schema.indexOf( "," );
                int numChars  = schema.lastIndexOf( ")" )-firstIndex;

                QString stripSchema = schema.mid( firstIndex+1, numChars-1 );

                tableFields  = parseFields( stripSchema );
            }
            else
                return QHash<QString, QString>();

            sqlQuery.finish();

            OCDBManager::close();
            
            OC_DEBUG_END;

            return tableFields;
        }

        /*!
          * Sincroniza a base de dados, ou seja, cria a tabela equivalente ao modelo, caso ela não exista.
          @throw OCSQLException
          */
        static void syncDatabase()
        {
            OC_DEBUG_BEGIN;

            QHash<QString, QString> oldFields = getTableColumns();
            QHash<QString, QString> newFields = parseFields( T::_fullSchema );

            //Se tabela já existe
            if ( oldFields.count() > 0 )
            {
                //Adiciona ou remove campos se necessário
                addColumns(oldFields, newFields);
                recreateTable(oldFields, newFields);
            }
            else
            {
                //Nao existe, criar tabela no banco
                createTable();
            }

            OC_DEBUG_END;
        }


        /*!
          * Carrega o objeto do banco do dados com base no id passado como parâmetro.
          @throw OCSQLException
          @param id Identificador do objeto no banco de dados.
          @return Valor booleano indicando se o objeto foi carregado ou não.
          */
        bool get(qint64 id)
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            QString strQuery = "SELECT id ";

            QHashIterator<QString, QVariant> it(_model.tableFields());
            while (it.hasNext()) {
                it.next();
                strQuery.append( "," );
                strQuery.append( it.key() );
            }
            strQuery.append( QString(" FROM %1 WHERE id = %2 ").arg(_model._tableName).arg( id ) );
            
            OC_DEBUG(OC_LOG_INFO, strQuery);

            QSqlQuery sqlQuery(OCDBManager::_db);
            if ( !sqlQuery.exec( strQuery ) )
            {
                OC_DEBUG_END;
                throw OCSQLException( OC_THROW_PARAMETERS( QString( QObject::tr("Error retrieving item ID=%1").arg( id ) ) ), OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );
            }

            if (sqlQuery.next())
            {
                OC_DEBUG( OC_LOG_INFO, QObject::trUtf8("Model found: %1").arg( _model._tableName ) );

                _model.setId( id );
                QHashIterator<QString, QVariant> it(_model.tableFields());
                while (it.hasNext()) {
                    it.next();
                    int fieldNo = sqlQuery.record().indexOf(it.key());
                    QVariant var = sqlQuery.value(fieldNo);
                    /*QRegExp regInteger("[\\d]+");
                    QRegExp regDecimal("[\\d]+\\.[\\d]+");

                    if ( regDecimal.indexIn( var.toString() ) > -1 )
                        setValue( it.key(),var.toDouble());
                    else if ( regInteger.indexIn( var.toString() ) > -1 )
                        setValue( it.key(),var.toInt());
                    else*/
                    setValue( it.key(),var);

                    //_model.tableFields()[it.key] = sqlQuery.value(fieldNo);
                }

                OCDBManager::close();

                OC_DEBUG_END;
                return true;
            }

            OC_DEBUG( OC_LOG_WARN,QObject::trUtf8("Model not found: %1").arg( _model._tableName ) );

            OCDBManager::close();

            OC_DEBUG_END;

            return false;
        }

        /*!
          *  Carrega campos baseado no schema
          */
        void LoadFields()
        {
            OC_DEBUG_BEGIN;

            _model.tableFields().clear();
            if ( T::_fullSchema.length() == 0 )
            {
                OC_DEBUG(OC_LOG_WARN,QObject::trUtf8("no such Schema found to table [%1]").arg(T::_tableName));
                OC_DEBUG_END;
                return;
            }

            QStringList lines = T::_fullSchema.split( "," );

            foreach( QString line, lines )
            {
                QString fieldName = line.split( " ", QString::SkipEmptyParts )[0];
               _model.tableFields().insert( fieldName, QVariant() );
            }

            OC_DEBUG_END;
        }

        /*!
          * Retorna o id do objeto
          @return id do objeto
          */
        quint64 id(){ return _model.id(); }
        
        /*!
          * Atribui o valor do id do objeto
          @param id do objeto
          */
        void setId(quint64 id){ _model.setId( id ); }

        /*!
          * Atribui o valor value para o campo field do objeto
          @param field Campo do objeto ao qual será atribuído o novo valor
          @param value Novo valor a ser assinalado para o campo field
          */
        void setValue( QString field, QVariant value ){ _model.tableFields()[field] = value; }

        /*!
          * Sobrecarga do operador [] para atribuir ou recuperar o valor de um campo do objeto
            Mantendo uma equivalencia com o nome do campo na base de dados.
            @param key Nome do campo na base de dados
            @return O valor armazenado pelo campo
          */
        QVariant& operator[]( QString key )
        {
            //OC_DEBUG_BEGIN;

            if ( !_model.tableFields().contains( key ) )
            {
                throw OCException( OC_THROW_PARAMETERS( QObject::trUtf8("Primary key not found: %1 for table: %2" ).arg( key ).arg( _model._tableName ) ) );
            }
            
            //OC_DEBUG_END;

            return _model.tableFields()[ key ];
        }
        
        /*!
          * Salva registro apenas se já não existe na base de dados.
          @param fields Campos para comparar quando verificar se já existe
          */
        void saveIfNotExists(QStringList fields = QStringList())
        {
            OC_DEBUG_BEGIN;
            
            if ( !existsSameValues( fields ) )
            {
                OC_DEBUG( OC_LOG_INFO, QString( "Not found in database, saving." ) );
                save();
            }
            
            OC_DEBUG_END;
        }        
        
        /*!
          * Salva registro apenas se já não existe na base de dados.
          @param exeptions Campos para não comparar quando verificar se já existe
          */
        void saveIfNotExistsExcept(QStringList exceptions = QStringList())
        {
            OC_DEBUG_BEGIN;
            
            if ( !existsSameValuesExcept( exceptions ) )
            {
                OC_DEBUG( OC_LOG_INFO, QString( "Not found in database, saving." ) );
                save();
            }
            
            OC_DEBUG_END;
        }
        

        /*!
          * Salva as alterações na base de dados, caso o registro ainda não exista então é feita uma inserção dos dados na base.
          @throw OCSQLException
          */
        void save()
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            bool exists = false;

            if ( _model.id() > 0 )
            {
                QString strExists = QString( "SELECT 1 FROM %1 WHERE id = %2 " ).arg( _model._tableName ).arg( _model.id() );

                OC_DEBUG( OC_LOG_INFO, QString( "Query Exists: %1" ).arg( strExists ) );

                QSqlQuery sqlQueryExists(OCDBManager::_db);
                if ( !sqlQueryExists.exec( strExists ) )
                {
                    throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error checking if item is in database" ) ),
                                              OCDBManager::_db.databaseName(), sqlQueryExists.lastQuery(), sqlQueryExists.lastError() );

                }

                exists = sqlQueryExists.next();
            }

            if (exists)
            {
                OC_DEBUG( OC_LOG_INFO, QString( QObject::tr("Object already exists: %1 [UPDATE]") ).arg( _model._tableName) );
                this->update();
            }
            else
            {
                OC_DEBUG( OC_LOG_INFO, QString( QObject::tr("Object doesn't exist: %1 [INSERT]") ).arg( _model._tableName ) );
                this->create();
            }

            OCDBManager::close();

            OC_DEBUG_END;
        }
        
        /*!
          * Verifica se existe já um registro no banco de dados com os mesmos valores do objeto, com os campos passados como parâmetro
            e carrega o primeiro valor encontrado
          @param fields Campos para comparar
          @return Retorna verdadeiro caso já exista o registro e tenha sido carregado, ou falso em caso negativo
          */
        bool getSameValues( QStringList fields = QStringList(), QString orderBy = QString() )
        {
            OC_DEBUG_BEGIN;
            
            QString whereClause;
            QHashIterator<QString, QVariant> it(_model.tableFields());
            
            while (it.hasNext())
            {
                it.next();
                
                if ( !fields.contains( it.key() ) )
                    continue;
                
                if ( !whereClause.isEmpty() )
                    whereClause.append( " AND " );
                
                whereClause.append( QString( "%1 = '%2'" ).arg( it.key() ).arg( it.value().toString() ) );
            }
            
            QList<OCDBObject<T> > listObjects = objects( whereClause, orderBy );
            
            if ( listObjects.count() == 0 )
                return false;
            
            OC_DEBUG_END;
            
            return get( listObjects.first().id() );
        }            
        
        /*!
          * Verifica se existe já um registro no banco de dados com os mesmos valores (Exceto id, que pode ser diferente) e exceto campos passados como parâmetro
            e carrega o primeiro valor encontrado
          @param fieldsExcept Campos para ignorar na comparação
          @return Retorna verdadeiro caso já exista o registro e tenha sido carregado, ou falso em caso negativo
          */
        bool getSameValuesExcept( QStringList fieldsExcept = QStringList(), QString orderBy = "" )
        {
            OC_DEBUG_BEGIN;
            
            QString whereClause;
            QHashIterator<QString, QVariant> it(_model.tableFields());
            
            while (it.hasNext())
            {
                it.next();
                
                if ( fieldsExcept.contains( it.key() ) )
                    continue;
                
                if ( !whereClause.isEmpty() )
                    whereClause.append( " AND " );
                
                whereClause.append( QString( "%1 = '%2'" ).arg( it.key() ).arg( it.value().toString() ) );
            }
            
            QList<OCDBObject<T> > listObjects = objects( whereClause, orderBy );
            
            if ( listObjects.count() == 0 )
                return false;
            
            OC_DEBUG_END;
            
            return get( listObjects.first().id() );
        } 
        
        /*!
          * Verifica se existe já um registro no banco de dados com os mesmos valores (Exceto id, que pode ser diferente) e exceto campos passados como parâmetro
          @param fieldsExcept Campos para ignorar na comparação
          @return Retorna verdadeiro caso já exista o registro, ou falso em caso negativo
          */
        bool existsSameValues( QStringList fields = QStringList() )
        {
            OC_DEBUG_BEGIN;
            
            QString whereClause;
            QHashIterator<QString, QVariant> it(_model.tableFields());
            
            while (it.hasNext())
            {
                it.next();
                
                if ( !fields.contains( it.key() ) )
                    continue;
                
                if ( !whereClause.isEmpty() )
                    whereClause.append( " AND " );
                
                whereClause.append( QString( "%1 = '%2'" ).arg( it.key() ).arg( it.value().toString() ) );
            }
            
            OC_DEBUG_END;
            
            return exists( whereClause );
        }
        
        
        /*!
          * Verifica se existe já um registro no banco de dados com os mesmos valores (Exceto id, que pode ser diferente) e exceto campos passados como parâmetro
          @param fieldsExcept Campos para ignorar na comparação
          @return Retorna verdadeiro caso já exista o registro, ou falso em caso negativo
          */
        bool existsSameValuesExcept( QStringList fieldsExcept = QStringList() )
        {
            OC_DEBUG_BEGIN;
            
            QString whereClause;
            QHashIterator<QString, QVariant> it(_model.tableFields());
            
            while (it.hasNext())
            {
                it.next();
                
                if ( fieldsExcept.contains( it.key() ) )
                    continue;
                
                if ( !whereClause.isEmpty() )
                    whereClause.append( " AND " );
                
                whereClause.append( QString( "%1 = '%2'" ).arg( it.key() ).arg( it.value().toString() ) );
            }
            
            OC_DEBUG_END;
            
            return exists( whereClause );
        }
        
        /*!
          * Verifica se existe o valor no banco de dados com a whereClause informada como parâmetro.
          @param  whereClause Clausula where no banco de dados sem o "WHERE"
          @return Verdadeiro caso exista, ou falso em caso negativo
          */
        bool exists( QString whereClause = "" )
        {
            return ( objects( whereClause ).count() > 0 );
        }

        /*!
          * Filtra dados da base de dados com base nos paramêtros
          @param  whereClause Clausula where no banco de dados sem o "WHERE"
          @return Lista de objetos retornados do banco de dados
          @throw OCSQLException
          */
        static QList< OCDBObject<T> > objects( QString whereClause = "", QString orderBy = "" )
        {
            OC_DEBUG_BEGIN;

            OCDBManager::connect();

            OCDBObject<T> newObject;

            QString strQuery = "SELECT id ";

            QHashIterator<QString, QVariant> it(newObject.tableFields());
            while (it.hasNext()) {
                it.next();
                strQuery.append( "," );
                strQuery.append( it.key() );
            }

            if ( !whereClause.isEmpty() )
                strQuery.append( QString( " FROM %1 WHERE %2 " ).arg( T::_tableName ).arg( whereClause ) );
            else
                strQuery.append( QString( " FROM %1 " ).arg( T::_tableName ) );
            
            if ( !orderBy.isEmpty() )
                strQuery.append( QString( " ORDER BY %1" ).arg( orderBy ) );
            
            OC_DEBUG( OC_LOG_INFO, strQuery );

            QSqlQuery sqlQuery(OCDBManager::_db);
            if ( !sqlQuery.exec( strQuery ) )
            {
                throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error while filtering items" ) ), OCDBManager::_db.databaseName(), sqlQuery.lastQuery(), sqlQuery.lastError() );
            }

            QList< OCDBObject<T> > listObjects;

            while (sqlQuery.next())
            {
                QHashIterator<QString, QVariant> it(newObject.tableFields());
                newObject.setId( sqlQuery.value(0).toUInt() );
                while (it.hasNext()) {
                    it.next();
                    int fieldNo = sqlQuery.record().indexOf(it.key());
                    newObject[it.key()] = sqlQuery.value(fieldNo);
                }
                listObjects.append( newObject );
            }

            OC_DEBUG( OC_LOG_INFO, QObject::trUtf8( "Number of records found: %1" ).arg( listObjects.count() ) );

            OCDBManager::close();

            OC_DEBUG_END;

            return listObjects;
        }

    };





} //namespace

#endif // OCDBOBJECT_H
