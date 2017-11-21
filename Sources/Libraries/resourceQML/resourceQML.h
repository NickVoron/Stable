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
