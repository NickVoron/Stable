// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "objectConverter.h"


namespace ObjectParser
{
	ClassesLib* ObjectConverter::classes = nullptr;
	Unroller* ObjectConverter::unroller = nullptr;

	void ObjectConverter::convert(const Expressions::Expression& expr, EntitiesStream& client)
	{
		ENFORCE(unroller);
		ENFORCE(classes);

		if (auto prototype = expr.cast<PrototypeHandle>())
		{

			auto& prototypes = classes->prototypes;
			
			void* identifier = (void*)&prototype->prototype;

			if (!prototypes.find(identifier))
			{			 
				ComponentModel::ComponentModelConfigurator configurator;
				Unroller localUnroller(unroller->classes, configurator, false);
				boost::any unrlr(&localUnroller);
				auto evaluatedPrototype = prototype->evaluated(Expressions::ScopeNames(), &unrlr);
				ENFORCE(evaluatedPrototype);
				if (auto instance = evaluatedPrototype->cast<ComponentModel::ComponentModelConfigurator::InstanceHandle>())
				{
					auto& ios = prototypes.create(identifier, instance->classDesc->className).ios;

					EntitiesList entities;
					configurator.configure(localUnroller, entities);
					classes->merge(entities.classes);


					entities.saveObjects(ios);
				}
			}

			client.streamId = identifier;
		}
		else
		{
			LOG_ERROR("type: " << expr.string() << " is not an object");
		}
	}
}

namespace Expressions
{
	template<> void RegisterExpressionConverter<EntitiesStream>() { UserStructsConvertersLib::addInst<ObjectParser::ObjectConverter>(); }
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