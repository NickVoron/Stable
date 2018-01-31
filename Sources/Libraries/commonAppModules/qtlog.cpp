// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "qtlog.h"

#if __has_include("QtCore/QtGlobal")
#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>
#include <QtCore/QString>

#ifdef _DEBUG
#pragma comment(lib, "libEGLd.lib")
#pragma comment(lib, "libGLESv2d.lib")
#pragma comment(lib, "Qt5AccessibilitySupportd.lib")
#pragma comment(lib, "Qt5AxBased.lib")
#pragma comment(lib, "Qt5AxContainerd.lib")
#pragma comment(lib, "Qt5AxServerd.lib")
#pragma comment(lib, "Qt5Bluetoothd.lib")
#pragma comment(lib, "Qt5Concurrentd.lib")
#pragma comment(lib, "Qt5Cored.lib")
#pragma comment(lib, "Qt5DBusd.lib")
#pragma comment(lib, "Qt5DesignerComponentsd.lib")
#pragma comment(lib, "Qt5Designerd.lib")
#pragma comment(lib, "Qt5DeviceDiscoverySupportd.lib")
#pragma comment(lib, "Qt5EglSupportd.lib")
#pragma comment(lib, "Qt5EventDispatcherSupportd.lib")
#pragma comment(lib, "Qt5FbSupportd.lib")
#pragma comment(lib, "Qt5FontDatabaseSupportd.lib")
#pragma comment(lib, "Qt5Gamepad.lib")
#pragma comment(lib, "Qt5Gamepadd.lib")
#pragma comment(lib, "Qt5Guid.lib")
#pragma comment(lib, "Qt5Helpd.lib")
#pragma comment(lib, "Qt5Locationd.lib")
#pragma comment(lib, "Qt5Multimediad.lib")
#pragma comment(lib, "Qt5MultimediaQuick_pd.lib")
#pragma comment(lib, "Qt5MultimediaWidgetsd.lib")
#pragma comment(lib, "Qt5Networkd.lib")
#pragma comment(lib, "Qt5Nfcd.lib")
#pragma comment(lib, "Qt5OpenGLd.lib")
#pragma comment(lib, "Qt5OpenGLExtensionsd.lib")
#pragma comment(lib, "Qt5PacketProtocold.lib")
#pragma comment(lib, "Qt5PlatformCompositorSupportd.lib")
#pragma comment(lib, "Qt5Positioningd.lib")
#pragma comment(lib, "Qt5PrintSupportd.lib")
#pragma comment(lib, "Qt5Qmld.lib")
#pragma comment(lib, "Qt5QmlDebugd.lib")
#pragma comment(lib, "Qt5QuickControls2d.lib")
#pragma comment(lib, "Qt5Quickd.lib")
#pragma comment(lib, "Qt5QuickParticlesd.lib")
#pragma comment(lib, "Qt5QuickTemplates2d.lib")
#pragma comment(lib, "Qt5QuickTestd.lib")
#pragma comment(lib, "Qt5QuickWidgetsd.lib")
#pragma comment(lib, "Qt5Scxmld.lib")
#pragma comment(lib, "Qt5Sensorsd.lib")
#pragma comment(lib, "Qt5SerialBusd.lib")
#pragma comment(lib, "Qt5SerialPortd.lib")
#pragma comment(lib, "Qt5Sqld.lib")
#pragma comment(lib, "Qt5Svgd.lib")
#pragma comment(lib, "Qt5Testd.lib")
#pragma comment(lib, "Qt5ThemeSupportd.lib")
#pragma comment(lib, "Qt5UiToolsd.lib")
#pragma comment(lib, "Qt5WebChanneld.lib")
#pragma comment(lib, "Qt5WebSocketsd.lib")
#pragma comment(lib, "Qt5WebViewd.lib")
#pragma comment(lib, "Qt5Widgetsd.lib")
#pragma comment(lib, "Qt5WinExtrasd.lib")
#pragma comment(lib, "Qt5Xmld.lib")
#pragma comment(lib, "Qt5XmlPatternsd.lib")
#pragma comment(lib, "Qt53DAnimationd.lib")
#pragma comment(lib, "Qt53DCored.lib")
#pragma comment(lib, "Qt53DExtrasd.lib")
#pragma comment(lib, "Qt53DInputd.lib")
#pragma comment(lib, "Qt53DLogicd.lib")
#pragma comment(lib, "Qt53DQuickAnimationd.lib")
#pragma comment(lib, "Qt53DQuickd.lib")
#pragma comment(lib, "Qt53DQuickExtrasd.lib")
#pragma comment(lib, "Qt53DQuickInputd.lib")
#pragma comment(lib, "Qt53DQuickRenderd.lib")
#pragma comment(lib, "Qt53DQuickScene2Dd.lib")
#pragma comment(lib, "Qt53DRenderd.lib")
#pragma comment(lib, "qtfreetyped.lib")
#else
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "Qt5AccessibilitySupport.lib")
#pragma comment(lib, "Qt5AxBase.lib")
#pragma comment(lib, "Qt5AxContainer.lib")
#pragma comment(lib, "Qt5AxServer.lib")
#pragma comment(lib, "Qt5Bluetooth.lib")
#pragma comment(lib, "Qt5Bootstrap.lib")
#pragma comment(lib, "Qt5Concurrent.lib")
#pragma comment(lib, "Qt5Core.lib")
#pragma comment(lib, "Qt5DBus.lib")
#pragma comment(lib, "Qt5Designer.lib")
#pragma comment(lib, "Qt5DesignerComponents.lib")
#pragma comment(lib, "Qt5DeviceDiscoverySupport.lib")
#pragma comment(lib, "Qt5EglSupport.lib")
#pragma comment(lib, "Qt5EventDispatcherSupport.lib")
#pragma comment(lib, "Qt5FbSupport.lib")
#pragma comment(lib, "Qt5FontDatabaseSupport.lib")
#pragma comment(lib, "Qt5Gui.lib")
#pragma comment(lib, "Qt5Help.lib")
#pragma comment(lib, "Qt5Location.lib")
#pragma comment(lib, "Qt5Multimedia.lib")
#pragma comment(lib, "Qt5MultimediaQuick_p.lib")
#pragma comment(lib, "Qt5MultimediaWidgets.lib")
#pragma comment(lib, "Qt5Network.lib")
#pragma comment(lib, "Qt5Nfc.lib")
#pragma comment(lib, "Qt5OpenGL.lib")
#pragma comment(lib, "Qt5OpenGLExtensions.lib")
#pragma comment(lib, "Qt5PacketProtocol.lib")
#pragma comment(lib, "Qt5PlatformCompositorSupport.lib")
#pragma comment(lib, "Qt5Positioning.lib")
#pragma comment(lib, "Qt5PrintSupport.lib")
#pragma comment(lib, "Qt5Qml.lib")
#pragma comment(lib, "Qt5QmlDebug.lib")
#pragma comment(lib, "Qt5QmlDevTools.lib")
#pragma comment(lib, "Qt5Quick.lib")
#pragma comment(lib, "Qt5QuickControls2.lib")
#pragma comment(lib, "Qt5QuickParticles.lib")
#pragma comment(lib, "Qt5QuickTemplates2.lib")
#pragma comment(lib, "Qt5QuickTest.lib")
#pragma comment(lib, "Qt5QuickWidgets.lib")
#pragma comment(lib, "Qt5Scxml.lib")
#pragma comment(lib, "Qt5Sensors.lib")
#pragma comment(lib, "Qt5SerialBus.lib")
#pragma comment(lib, "Qt5SerialPort.lib")
#pragma comment(lib, "Qt5Sql.lib")
#pragma comment(lib, "Qt5Svg.lib")
#pragma comment(lib, "Qt5Test.lib")
#pragma comment(lib, "Qt5ThemeSupport.lib")
#pragma comment(lib, "Qt5UiTools.lib")
#pragma comment(lib, "Qt5WebChannel.lib")
#pragma comment(lib, "Qt5WebSockets.lib")
#pragma comment(lib, "Qt5WebView.lib")
#pragma comment(lib, "Qt5Widgets.lib")
#pragma comment(lib, "Qt5WinExtras.lib")
#pragma comment(lib, "Qt5Xml.lib")
#pragma comment(lib, "Qt5XmlPatterns.lib")
#pragma comment(lib, "Qt53DAnimation.lib")
#pragma comment(lib, "Qt53DCore.lib")
#pragma comment(lib, "Qt53DExtras.lib")
#pragma comment(lib, "Qt53DInput.lib")
#pragma comment(lib, "Qt53DLogic.lib")
#pragma comment(lib, "Qt53DQuick.lib")
#pragma comment(lib, "Qt53DQuickAnimation.lib")
#pragma comment(lib, "Qt53DQuickExtras.lib")
#pragma comment(lib, "Qt53DQuickInput.lib")
#pragma comment(lib, "Qt53DQuickRender.lib")
#pragma comment(lib, "Qt53DQuickScene2D.lib")
#pragma comment(lib, "Qt53DRender.lib")
#pragma comment(lib, "qtfreetype.lib")
#endif

#define SETUP_HANDLER_QT_LOG_HANDLER
#endif


namespace AppModules
{
#ifdef SETUP_HANDLER_QT_LOG_HANDLER
	void MessageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
		bool decorated = true;
		std::string logtext;

		{
			QByteArray localMsg = msg.toLocal8Bit();
			if (context.file && context.function)
			{
				decorated = false;
				logtext = str::spaced(context.file, context.line, str::spaced(context.function, ":", localMsg.constData()));
			}
			else
			{
				logtext = localMsg.constData();
			}
		}		

		auto logout = [decorated, &logtext]()
		{
			if (decorated)
			{
				LOG_MSG(logtext);
			}				
			else
			{
				LOG_MSG_UNDECORATED(logtext);
			}				
		};

		auto errout = [decorated, &logtext]()
		{
			if (decorated)
			{
				LOG_ERROR(logtext);
			}				
			else
			{
				LOG_ERROR_UNDECORATED(logtext);
			}				
		};

		switch (type)
		{
		case QtDebugMsg: logout();	break;
		case QtInfoMsg:  logout();	break;
		case QtWarningMsg:	logout(); break;
		case QtCriticalMsg:	errout(); break;
		case QtFatalMsg: errout();	abort();
		}
	}
#endif

	char* argv [] = { "application" };
	int argc = 1;

	void QtLog::init_impl(const std::vector<std::string>& commandLine)
	{
#ifdef SETUP_HANDLER_QT_LOG_HANDLER
		qInstallMessageHandler(MessageOutputHandler);
#endif

#ifdef QT_IS_PRESENT
		qtapplication.reset(new QApplication(argc, argv));
		
		std::thread([this]
		{
			qtapplication->exec();
		}).detach();
#endif
	}

	void QtLog::release_impl()
	{
#ifdef QT_IS_PRESENT
		if (qtapplication)
		{
			qtapplication->exit(0);
			qtapplication = nullptr;
		}
#endif
	}
}




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.