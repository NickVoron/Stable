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