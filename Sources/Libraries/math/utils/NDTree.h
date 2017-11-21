#pragma once

#include "NDCube.h"

namespace Base{

template< class T, int D > class NDTree
{
public:
	static const size_t MAX_ELEMENTS_IN_NODE = 1;
	typedef std::vector<T> PointsList;
	typedef std::vector<size_t> IndicesList;

	PointsList points;

	struct Node
	{
		IndicesList idx;
		Node* children[1<<D];
		Node()	{	mem::memzero(children);	}

		void search(IndicesList& out, const NDCube<D>& cb, const T& point, float radius, const std::vector<T>& points) const
		{
			//проверим точки этого нода в радиус попадания
			for(size_t i = 0; i < idx.size(); ++i)
				if( ( p - points[idx[i]] ).Magnitude() <= radius )
					out.push_back(idx[i]);

			//если дочерний нод попадает в радиус - поищем в нём
			for(int d=0; d < (1<<D); d++)
			{
				if(children[d]==0)	continue;
				//бокс дочернего нода
				NDVector<D> c = cb.corner;
				for(int dd = 0; dd < D; dd++)	if( d & (1<<dd) ) c[dd] += cb.size * 0.5f;
				NDCube<D> cb( c, cb.size * 0.5f );

				//попадает-ли сфера в нод?
				if(cb.isIntersect(p,r))
					children[d]->search(out, cb, point, radius, points);
			}
		}
	};

	Node* root;
	NDCube<D> cube;
	
	NDTree(){}
	NDTree( const PointsList& objects )	{ build(objects); }

	void build(const PointsList& objects)
	{
		points.insert(points.begin(), objects.begin(), objects.end());

		//найдём баундинг-бокс
		NDVector<D> bmin(+FLT_MAX), bmax(-FLT_MAX);
		for(size_t i=0; i < objects.size(); ++i)
			for(int d=0; d<D; ++d)
			{
				bmin[d] = std::min( bmin[d], objects[i][d] );
				bmax[d] = std::max( bmax[d], objects[i][d] );
			}
		//сделаем бокс чуть больше, чтобы точки на границе не выпали из него в результате арифметической погрешности
		bmin -= 1e-6f;
		bmax += 1e-6f;

		//минимальный ограничивающий объём с равными сторонами
		NDVector<D> corner = bmin;
		NDVector<D> sz = bmax-bmin;
		float size = sz[sz.maxVal()];
		cube.corner = corner;
		cube.size = size;

		IndicesList remainder;
		//for(size_t i = 0; i < objects.size(); ++i) remainder.push_back(i);

		root = process(objects, remainder, cube);
	}

	struct Pred
	{
		Pred(const NDCube<D>& _cb, const PointsList& _o) : cb(_cb), o(_o){}
		bool operator()(size_t s)
		{
			return cb.isInside( o[s] );	
		}

		const NDCube<D>& cb;
		const PointsList& o;
	};

	Node* process( const PointsList& o, IndicesList& rem, const NDCube<D>& cube)
	{
		//нет элементов
		if(rem.size()==0)	return 0;

		Node* node = new Node();
		//все элементы влезают в текущий нод - нет потомков
		if(rem.size() <= MAX_ELEMENTS_IN_NODE )	{	node->idx.insert( node->idx.end(), rem.begin(), rem.end() );	}
		else
			//делим каждое подпространство пополам на левое и правое и рекурсивно вынимаем из оставшихся элементы
			for(int d=0; d< (1<<D); d++)
			{
				NDVector<D> c = cube.corner;
				for(int dd=0; dd < D; dd++)	if( d & (1<<dd) ) c[dd] += cube.size*0.5f;
				NDCube<D> cb(c, cube.size*0.5f);

				//формируем локально список точек, попадающих в этот нод
				IndicesList local;
				Pred p(cb,o);
				foreach(size_t i, rem)	{	if(cb.isInside(o[i]))	local.push_back(i);	}
				rem.erase(std::remove_if(rem.begin(), rem.end(), p ), rem.end());

				node->children[d] = process(o, local, cb);
			}

		return node;
	}

	void search(IndicesList& i, const T& point, float radius) const
	{
		root->search(i, cube, p, r, points);
	}
};

};
