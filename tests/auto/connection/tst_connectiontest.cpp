#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <client.h>

static QString getVariable(const char *name)
{
    return qgetenv(name);
}

class ConnectionTest: public QObject
{
    Q_OBJECT
private slots:
    void testDirectConnection_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("From enviroment variables VK_LOGIN and VK_PASSWORD")
                << getVariable("VK_LOGIN")
                << getVariable("VK_PASSWORD");

    }

    void testDirectConnection()
    {
        QFETCH(QString, login);
        QFETCH(QString, password);

        vk::Client client(login, password);

        QEventLoop loop;
        connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit()));
        client.connectToHost();
        loop.exec();

        QCOMPARE(client.isOnline(), true);
    }

    void testDirectConnectionWrongData_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("Wrong data")
                << QString("Foo")
                << QString("bar");
    }

    void testDirectConnectionWrongData()
    {
        QFETCH(QString, login);
        QFETCH(QString, password);

        vk::Client client(login, password);

        QEventLoop loop;
        connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit()));
        client.connectToHost();
        loop.exec();

        QCOMPARE(client.isOnline(), false);
    }
};

QTEST_MAIN(ConnectionTest)

#include "tst_connectiontest.moc"
