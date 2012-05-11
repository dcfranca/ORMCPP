#ifndef OCEXCEPTIONSHELPER_H
#define OCEXCEPTIONSHELPER_H

#include <ocobject.h>
#include <ocexception.h>
#include <ocsqlexception.h>
#include <ocfileexception.h>
#include <oclogger.h>

namespace ormcore
{

    class OCExceptionsHelper : public OCObject
    {
        public:
            OCExceptionsHelper();
            
            static void logException(OCSQLException ex);
            static void logException(OCException ex);
    };
}

#endif // OCEXCEPTIONSHELPER_H
