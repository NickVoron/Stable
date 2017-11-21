#include "resourceQML.h"
#include "textFile/library.include.h"
#include <boost/algorithm/string/replace.hpp>

#include <QtQuick/QQuickWindow>
#include <QtQml/QQmlContext>
#include "expressionFunctions/filesystem.h"


namespace Resources
{
	void QML::Clear()
	{
		deactivate();

		if(!nativeResource.rootObjects().empty())
		{
			if (QObject* root = nativeResource.rootObjects().first())
			{
				nativeResource.rootObjects().clear();
				QQuickWindow* window = qobject_cast<QQuickWindow*>(root);
				ENFORCE_POINTER(window);
				window->close();
				nativeResource.clearComponentCache();
			}
		}		
	}
	
	void QML::LoadSource(const char* sourceFile, const UserData* userData)
	{
		properties.clear();

		qmldata = TextFile::TextFromFile(sourceFile).text();
		auto filename = boost::filesystem::canonical(boost::filesystem::path(sourceFile));
		url = filename.string();
		boost::replace_all(url, "\\", "/");

		Base::FileUtils::apply_to_files(filename.remove_filename(), "qml", true, [this](auto& file)
		{
			auto filename = boost::filesystem::canonical(file);
			dependencies.addActualDependence(filename, filename);
		});		

		if(userData)
		{
			userData->store(properties);			
		}		
	}

	void QML::Compile(stream::ostream& os)
	{
		os << qmldata << url;
	}

	void QML::LoadCompiled(stream::istream& is)
	{
		is >> qmldata >> url;
	
		for(auto& property : properties)
		{
			nativeResource.rootContext()->setContextProperty(property.first.c_str(), property.second);
		}

		nativeResource.loadData(QByteArray(qmldata.c_str()), QUrl(("file:///" + url).c_str()));
		activate();		
	}	
}
