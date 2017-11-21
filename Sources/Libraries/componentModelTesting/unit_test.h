#pragma once

#include "library.include.h"
#include "unitTest/library.include.h"

TEST_SUITE(ComponentModelUnitTest)

TEST_CASE(RootDirSetter) { Base::FileUtils::setModuleDirAsRoot(); }


//TEST_CASE(ComponentAndProperties) { ComponentModelTesting::ComponentAndProperties(); }
TEST_CASE(Lambda) { ComponentModelTesting::Lambda(); }
TEST_CASE(DataArray) { ComponentModelTesting::DataArray(); }
TEST_CASE(ExpressionsConversion) { ComponentModelTesting::ExpressionsConversion(); }
TEST_CASE(ComponentAsParameter) { ComponentModelTesting::ComponentAsParameter(); }
TEST_CASE(FunctionCall) { ComponentModelTesting::FunctionCall(); }
TEST_CASE(DataStruct) { ComponentModelTesting::DataStruct(); }
TEST_CASE(ComplexData) { ComponentModelTesting::ComplexData(); }
TEST_CASE(ExternalLanguage) { ComponentModelTesting::ExternalLanguage(); }
TEST_CASE(Spawner) { ComponentModelTesting::Spawner(); }
TEST_CASE(NoNamedComponents) { ComponentModelTesting::NoNamedComponents(); }
TEST_CASE(NestedSpawners) { ComponentModelTesting::NestedSpawners(); }
TEST_CASE(CyclicSpawners) { ComponentModelTesting::CyclicSpawners(); }
TEST_CASE(ConfigurableSpawners) { ComponentModelTesting::ConfigurableSpawners(); }
TEST_CASE(StaticTypization) { ComponentModelTesting::StaticTypization(); }
TEST_CASE(ComponentsReordering) { ComponentModelTesting::ComponentsReordering(); }
TEST_CASE(ComponentLink) { ComponentModelTesting::ComponentLink(); }
TEST_CASE(NestedObject) { ComponentModelTesting::NestedObject(); }
TEST_CASE(CirclePoints) { ComponentModelTesting::CirclePoints(); }
TEST_CASE(ObjectArrayEmbeddedScopename) { ComponentModelTesting::ObjectArrayEmbeddedScopename(); }
TEST_CASE(CrossReference) { ComponentModelTesting::CrossReference(); }
TEST_CASE(References) { ComponentModelTesting::References(); }
TEST_CASE(ObjectArray) { ComponentModelTesting::ObjectArray(); }
TEST_CASE(NestedObjectsInitialized) { ComponentModelTesting::NestedObjectsInitialized(); }
TEST_CASE(SpawnerPrototypeInternalLinks) { ComponentModelTesting::SpawnerPrototypeInternalLinks(); }
TEST_CASE(Bugs) { ComponentModelTesting::Bugs(); }
TEST_CASE(Bug0005) { ComponentModelTesting::Bug0005(); }
TEST_CASE(bug0008) { ComponentModelTesting::bug0008(); }
TEST_CASE(bug0011) { ComponentModelTesting::bug0011(); }


TEST_SUITE_END();
