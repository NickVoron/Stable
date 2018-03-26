// Copyright (C) 2017-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

﻿#include "resourceQML.h"
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
	
		
		
		for(auto& property : contextValues.properties)
		{
			nativeResource.rootContext()->setContextProperty(property.first.c_str(), property.second);
		}

		for (auto& object : contextValues.objects)
		{
			nativeResource.rootContext()->setContextProperty(object.first.c_str(), object.second);
		}

		nativeResource.loadData(QByteArray(qmldata.c_str()), QUrl(("file:
		activate();		
	}	
}




// Copyright (C) 2017-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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