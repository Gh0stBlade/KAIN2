#include "UserInterface/ShiftWindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>

///@FIXME should ideally fix this sometime.
#pragma warning( disable : 4100 )

#if 0
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	if (!Editor::CreateEditorWindow(hInstance, nShowCmd, windowWidth, windowHeight, hWnd)) {
		printf("Failed to create editor window!\n");
		return false;
	}

	if (!g_engine.getRenderer()->InitialiseD3D11(windowWidth, windowHeight, hWnd)) {
		printf("Failed to initialise D3D11!\n");
		return false;
	}

	if (!Editor::UI::Initalise(hWnd)) {
		printf("Failed to initialise editor ui!\n");
		return false;
	}

	if (!Editor::InitScene()) {
		printf("Failed to initialise scene!\n");
		return false;
	}

	Editor::UI::ApplyTheme();

	if (!g_engine.m_initialised) {
		g_engine.Initialise();
	}

	Editor::MessageLoop();
	
	Editor::ReleaseObjects();

	Editor::UI::Shutdown();

	return 0;
}
#else


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

#endif