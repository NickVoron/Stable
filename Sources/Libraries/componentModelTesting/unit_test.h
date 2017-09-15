// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "library.include.h"
#include "unitTest/library.include.h"

TEST_SUITE(ComponentModelUnitTest)

TEST_CASE(RootDirSetter) { Base::FileUtils::setModuleDirAsRoot(); }
TEST_CASE(ComponentAndProperties) { ComponentModelTesting::ComponentAndProperties(); }
TEST_CASE(ComponentLink) { ComponentModelTesting::ComponentLink(); }
TEST_CASE(ComponentAsParameter) { ComponentModelTesting::ComponentAsParameter(); }
TEST_CASE(FunctionCall) { ComponentModelTesting::FunctionCall(); }
TEST_CASE(CrossReference) { ComponentModelTesting::CrossReference(); }
TEST_CASE(DataArray) { ComponentModelTesting::DataArray(); }
TEST_CASE(DataStruct) { ComponentModelTesting::DataStruct(); }
TEST_CASE(ComplexData) { ComponentModelTesting::ComplexData(); }
TEST_CASE(ObjectArray) { ComponentModelTesting::ObjectArray(); }
TEST_CASE(NestedObject) { ComponentModelTesting::NestedObject(); }
TEST_CASE(References) { ComponentModelTesting::References(); }
TEST_CASE(CirclePoints) { ComponentModelTesting::CirclePoints(); }
TEST_CASE(Spawner) { ComponentModelTesting::Spawner(); }
TEST_CASE(StaticTypization) { ComponentModelTesting::StaticTypization(); }
TEST_CASE(ComponentsReordering) { ComponentModelTesting::ComponentsReordering(); }
TEST_CASE(NoNamedComponents) { ComponentModelTesting::NoNamedComponents(); }
TEST_CASE(NestedSpawners) { ComponentModelTesting::NestedSpawners(); }
TEST_CASE(CyclicSpawners) { ComponentModelTesting::CyclicSpawners(); }
TEST_CASE(ConfigurableSpawners) { ComponentModelTesting::ConfigurableSpawners(); }
TEST_CASE(ObjectArrayEmbeddedScopename) { ComponentModelTesting::ObjectArrayEmbeddedScopename(); }

TEST_SUITE_END();




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