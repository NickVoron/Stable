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
		contextValues.clear();

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
			userData->store(contextValues);
		}		
	}

	void QML::Compile(stream::ostream& os)
	{
		os << qmldata << url;
	}

	void QML::LoadCompiled(stream::istream& is)
	{
		is >> qmldata >> url;
	
		// нужно два механизма для передачи свойств которые могут быть спрятаны внутри QVaraint и для указателей на QObject
		// для этого нужна структура которая будет хранить и то и другое 
		for(auto& property : contextValues.properties)
		{
			nativeResource.rootContext()->setContextProperty(property.first.c_str(), property.second);
		}

		for (auto& object : contextValues.objects)
		{
			nativeResource.rootContext()->setContextProperty(object.first.c_str(), object.second);
		}

		nativeResource.loadData(QByteArray(qmldata.c_str()), QUrl(("file:///" + url).c_str()));
		activate();		
	}	
}
