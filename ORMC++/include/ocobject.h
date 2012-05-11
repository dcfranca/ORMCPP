#ifndef OCOBJECT_H
#define OCOBJECT_H

#include <QString>

#include <ocutils.h>
namespace ormcore
{
    /*!
      * Classe abstrata da qual todos as classes do projeto devem herdar
      */
    class OCObject
    {
    private:
        QString _name;

    public:
        /*!
          * Construtor da classe
          @param name Nome da classe como metainformação
          */
        OCObject(QString name="OCObject");
    };
};

#endif // OCOBJECT_H
