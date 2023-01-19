#include "UserInterface/ShiftWindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

///@FIXME should ideally fix this sometime.
#pragma warning( disable : 4100 )

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);

	QFile styleFile(":/Dark/Main.qss");
	styleFile.open(QFile::ReadOnly);

	QString style(styleFile.readAll());
	a.setStyleSheet(style);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString& locale : uiLanguages) {
		const QString baseName = "Engine_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			a.installTranslator(&translator);
			break;
		}
	}

	a.setWindowIcon(QIcon(":/Dark/Engine.ico"));
	ShiftWindow w;
	w.setWindowState(Qt::WindowMaximized);
	w.show();
	return a.exec();
}