#pragma once
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
		// нужно два механизма для передачи свойств которые могут быть спрятаны внутри QVaraint и для указателей на QObject
		// для этого нужна структура которая будет хранить и то и другое 
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
		//std::map<std::string, QVariant> properties;
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