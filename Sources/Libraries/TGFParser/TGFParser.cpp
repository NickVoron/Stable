// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "TGFParser.h"

#include <fstream>
#include <map>

namespace tgf
{
	TGFGraph load(std::istream& is)
	{
		TGFGraph result;
		char buffer[4096];
		bool nodes = true;
		std::map<std::string, TGFGraph::Node*> nodesmap;
		while (is.good())
		{
			if (nodes)
			{
				TGFGraph::Node node;
				is >> node.id;
				if (node.id != "#")
				{
					is.getline(&buffer[0], std::size(buffer));
					node.content = buffer;
					result.nodes.push_back(std::make_unique<TGFGraph::Node>(node));
					nodesmap[node.id] = result.nodes.back().get();
				}
				else
				{
					nodes = false;
				}
			}
			else
			{
				std::string id0;
				std::string id1;
				is >> id0 >> id1;

				TGFGraph::Edge edge;
				edge.node0 = nodesmap[id0];
				edge.node1 = nodesmap[id1];
				is.getline(&buffer[0], std::size(buffer));
				edge.content = buffer;
				result.edges.push_back(std::make_unique<TGFGraph::Edge>(edge));
			}
		}

		return std::move(result);


	}

	void save(const TGFGraph& graph, std::ostream& os)
	{
		for (auto& node : graph.nodes)
		{
			os << node->id << " " << node->content << std::endl;
		}

		os << "#" << std::endl;

		for (auto& edge : graph.edges)
		{
			os << edge->node0->id << " " << edge->node1->id << " " << edge->content;
		}
	}

	TGFGraph load(const char* fileName)
	{
		std::ifstream is(fileName);
		return load(is);
	}

	void save(const TGFGraph& graph, const char* fileName)
	{
		std::ofstream os(fileName);
		return save(graph, os);
	}
}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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