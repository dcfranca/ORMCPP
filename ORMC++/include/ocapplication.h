#ifndef OCAPPLICATION_H
#define OCAPPLICATION_H

//Qt
#include <QDateTime>

//ORMC
#include <ocapplicationmanager.h>
#include <ocdbobject.h>
#include <ocapplicationmanager.h>
#include <ocdbheaders.h>

namespace ormcore
{

    class OCApplication : public OCObject
    {
        public:
            static OCDBObject<OCApplicationManager> _appManager;
        
        public:
            OCApplication();
            
            /*!
              * Inicializa aplicação, sincronizando dados com o banco
              */
            static bool initOCApllication(QString applicationName, QString applicationVersion);
            
            /*!
              * Inicializa gerenciador da aplicação no banco de dados 
              @param applicationName QString com o nome da aplicação
              @param applicationVersion QString com a versão da aplicação
              */
            static bool initApplicationManager(QString applicationName, QString applicationVersion);
    
            /*!
              * Fecha gerenciador da aplicação no banco de dados
              */
            static void closeApplicationManager();
            
            static OCDBObject<OCApplicationManager> applicationManager(){ return _appManager; }
    };

}

#endif // OCAPPLICATION_H
