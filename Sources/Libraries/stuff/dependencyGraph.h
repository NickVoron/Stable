#pragma once

#include <list>
#include <vector>

#include "common/clearPtrContainer.h"


template<class Content>
class DependencyGraph
{
public:
	struct Node;
	typedef std::list<const Node*> DependencyList;

	~DependencyGraph();

	struct Node
	{
		Node(const Content& cId);

		Node* dependence(DependencyGraph& parent, Content id);

		void mark(bool m) const;
		bool acyclic(DependencyList* dependencies) const;
		void countDependencies();

		void linearize(std::vector<Node*>& result);

		Content componentId;
		std::vector<Node*> nodes;
		int dependenciesCount = 0;
		int index = -1;
		mutable bool marked; 
	};

	Node* node(const Content& componentId);
	Node* dependence(const Content& parent, const Content& child);
	
	bool acyclic(DependencyList* dependencies = 0) const;
	bool linearize(std::vector<Node*>& result);
	void sortByDependenciesCount();
	void countDependencies();
	
	std::vector<Node*> nodes;
};


//
//
//
template<class Content>
void unique(std::vector<typename DependencyGraph<Content>::Node*>& result)
{
	std::vector<typename DependencyGraph<Content>::Node*> uniq;
	uniq.reserve(result.size());
	for(auto* node : result)
	{
		if(node)
		{
			if(std::find(uniq.begin(), uniq.end(), node) == uniq.end())
			{
				uniq.push_back(node);
			}	
		}				
	}

	result.swap(uniq); 
}

//
//
//
template<class Content>
DependencyGraph<Content>::~DependencyGraph()
{
	Base::clearPtrContainer(nodes);
}

template<class Content>
DependencyGraph<Content>::Node::Node(const Content& cId)
:componentId(cId), marked(false)
{

}

template<class Content>
typename DependencyGraph<Content>::Node* DependencyGraph<Content>::Node::dependence(DependencyGraph<Content>& parent, Content id)
{
	nodes.push_back( parent.node(id) );
	return nodes.back();
}

template<class Content>
void DependencyGraph<Content>::Node::mark(bool m) const 
{
	marked = m; 
}

template<class Content>
bool DependencyGraph<Content>::Node::acyclic(DependencyList* dependencies) const
{
	if(dependencies)
	{
		dependencies->push_back(this);
	}

	if(!marked)
	{
		mark(true);
		
		for(Node* node : nodes)
		{
			if(!node->acyclic(dependencies))
				return false;
		}

		if(dependencies)
		{
			dependencies->pop_back();
		}
		mark(false);

		return true;
	}
	
	return false;
}

template<class Content>
void DependencyGraph<Content>::Node::countDependencies()
{
	std::vector<Node*> result;
	linearize(result);
	unique<Content>(result);

	dependenciesCount = result.size();
}

template<class Content>
void DependencyGraph<Content>::Node::linearize(std::vector<Node*>& result)
{
	result.push_back(this);
	for(Node* node : nodes)
	{
		node->linearize(result);
	}
}

template<class Content>
struct compareId
{
	compareId(Content cid):componentId(cid){}
	bool operator()(typename DependencyGraph<Content>::Node* node) const { return node->componentId == componentId; }
	Content componentId;
};


template<class Content>
typename DependencyGraph<Content>::Node* DependencyGraph<Content>::node(const Content& componentId)
{
	auto it = std::find_if(nodes.begin(), nodes.end(), compareId<Content>(componentId) );

	if(it == nodes.end())
	{
		nodes.push_back(new Node(componentId));
		nodes.back()->index = static_cast<int>(nodes.size()) - 1;
		return nodes.back();
	}

	return *it;
}

template<class Content>
typename DependencyGraph<Content>::Node* DependencyGraph<Content>::dependence(const Content& parent, const Content& child)
{
	return node(parent)->dependence(*this, child);
}

template<class Content>
bool DependencyGraph<Content>::acyclic( DependencyList* dependencies ) const
{
	for(Node* node : nodes)
	{
		if(!node->acyclic( dependencies ))
			return false;
	}

	return true;
}


template<class Content>
bool DependencyGraph<Content>::linearize(std::vector<Node*>& result)
{
	if(acyclic())
	{
		for(Node* node : nodes)
		{
			node->linearize(result);
		}
		std::reverse(result.begin(), result.end());
		unique<Content>(result);

		return true;
	}

	return false;
}

template<class Content>
void DependencyGraph<Content>::countDependencies()
{
	for(Node* node : nodes)
	{
		node->countDependencies();
	}
}

template<class Content>
bool compareDependenciesCount(typename DependencyGraph<Content>::Node* n0, typename DependencyGraph<Content>::Node* n1)
{
	return (n0 && n1) ? (n0->dependenciesCount > n1->dependenciesCount) : false;
}

template<class Content>
void DependencyGraph<Content>::sortByDependenciesCount()
{
	nodes.erase( std::remove(nodes.begin(), nodes.end(), (DependencyGraph<Content>::Node*)0), nodes.end() );
	countDependencies();
	std::sort(nodes.begin(), nodes.end(), compareDependenciesCount<Content>);
}

//
//
//
template<class Content>
struct DependencyGraphBuilder
{
	DependencyGraph<Content> graph;

	void dependence(const Content& content)
	{
		if( stack.empty() )
		{
			stack.push_back( graph.node(content) );
		}
		else
		{
			stack.push_back( graph.dependence(stack.back()->componentId, content) );			
		}
	}

	void sibling(const Content& content)
	{
        if( stack.empty() )
        {
            stack.push_back( graph.node(content) );
        }
        else
        {
            stack.pop_back();
            dependence(content);
        }
	}

	void pop()
	{
		if( !stack.empty() )
		{
			stack.pop_back();
		}		
	}

	typename DependencyGraph<Content>::DependencyList stack;
};