#include <oclogger.h>
#include <ocfileexception.h>

using namespace ormcore;

bool OCLogger::_isLogActive = true;
int OCLogger::_logLevel = -1;
QFile *OCLogger::_logFile = new QFile( );

OCLogger::OCLogger()
{
}

void OCLogger::init( const QString fileName )
{
    if ( _logFile->isOpen() )
        _logFile->close();

    if ( fileName.compare( "stdout" ) == 0 )
    {
        if ( !_logFile->open( stdout, QIODevice::WriteOnly ) )
        {
            throw OCFileException( OC_THROW_PARAMETERS( "Error opening log in stdout" ),
                                      fileName, OC_EXC_FILE_OPEN );
            return;
        }
    }
    else if ( fileName.compare( "stderr" ) == 0 )
    {
        if ( !_logFile->open( stderr, QIODevice::WriteOnly ) )
        {
            throw OCFileException( OC_THROW_PARAMETERS( "Error opening log in stderr" ),
                                      fileName, OC_EXC_FILE_OPEN );
            return;
        }
    }
    else
    {
        OCLogger::_logFile->setFileName( fileName );

        if ( !_logFile->open( QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append ) )
        {
            throw OCFileException( OC_THROW_PARAMETERS( "Erro opening log file" ),
                                      fileName, OC_EXC_FILE_OPEN );
            return;
        }
    }
    
    //TODO: Logar informações do ambiente (variaveis, programas instalados, codecs)
    OC_DEBUG( OC_LOG_INFO, QObject::tr( "Starting" ) );
}

void OCLogger::end( )
{
    if ( _logFile->isOpen() )
        _logFile->close();
}

void OCLogger::debug(OC_LOG_TYPE logType, QString file, qint32 line, QString method, QString message)
{
    if ( _logFile == NULL )
        return;
    
    if ( !_logFile->isOpen() || !_isLogActive || logType < _logLevel )
        return;

    QString typeString;
    switch(logType)
    {
    case OC_LOG_INFO:
        typeString.append("INFO");
        break;
    case OC_LOG_WARN:
        typeString.append("WARN");
        break;
    case OC_LOG_ERROR:
        typeString.append("ERROR");
        break;
    case OC_LOG_FATAL:
        typeString.append("FATAL");
        exit(-1);
        break;
    }

    QTextStream out(_logFile);

    QString formatText = QString( "%1 - %2:%3 - %4 |%5: %6\n").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")).arg( file ).arg( line ).arg( method ).arg( typeString ).arg( message );
    out << formatText;

}
