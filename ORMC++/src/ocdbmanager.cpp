#include <ocdbmanager.h>

using namespace ormcore;

QSqlDatabase OCDBManager::_db = QSqlDatabase();

OCDBManager::OCDBManager()
{
}

void OCDBManager::connect()
{
    OC_DEBUG_BEGIN;

    if ( _db.isOpen() )
        return;

    if ( !_db.open() )
        throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error connecting to database: " ) ), OC_DATABASENAME, "", OCDBManager::_db.lastError() );

    OC_DEBUG_END;
}

void  OCDBManager::close()
{
    OC_DEBUG_BEGIN;

    if ( _db.isOpen() )
        _db.close();

    OC_DEBUG_END;
}

void OCDBManager::setDataBaseName(const QString databaseName)
{
    OC_DEBUG_BEGIN;
    
    _db = QSqlDatabase::addDatabase( "QSQLITE" );
    _db.setDatabaseName( databaseName );
    
    OC_DEBUG_END;
}

void OCDBManager::drop()
{
    OC_DEBUG_BEGIN;

    if ( !QFile::remove( _db.databaseName() ) )
        throw OCSQLException( OC_THROW_PARAMETERS( QObject::tr( "Error trying to drop database: " ) ), OC_DATABASENAME, "", OCDBManager::_db.lastError() );

    OC_DEBUG_END;
}
