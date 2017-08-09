// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
			
			for(size_t i = 0; i < idx.size(); ++i)
				if( ( p - points[idx[i]] ).Magnitude() <= radius )
					out.push_back(idx[i]);

			
			for(int d=0; d < (1<<D); d++)
			{
				if(children[d]==0)	continue;
				
				NDVector<D> c = cb.corner;
				for(int dd = 0; dd < D; dd++)	if( d & (1<<dd) ) c[dd] += cb.size * 0.5f;
				NDCube<D> cb( c, cb.size * 0.5f );

				
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

		
		NDVector<D> bmin(+FLT_MAX), bmax(-FLT_MAX);
		for(size_t i=0; i < objects.size(); ++i)
			for(int d=0; d<D; ++d)
			{
				bmin[d] = std::min( bmin[d], objects[i][d] );
				bmax[d] = std::max( bmax[d], objects[i][d] );
			}
		
		bmin -= 1e-6f;
		bmax += 1e-6f;

		
		NDVector<D> corner = bmin;
		NDVector<D> sz = bmax-bmin;
		float size = sz[sz.maxVal()];
		cube.corner = corner;
		cube.size = size;

		IndicesList remainder;
		

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
		
		if(rem.size()==0)	return 0;

		Node* node = new Node();
		
		if(rem.size() <= MAX_ELEMENTS_IN_NODE )	{	node->idx.insert( node->idx.end(), rem.begin(), rem.end() );	}
		else
			
			for(int d=0; d< (1<<D); d++)
			{
				NDVector<D> c = cube.corner;
				for(int dd=0; dd < D; dd++)	if( d & (1<<dd) ) c[dd] += cube.size*0.5f;
				NDCube<D> cb(c, cube.size*0.5f);

				
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




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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