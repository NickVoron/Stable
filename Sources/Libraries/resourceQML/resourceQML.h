// Copyright (C) 2017-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

﻿#pragma once
#include "resourceManager/library.include.h"
#include "resourceManager/resource.h"

#undef realloc
#include <QtQml/QQmlApplicationEngine>
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#undef foreach

namespace std 
{
	class QVariantHasher 
	{
	public:
		QVariantHasher() : buff(&bb), ds(&buff) 
		{
			bb.reserve(1000);
			buff.open(QIODevice::WriteOnly);
		}

		uint hash(const QVariant& v) 
		{
			buff.seek(0);
			ds << v;
			return qHashBits(bb.constData(), buff.pos());
		}
	private:
		QByteArray bb;
		QBuffer buff;
		QDataStream ds;
	};

	template<>
	class hash<QVariant> 
	{
	public:
		size_t operator()(const QVariant &s) const
		{
			QVariantHasher hasher;
			return hasher.hash(s);
		}
	};
}
namespace Resources
{
	class QML : public ResourceT<QML, QQmlApplicationEngine>
	{
	public:
		
		
		struct ContextValues
		{
			void clear() { properties.clear(); objects.clear(); }
			std::map<std::string, QVariant> properties;
			std::map<std::string, QObject*> objects;
		};

		static const char* basePath() { return "qml/"; }
		static const char* TypeName() { return "QML"; }

		virtual void Clear();

		QQmlApplicationEngine& engine() { return nativeResource; }
	protected:
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadCompiled(stream::istream& is) override;

		ContextValues contextValues;
		
		std::string qmldata;
		std::string url;
		QObject holder;
	};
}


namespace std
{
	template<>
	class hash<Resources::QML::ContextValues>
	{
	public:
		size_t operator()(const Resources::QML::ContextValues& s) const
		{
			size_t seed;
			Resources::hash_combine_container(seed, s.properties);
			return seed;
		}
	};

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