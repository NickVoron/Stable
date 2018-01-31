// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "resourceManager/library.include.h"

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
		static const char* basePath() { return "qml/"; }
		static const char* TypeName() { return "QML"; }

		virtual void Clear();

	protected:
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadCompiled(stream::istream& is) override;

		std::map<std::string, QVariant> properties;
		std::string qmldata;
		std::string url;
		QObject holder;
	};
}




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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