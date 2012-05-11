#ifndef OCLOGGER_H
#define OCLOGGER_H

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextOStream>
#include <QTime>

#include <ocobject.h>

#include <iostream>

namespace ormcore {

    #define OC_DEBUG( type, msg ) \
        OCLogger::debug( type, __FILE__, __LINE__, __FUNCTION__, msg )
    
    #define OC_DEBUG_INFO( msg ) \
        OCLogger::debug( OC_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, msg )

    #define OC_DEBUG_WARN( msg ) \
        OCLogger::debug( OC_LOG_WARN, __FILE__, __LINE__, __FUNCTION__, msg )

    #define OC_DEBUG_ERROR( msg ) \
        OCLogger::debug( OC_LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, msg )

    #define OC_DEBUG_FATAL( msg ) \
        OCLogger::debug( OC_LOG_FATAL, __FILE__, __LINE__, __FUNCTION__, msg )

    #define OC_DEBUG_BEGIN \
        OCLogger::debug( OC_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, " - BEGIN" )
    
    #define OC_DEBUG_BEGIN_MSG( msg ) \
        OCLogger::debug( OC_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, QString(" - BEGIN - %1").arg(msg) )
    

    #define OC_DEBUG_END \
        OCLogger::debug( OC_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, " - END" )

    #define OC_DEBUG_END_MSG( msg ) \
        OCLogger::debug( OC_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, QString(" - END - %1").arg(msg) )
    

    typedef enum
    {
        OC_LOG_INFO = 0,
        OC_LOG_WARN,
        OC_LOG_ERROR,
        OC_LOG_FATAL
    } OC_LOG_TYPE;


    /*!
      * Classe que controla o log de mensagens em arquivo ou outros meios
      */
    class OCLogger : public OCObject
    {
    private:
        static QFile *_logFile;
        static bool _isLogActive;
        static int _logLevel;

    public:
        OCLogger();

        /*!
          * Inicializa registro do log
          @param fileName Nome do arquivo para onde serão registradas as mensagens
          */
        static void init( const QString fileName );

        /*!
          * @return Se debug está ativo ou não
          */
        static bool isLogActive(){ return _isLogActive; }
        
        /*!
          Atribui o nível mínimo de log (-1 para todos os níveis)
          @param logLevel O nível de log a ser mostrado
          */
        static void setLogLevel( int logLevel ){ _logLevel = logLevel; }

        /*!
          * Ativa ou desativa geração de log
          * @param isLogActive Indica se o log será ativado ou não (true ou false)
          */
        static void activateDebug( bool activate = true ){ _isLogActive = activate; }


        /*!
          * Encerra gravação do log
          */
        static void end( );

        /*!
          * Registra informações de debug
          @param logType Tipo da mensagem registrada, podendo ser:
          OC_LOG_INFO  Mensagem apenas informativa.
          OC_LOG_WARN  Mensagem de alerta, porém não é erro.
          OC_LOG_ERROR Mensagem de erro, porém não é erro fatal.
          OC_LOG_FATAL Erro fatal, o programa pode fechar ou apresentar instabilidade.
          @param file Nome do arquivo fonte que está sendo executado.
          @param line Linha do arquivo fonte que está sendo executada.
          @param method Método do programa que está sendo executado.
          @param message Mensagem a ser registrada no log.
          */
        static void debug(OC_LOG_TYPE logType, QString file, qint32 line, QString method, QString message);

    };
}

#endif // OCLOGGER_H
