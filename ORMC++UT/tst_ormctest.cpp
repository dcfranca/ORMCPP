//Qt
#include <QtCore/QString>
#include <QtTest/QtTest>

//OC
#include <ocdbobject.h>
#include <ocparameter.h>
#include <oclogger.h>
#include <ocsqlexception.h>
#include <ocfileexception.h>
#include <ocexception.h>
#include <ocdbmanager.h>
#include <ocexceptionshelper.h>

using namespace ormcore;

class ORMCUTTest : public QObject
{
        Q_OBJECT
        
    public:
        ORMCUTTest();
        
    private Q_SLOTS:
        void initTestCase();
        void testCaseParameter();
        void cleanupTestCase();
};

ORMCUTTest::ORMCUTTest()
{
}

void ORMCUTTest::initTestCase()
{
    try
    {
        OCLogger::init( "stderr" );

        OCLogger::activateDebug(true);
        //OCLogger::setLogLevel( OC_LOG_WARN );
        OCDBManager::setDataBaseName( "test_ormc.db" );
        OCDBManager::drop();

        QCOMPARE( OCDBManager::_db.databaseName().compare( "test_ormc.db" ), 0 );

    }
    catch( OCSQLException ex )
    {
        OCExceptionsHelper::logException( ex );
    }
    catch( OCException ex )
    {
        OCExceptionsHelper::logException( ex );
    }
    catch (std::exception const& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << __FUNCTION__ << ": Exception not handled!!!" << std::endl;
    }
}

void ORMCUTTest::cleanupTestCase()
{
    OCLogger::end();
}

void ORMCUTTest::testCaseParameter()
{
    
    OC_DEBUG_BEGIN;
    
    OCDBObject<OCParameter>::syncDatabase();
    
    try
    {
        OCDBObject<OCParameter> parameter;
        parameter["settingsItem"] = 1;
        parameter["label"] = QString::fromUtf8("Item de Revisão");
        parameter["widgetType"] = "RADIO";
        parameter["options"] = QString::fromUtf8("Sim,Não,Talvez");
        parameter["defaultValue"] = QString::fromUtf8("Sim");
        parameter["value"] = QString::fromUtf8("Não");
        parameter["itemOrder"] = 3;

        QCOMPARE( parameter["settingsItem"].toInt(), 1 );
        QCOMPARE( parameter["label"].toString(), QString::fromUtf8("Item de Revisão"));
        QCOMPARE( parameter["widgetType"].toString(),QString::fromUtf8("RADIO"));
        QCOMPARE( parameter["options"].toString(), QString::fromUtf8("Sim,Não,Talvez"));
        QCOMPARE( parameter["defaultValue"].toString(), QString::fromUtf8("Sim") );
        QCOMPARE( parameter["value"].toString(), QString::fromUtf8( "Não" ) );
        QCOMPARE( parameter["itemOrder"].toInt(), 3 );

        parameter.save();
        QVERIFY( parameter.id() > 0 );

        quint64 lastId = parameter.id();

        parameter["itemOrder"] = 5;
        QCOMPARE( parameter["itemOrder"].toInt(), 5 );
        parameter.save();

        QCOMPARE( lastId, parameter.id() );

        OCDBObject<OCParameter> parameter2;
        QVERIFY( parameter2.get( lastId ) );

        QCOMPARE( parameter2["label"].toString(), QString::fromUtf8("Item de Revisão"));
        QCOMPARE( parameter2["widgetType"].toString(),QString::fromUtf8("RADIO"));
        QCOMPARE( parameter2["options"].toString(), QString::fromUtf8("Sim,Não,Talvez"));
        QCOMPARE( parameter2["defaultValue"].toString(), QString::fromUtf8("Sim") );
        QCOMPARE( parameter2["value"].toString(), QString::fromUtf8( "Não" ) );
        QCOMPARE( parameter2["itemOrder"].toInt(), 5 );
        QCOMPARE( parameter2["settingsItem"].toInt(), 1 );
        parameter2.save();

        OCDBObject<OCParameter> parameter3;
        parameter3["settingsItem"] = 4;
        parameter3["label"] = QString::fromUtf8("Verificar padrões");
        parameter3["widgetType"] = "CHECKBOX";
        parameter3["options"] = QString::fromUtf8("");
        parameter3["defaultValue"] = QString::fromUtf8("");
        parameter3["value"] = QString::fromUtf8("Teste");
        parameter3["itemOrder"] = 10;
        parameter3.save();

        QList< OCDBObject<OCParameter> > parameters = OCDBObject<OCParameter>::objects();
        QCOMPARE( parameters.count(), 2 );
        
        //Testar novos métodos exists
        QVERIFY( parameter3.existsSameValues() );
        QVERIFY( parameter3.exists( QString::fromUtf8("label = 'Verificar padrões'") ) );
        QVERIFY( parameter3.exists( QString::fromUtf8("label = 'Verificar padrões' AND value = 'Teste'") ) );
        QCOMPARE( parameter3.exists( QString::fromUtf8("label = 'Verificar padrões' AND value = 'Teste2'") ), false );
        
        parameter3["settingsItem"] = 10;
        parameter3["label"] = QString::fromUtf8("Novo label");
        
        QCOMPARE( parameter3.existsSameValuesExcept(), false );
        QVERIFY( parameter3.existsSameValuesExcept( QStringList() << "settingsItem" << "label" << "options" << "permissions" << "defaultValue" ) );
        
        QVERIFY( parameter3.getSameValuesExcept( QStringList() << "settingsItem" << "label" << "options" << "permissions" << "defaultValue" ) );
        
        QVERIFY( parameter3.getSameValues( QStringList() << "widgetType" << "itemOrder" ) );
        
        parameter3["label"] = "Teste Inexistente";
        QCOMPARE( parameter3.getSameValues( QStringList() << "label" ), false );
        QVERIFY( parameter3.getSameValues( QStringList() << "widgetType" << "value" << "itemOrder" ) );
        
        parameter3["label"] = "Teste Inexistente";
        parameter3.save();
        
        QVERIFY( parameter3.getSameValuesExcept() );
        parameter3.saveIfNotExistsExcept();
        
        QCOMPARE( OCDBObject<OCParameter>::objects( "label = 'Teste Inexistente'" ).count(), 1 );

    }
    catch( OCSQLException ex )
    {
        OCExceptionsHelper::logException( ex );
    }
    catch( OCException ex )
    {
        OCExceptionsHelper::logException( ex );
    }
    catch(...)
    {
        std::cerr << __FUNCTION__ << ": Exception not handled!!!" << std::endl;
    }
    
    OC_DEBUG_END;
}




QTEST_APPLESS_MAIN(ORMCUTTest)

#include "tst_ormctest.moc"
