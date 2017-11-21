#pragma once

#include <type_traits>
#include <utility>

namespace Base
{

template<class... Nodes>
struct IntrusiveIntersection : public Nodes...
{
	void remove()
	{
		auto sz = { (this->Nodes::remove(), 0)... };
	}
};
	  
template<class Content, class tag = int>
class IntrusiveList
{
template<class NodeType, bool bindedNode> friend struct PushBack;
template<class NodeType, bool bindedNode> friend struct PushFront;

public:

	IntrusiveList();
	IntrusiveList(const IntrusiveList&) = delete;
	IntrusiveList& operator=(const IntrusiveList&) = delete;

	IntrusiveList(IntrusiveList&& other);
	IntrusiveList& operator=(IntrusiveList&& other);

	struct Node;
	class iterator
	{
	public:
		iterator() :node(nullptr) {}
		iterator(Node* n) :node(n) {}

		iterator& operator++()
		{
			node = node->next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator t = *this;
			++t;
			return t;
		}

		iterator& operator--()
		{
			node = node->prev;
			return *this;
		}

		iterator operator--(int)
		{
			iterator t = *this;
			--t;
			return t;
		}

		operator Content*() { return (Content*) node; }

		Content& operator*() const { return (Content&) *node; }
		Content* operator->() const { return (Content*) node; }

		bool operator==(const iterator& it) const { return node == it.node; }
		bool operator!=(const iterator& it) const { return !operator==(it); }

		Node* node;
	};

	class const_iterator
	{
	public:
		const_iterator() :node(nullptr) {}
		const_iterator(const Node* n) :node(n) {}

		const_iterator& operator++()
		{
			node = node->next;
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator t = *this;
			++t;
			return t;
		}

		const_iterator& operator--()
		{
			node = node->prev;
			return *this;
		}

		const_iterator operator--(int)
		{
			const_iterator t = *this;
			--t;
			return t;
		}

		operator const Content*() { return (const Content*) node; }

		const Content& operator*() const { return (const Content&) *node; }
		const Content* operator->() const { return (const Content*) node; }

		bool operator==(const const_iterator& it) const { return node == it.node; }
		bool operator!=(const const_iterator& it) const { return !operator==(it); }

		const Node* node;
	};

	struct Node
	{
		typedef IntrusiveList<Content, tag> ParentListType;

		~Node() { remove(); }

		iterator remove()
		{
			if (next && prev)
			{
				if (next->prev == this) next->prev = prev;
				if (prev->next == this) prev->next = next;
				Node* n = next;
 				prev = next = nullptr;
 				return iterator(n);
			}

			return iterator(nullptr);
		}

		bool isInList() const { return prev && next; }
		void iterate(Content*& n) { n = static_cast<Content*>(next); }
		void iterate(const Content*& n) const { n = static_cast<Content*>(next); }

		Node* prev = nullptr;
		Node* next = nullptr;
	};

	struct BindedNode : public Node
	{
		typename Node::ParentListType* parent_list = nullptr;

		void include() { parent_list->push_front((Content&)*this); }
		void exclude() { Node::remove(); }
	};

	inline void splice(IntrusiveList& splicedList);
	inline Content& push_back(Content& content);
	inline Content& push_front(Content& content);
	inline Content* pop_back();
	inline Content* pop_front();
	inline bool empty() const { return begin() == end(); }
	inline std::size_t size() const;

	inline void clear();
	inline void remove_all();
	inline void destroy_all();

	iterator begin() { return iterator(start_.next); }
	iterator end() { return iterator(&end_); }

	const_iterator begin() const { return const_iterator(start_.next); }
	const_iterator end() const { return const_iterator(&end_); }

	inline const Content* first() const { return (const Content*) ((start_.next != &end_) ? start_.next : nullptr); }
	inline const Content* last() const { return (const Content*)((end_.prev != &start_) ? end_.prev : nullptr); }
	inline Content* first() { return const_cast<Content*>(const_cast<const IntrusiveList*>(this)->first()); }
	inline Content* last() { return const_cast<Content*>(const_cast<const IntrusiveList*>(this)->last()); }

private:
	Node start_;
	Node end_;
};

//
//
//
template<class Content, class tag>
IntrusiveList<Content, tag>::IntrusiveList()
{
	clear();
}

template<class Content, class tag>
IntrusiveList<Content, tag>::IntrusiveList(IntrusiveList&& other)
{
	operator=(std::move(other));
}

template<class Content, class tag>
IntrusiveList<Content, tag>& IntrusiveList<Content, tag>::operator=(IntrusiveList&& other)
{
	if (this != &other)
	{
		if (!other.empty())
		{
			start_.next = other.start_.next;
			end_.prev = other.end_.prev;
			start_.next->prev = &start_;
			end_.prev->next = &end_;
			other.start_.next = &other.end_;
			other.end_.prev = &other.start_;
		}
		else
		{
			clear();
		}
	}

	return *this;
}


template<class Content, class tag>
void IntrusiveList<Content, tag>::splice(IntrusiveList& splicedList)
{
	auto lastNode = last();
	lastNode->next = splicedList.first();
	splicedList.first()->prev = lastNode;
	end_.prev = splicedList.last();
	splicedList.last()->next = end();

	splicedList.clear();
}

template <class T, T val> struct member_wrapper {};
template <class T> std::true_type test_for_parentList(member_wrapper<decltype(&T::parent_list), &T::parent_list>*);
template <class T> std::false_type test_for_parentList(...);

template<class NodeType, bool bindedNode = std::is_same<decltype(test_for_parentList<NodeType>(nullptr)), std::true_type>::value>
struct PushBack;

template<class NodeType> 
struct PushBack<NodeType, false>
{
	template<class TAG>
	static NodeType& invoke(IntrusiveList<NodeType, TAG>& list, NodeType& node)
	{
		((decltype(list.end_)&) node).remove();
		((decltype(list.end_)&) node).prev = list.end_.prev;
		((decltype(list.end_)&) node).next = &list.end_;
		list.end_.prev->next = &node;
		list.end_.prev = &node;

		return node;
	}
};

template<class NodeType>
struct PushBack<NodeType, true>
{
	template<class TAG>
	static NodeType& invoke(IntrusiveList<NodeType, TAG>& list, NodeType& node)
	{
		((typename IntrusiveList<NodeType, TAG>::BindedNode&)node).parent_list = &list;
		return PushBack<NodeType, false>::invoke(list, node);
	}
};

template<class NodeType, bool bindedNode = std::is_same<decltype(test_for_parentList<NodeType>(nullptr)), std::true_type>::value>
struct PushFront;

template<class NodeType>
struct PushFront<NodeType, false>
{
	template<class TAG>
	static NodeType& invoke(IntrusiveList<NodeType, TAG>& list, NodeType& node)
	{
		((decltype(list.end_)&) node).remove();
		((decltype(list.end_)&) node).prev = &list.start_;
		((decltype(list.end_)&) node).next = list.start_.next;
		list.start_.next->prev = &node;
		list.start_.next = &node;

		return node;
	}
};

template<class NodeType>
struct PushFront<NodeType, true>
{
	template<class TAG>
	static NodeType& invoke(IntrusiveList<NodeType, TAG>& list, NodeType& node)
	{
		((typename IntrusiveList<NodeType, TAG>::BindedNode&)node).parent_list = &list;
		return PushBack<NodeType, false>::invoke(list, node);
	}
};


template<class Content, class tag>
Content& IntrusiveList<Content, tag>::push_back(Content& content)
{
	return PushBack<Content>::invoke(*this, content);
}

template<class Content, class tag>
Content& IntrusiveList<Content, tag>::push_front(Content& content)
{
	return PushFront<Content>::invoke(*this, content);
}


template<class Content, class tag>
Content* IntrusiveList<Content, tag>::pop_back()
{
	if (auto lastn = last())
	{
		lastn->prev->next = end();
		end_.prev = lastn->prev;
		return lastn;
	}
	
	return nullptr;
}

template<class Content, class tag>
Content* IntrusiveList<Content, tag>::pop_front()
{
	if (auto firstn = first())
	{
		firstn->next->prev = &start_;
		start_.next = firstn->next;
		return firstn;
	}	
}

template<class Content, class tag>
std::size_t IntrusiveList<Content, tag>::size() const
{
	std::size_t s = 0;
	const_iterator f = begin();
	const_iterator e = end();
	for (; f != e; ++f)
		++s;

	return s;
}

template<class Content, class tag>
void IntrusiveList<Content, tag>::clear()
{
	if (!empty())
	{
		if (start_.next) start_.next->prev = nullptr;
		if (end_.prev) end_.prev->next = nullptr;

		start_.next = &end_;
		end_.prev = &start_;

		start_.prev = nullptr;
		end_.next = nullptr;
	}	
}

template<class Content, class tag>
void IntrusiveList<Content, tag>::remove_all()
{
	auto f = begin();
	auto e = end();
	for (; f != e; f = f->remove()) {}
}

template<class Content, class tag>
void IntrusiveList<Content, tag>::destroy_all()
{
	auto f = begin();
	auto e = end();
	while (f != e)
	{
		auto node = f;
		f = f->remove();
		delete node;
	}
}

}