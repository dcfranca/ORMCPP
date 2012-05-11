#ifndef OCFILEEXCEPTION_H
#define OCFILEEXCEPTION_H

#include <QString>
#include <QIODevice>

#include <ocexception.h>

namespace ormcore {

    /*!
     * Enum indicando a operação sendo efetuada no arquivo no momento da excessão, podendo ser:
        OC_EXC_FILE_OPEN (Operação de abertura de arquivo)
        OC_EXC_FILE_OPEN (Operação de fechamento de arquivo)
        OC_EXC_FILE_READ (Operação de leitura)
        OC_EXC_FILE_WRITE (Operação de escrita)
     */
    typedef enum {
        OC_EXC_FILE_OPEN = 0,
        OC_EXC_FILE_CLOSE,
        OC_EXC_FILE_READ,
        OC_EXC_FILE_WRITE
    }OC_EXC_FILE;

  /*!
  * Classe para excessões decorrentes da manipulação de arquivos
  @see OCException
  */
    class OCFileException : public OCException
    {
    private:
        QString _filePath;
        OC_EXC_FILE _operation;

    public:
        /*!
          * Construtor da classe, com os seguintes parâmetros:
            @param fileName Nome do arquivo onde foi gerada a excessão
            @param methodName Nome do método onde foi gerada a excessão
            @param message Mensagem da excessão gerada
            @param lineNo Número da linha no arquivo onde foi gerada a excessão
            @param filePath Caminho do arquivo que estava sendo manipulado
            @param operation Operação que estava sendo executada no arquivo
          */
        OCFileException( QString fileName, QString methodName, QString message, quint32 lineNo,
                         QString filePath, OC_EXC_FILE operation):OCException(fileName,methodName,message,lineNo)
        {
            _filePath  = filePath;
            _operation = operation;
        }

        /*!
          @return O caminho do arquivo que estava sendo manipulado e no qual foi gerada uma excessão
          */
        QString filePath(){ return _filePath; }

        /*!
          * @return A String correnspondente para a operação sendo executada no arquivo no momento da excessão
          */
        QString operation()
        {
            switch(_operation)
            {
                case OC_EXC_FILE_OPEN: return QString("OPEN");
                case OC_EXC_FILE_CLOSE:return QString("CLOSE");
                case OC_EXC_FILE_READ: return QString("READ");
                case OC_EXC_FILE_WRITE:return QString("WRITE");
            }
        }
    };
}

#endif // OCFILEEXCEPTION_H
