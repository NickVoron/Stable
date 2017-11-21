#include "createFromDesc.h"
#include "stuff/library.include.h"

namespace unigui 
{
namespace font
{		
	namespace
	{
		typedef unsigned int DWORD;
		typedef unsigned char BYTE;

		DWORD hexToDword(const std::string & hex)
		{
			std::string preparedString = hex;
			if(preparedString[1] == 'x')
			{
				preparedString = preparedString.substr(2, preparedString.size() - 2 );
			}

			int size = preparedString.size();
			int c = 8 - size;
			if(c > 0)
			{
				for (int i = 0; i < c; ++i)
				{
					preparedString = "0" + preparedString;
				}
				hexToDword(preparedString);
			}

			DWORD v = 0;
			size = static_cast<int>(hex.size());
			DWORD h = 1;
			for(int b = size - 1; b >= 0; --b)
			{
				BYTE x = 0;
				if('0' <= hex[b] && hex[b] <= '9') x = hex[b] - '0';
				else {
					const char l = tolower(hex[b]);
					if('a' <= l && l <= 'f') x = l + 10 - 'a';
				}
				v += x * h;
				h <<= 4;
			}
			return v;
		}
	}

	void createFromXMLDesc(const char *descFileName, FontExport& font)
	{
		/*
		TiXmlDocument doc(descFileName);
		if (!doc.LoadFile()) 
		{
			throw Base::Errors::Simple(__FUNCTION__" can't open font decription:" + std::string(descFileName) );
		}

		TiXmlElement* root = doc.FirstChildElement();

		const char* name = root->Attribute("name");
		const char* systemName = root->Attribute("sysFontName");
		int size;
		root->QueryIntAttribute("size", &size);

		font.init(name, systemName, size);

		// 	TiXmlElement* stylesNode = root->FirstChildElement("styles");
		// 	TiXmlElement* styleNode = stylesNode->FirstChildElement();
		// 	while(styleNode)
		// 	{
		// 		Style style;
		// 
		// 		style.name = styleNode->Value();
		// 		//styleNode->QueryIntAttribute("bold", &style.bold);
		// 		//styleNode->QueryIntAttribute("italic", &style.metric.italic);
		// 
		// 		fontDesc.styles.push_back(style);
		// 
		// 		styleNode = dynamic_cast<TiXmlElement*>( stylesNode->IterateChildren(styleNode) );
		// 	}

		TiXmlElement* rangesNode = root->FirstChildElement("ranges");
		TiXmlElement* rangeNode = rangesNode->FirstChildElement();
		while(rangeNode)
		{
			TiXmlElement* subRangeNode = rangeNode->FirstChildElement();

			while(subRangeNode)
			{
				std::string start = subRangeNode->Attribute("start");
				std::string end = subRangeNode->Attribute("end");

				font.addRange(hexToDword(start), hexToDword(end));

				subRangeNode = rangeNode->IterateChildren(subRangeNode)->ToElement();
			}

			rangeNode = rangesNode->IterateChildren(rangeNode)->ToElement();
		}
		*/
	}
}
}