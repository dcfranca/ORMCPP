#include <ocexceptionshelper.h>

using namespace ormcore;

OCExceptionsHelper::OCExceptionsHelper()
{
}

void OCExceptionsHelper::logException(OCSQLException ex)
{
    logException( static_cast<OCException>(ex) );
    OC_DEBUG( OC_LOG_ERROR, QString( "Query: %1" ).arg( ex.sqlQuery() ) );
    OC_DEBUG( OC_LOG_ERROR, QString( "SQL Error: %1" ).arg( ex.sqlError().text() ) );
}

void OCExceptionsHelper::logException(OCException ex)
{
    OC_DEBUG( OC_LOG_ERROR, QString("File name: %1").arg( ex.fileName() ) );
    OC_DEBUG( OC_LOG_ERROR, QString("Method name: %1" ).arg( ex.methodName() ) );
    OC_DEBUG( OC_LOG_ERROR, QString("Line: %1" ).arg( ex.lineNo() ) );
    OC_DEBUG( OC_LOG_ERROR, QString("Message: %1").arg( ex.message() ) );
}
