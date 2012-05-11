#ifndef OCEXCEPTION_H
#define OCEXCEPTION_H

//Qt
#include <QString>

//ORMC
#include <ocobject.h>
#include <oclogger.h>

namespace ormcore {

    /*!
      * Classe base das excessões
      */
    class OCException : public OCObject
    {
    private:
        QString _fileName;
        QString _methodName;
        QString _message;
        quint32 _lineNo;

    public:
        OCException();

        /*!
          * Construtor da classe, com os seguintes parâmetros:
            @param fileName Nome do arquivo onde foi gerada a excessão
            @param methodName Nome do método onde foi gerada a excessão
            @param message Mensagem da excessão gerada
            @param lineNo Número da linha no arquivo onde foi gerada a excessão
          */
        OCException( QString fileName, QString methodName, QString message, quint32 lineNo )
        {
            OC_DEBUG_BEGIN;

            _fileName = fileName;
            _methodName = methodName;
            _message = message;
            _lineNo = lineNo;

            OC_DEBUG_END;
        }

        /*!
          * @return Nome do arquivo onde foi gerada a excessão
          */
        QString fileName(){ return _fileName; }

        /*!
         * @return Nome do método onde foi gerada a excessão
         */
        QString methodName(){ return _methodName; }

        /*!
         * @return Mensagem da excessão
         */
        QString message(){ return _message; }

        /*!
         * @return Número da linha do arquivo onde foi gerada a excessão
         */
        quint32 lineNo(){ return _lineNo; }
    };
}

#endif // OCEXCEPTION_H
