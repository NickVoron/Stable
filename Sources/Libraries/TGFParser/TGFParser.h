#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace tgf
{
	template<class Content>
	struct Graph 
	{
		struct Node
		{
			std::string id;
			Content content;
		};

		struct Edge
		{
			Node* node0;
			Node* node1;
			std::string content;
		};

		std::vector<std::unique_ptr<Node>> nodes;
		std::vector<std::unique_ptr<Edge>> edges;
	};

	typedef Graph<std::string> TGFGraph;

	TGFGraph load(std::istream& is);
	void save(const TGFGraph& graph, std::istream& os);

	TGFGraph load(const char* fileName);
	void save(const TGFGraph& graph, const char* fileName);
}