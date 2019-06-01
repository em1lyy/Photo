#include <QApplication>
#include <QTranslator>
#include <QString>
#include <QLockFile>
#include <QDir>
#include <QtDBus/QtDBus>
#include <QMessageBox>
#include "mainwindow.h"
#include "photo-common-dbus.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(":/" + locale);
    a.installTranslator(&translator);
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, QObject::tr("Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`").toStdString().c_str());
        QMessageBox::critical(NULL, "Photo", QObject::tr("Cannot connect to the D-Bus session bus.\nTo start it, run:\n\teval `dbus-launch --auto-syntax`"), QMessageBox::Ok);
        return 1;
    }
    QString tmpDir = QDir::tempPath();
    QLockFile lockFile(tmpDir + "/jagudev-Photo.lock");
    if (lockFile.tryLock(50))
    {
        if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::sessionBus().lastError().message()));
            QMessageBox::critical(NULL, "Photo", QObject::tr("Could not register service to D-Bus session bus."), QMessageBox::Ok);
            exit(1);
        }
        QGuiApplication::setApplicationDisplayName("Photo");
        MainWindow w;
        QDBusConnection::sessionBus().registerObject("/", &w, QDBusConnection::ExportAllSlots);
        if (argc != 2)
        {
            if (argc == 1)
            {
                w.open();
            }
            else
            {
                return 1;
            }
        }
        else if (!w.loadFile(argv[1]))
        {
            return -1;
        }
        w.show();
        return a.exec();
    }
    else
    {
        if (argc == 2)
        {
            QDBusInterface iface(SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
            if (iface.isValid()) {
                iface.call("loadFile", argv[1]);
            } else {
                QMessageBox::critical(NULL, "Photo", QObject::tr("Interface invalid."), QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::critical(NULL, "Photo", QObject::tr("An instance of Photo is already running!"), QMessageBox::Ok);
        }
        return 0;
    }
}
