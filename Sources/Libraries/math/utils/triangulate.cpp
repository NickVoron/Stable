// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "triangulate.h"
#include <float.h>

namespace 
{

class CPolyTri
{
public:
	CPolyTri()              {  m_nIndex=NULL; m_nMaxPoints=0;      }
	virtual  ~CPolyTri()    {  if( m_nIndex ) delete [] m_nIndex;  }
	
	
	
	int   Triangulate(const Vector3* points, const Vector3 &normal, int nCount)
	{
		
		
		
		int   nTriangle= 0;
		int   nVertex  = nCount;
		
		AllocIndex(nCount);
		
		bool     bNoErrors   = true;
		
		while( nVertex > 3 && bNoErrors ){
			
			
			
			bNoErrors   = false;
			
			for( int i=0 , j=1 , k=2 ; k < nVertex ; ){
				
				switch( TriangleArea(points      ,
					m_nIndex[i] ,
					m_nIndex[j] ,
					m_nIndex[k] ,
					normal      ) ){
						
						
						
case convex       :
	
	
	
	if( IsAnyPointInside(points,i,j,k,nVertex) ){
		
		
		
		i  = j;
		j  = k;
		k++;
	}else{
		nTriangle++;
		AddFace(points,m_nIndex[i],m_nIndex[j],m_nIndex[k]);
		
		
		
		nVertex  = RemoveVertex( j,nVertex );
		bNoErrors= true;
	}
	break;
case concave      :
	
	
	
	i  = j;
	j  = k;
	k++;
	break;
case degenerate   :
	
	
	
	nVertex  = RemoveVertex( j,nVertex );
	bNoErrors= true;
	break;
				}
			}
		}
		return nTriangle;
	}
	
	
	
	static   void  ComputeNormal(const Vector3* points,int nPoints,Vector3 &normal)
	{
		
		
		
		normal[0]=normal[1]=normal[2]=0.0f;
		for( int i=0 , j=1 ; j < nPoints ; i++ , j++ ){
			normal[0]+= ( points[i][1] - points[j][1] ) * ( points[i][2] + points[j][2] ) ;
			normal[1]+= ( points[i][2] - points[j][2] ) * ( points[i][0] + points[j][0] ) ;
			normal[2]+= ( points[i][0] - points[j][0] ) * ( points[i][1] + points[j][1] ) ;
		}
	}
	
	
	
	static   bool  IsConvex(const Vector3* points,int nPoints,const Vector3 &normal)
	{
		
		
		
		
		Vector3 vi( points[1] );   vi-=points[0];
		Vector3 vj,n;
		int      nP=nPoints-1;
		int i, j, k;
		
		for( i=0 , j=1 , k ; j < nPoints ; i++ , j++ ){
			k  = (j+1) % nP;
			vj = points[k];
			vj-= points[j];
			
			vi.SCross(vj, n);
			
			if( (n[0]*normal[0]) < 0.0f ||
				(n[1]*normal[1]) < 0.0f ||
				(n[2]*normal[2]) < 0.0f ) break;
			vi = vj;
		}
		return ( j == nPoints ? true : false );
	}
protected:
	
	int         *m_nIndex;
	Vector3    m_e0     ;
	Vector3    m_e1     ;
	Vector3    m_N      ;
	float       m_A      ;  
	
	enum  {  convex      =  1,
		degenerate  =  0,
		concave     = -1};
	
	int   RemoveVertex( int j,int nVertex )
	{
		while( ++j < nVertex )
			m_nIndex[j-1]=m_nIndex[j];
		return (nVertex-1);
	}
	
	bool  IsAnyPointInside(const Vector3* points,int i,int j,int k,int nVertex) const
	{
		int   ik=m_nIndex[k];
		for( int ip=0 ; ip < nVertex ; ip++ )
			if( ( ip < i || ip > k ) &&
				IsPointInside(points[m_nIndex[ip]],points[ik]) ){
					return true;
			}
			return false;
	}
	
	bool  IsPointInside(const Vector3  point,const Vector3  q2)  const
	{
		Vector3 pmq2  = point;
		pmq2 -= q2;
		
		Vector3 ntmp;
		
		float    B0,B1;
		
		pmq2.SCross(m_e1,ntmp);  if( (B0=m_N.SDot(ntmp)) <= 0.0 ) return false;
		m_e0.SCross(pmq2,ntmp);  if( (B1=m_N.SDot(ntmp)) <= 0.0 ) return false;
		return ( (m_A-B0-B1) > 0.0 ? true : false );
	}
	
	int   TriangleArea(const Vector3* points,int i,int j,int k,const Vector3& normal)
	{
		m_e0=points[i];  m_e0-=points[k];
		m_e1=points[j];  m_e1-=points[k];
		
		m_e0.SCross(m_e1,m_N);
		
		m_A=m_N.MagnitudeSquared();
		
		
		
		if( (-FLT_EPSILON) < m_A && m_A < FLT_EPSILON )
			return degenerate;
		
		
		
		return ( m_N.SDot( normal ) < 0.0 ? concave : convex );
	}
	
	virtual  void  AddFace(const Vector3* points,int i,int j,int k)
	{}
private:
	int	m_nMaxPoints;
	void  AllocIndex(int nCount)
	{
		if( nCount > m_nMaxPoints ){
			if( m_nIndex ) delete [] m_nIndex;
			m_nMaxPoints   = nCount+2;
			m_nIndex       = new int[m_nMaxPoints];
		}
		for( int i=0 ; i < nCount ; i++ )
			m_nIndex[i] = i;
	}
};


class CTri : public CPolyTri
{
public:
	std::vector<Vector3>& o;
	CTri(std::vector<Vector3>& _o) : o(_o){}
protected:
	
	virtual void  AddFace(const Vector3* points,int i,int j,int k)
	{
		o[i]  = points[i];
		o[j]  = points[j];
		o[k]  = points[k];
	}
};

}

namespace Base{
namespace MathUtils	{

	void triangulate(const std::vector<Vector3>& points, std::vector<Vector3>& output)
	{
		CTri triangulator(output);
		Vector3 norm  = (points[1] - points[0]).SCross( points[2] - points[0] );
		triangulator.Triangulate(&points[0], norm, (int)points.size() );
	}

}}





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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