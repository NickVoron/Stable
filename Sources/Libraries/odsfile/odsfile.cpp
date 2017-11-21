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
