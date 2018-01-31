// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "odsfile.h"

#include "rapidxml/library.include.h"
#include "zipfile/library.include.h"
#include "defaultLogs/library.include.h"

namespace odsfile
{
	bool table(const std::string &fileName, std::vector<std::vector<std::string> >& data)
	{
		std::vector<char> content;

		try 
		{
			zip::File file(fileName);
			file.readFile("content.xml", content);
		}
		catch (const std::runtime_error& e) 
		{
			LOG_ERROR(e);
			return false;
		}

		content.emplace_back(0);

		rapidxml::xml_document<> doc;
		doc.parse<0>(&content[0]);
		if (strcmp("office:document-content", doc.first_node()->name()) != 0) 
		{ 
			return false; 
		}

		if (auto worksheet = doc.first_node("office:document-content")->first_node("office:body")->first_node("office:spreadsheet"))
		{
			while (auto table = worksheet->first_node("table:table"))
			{
				auto row = table->first_node("table:table-row");
				if (!row)
				{
					table = table->next_sibling("table:table");
					continue;
				}

				while (row)
				{
					std::vector<std::string> rowdata;
					while (auto cell = row->first_node("table:table-cell"))
					{
						auto data = cell->first_node("text:p");
						if (data && data->first_node())
						{
							rowdata.push_back(data->first_node()->value());
						}

						cell = cell->next_sibling("table:table-cell");
					}
					row = row->next_sibling("table:table-row");

					data.emplace_back(rowdata);
				}
				table = table->next_sibling("table:table");
			}
			return true;
		}

		return false;
	}

}




// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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