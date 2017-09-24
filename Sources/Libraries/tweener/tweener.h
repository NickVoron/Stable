// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/intrusiveList.h"

#include "equationType.h"
#include "equationSelector.h"
#include "equationsTable.h"
#include "interpolator.h"

namespace tween
{
	template<class ValueType, int count> class Tweener;
	
	template<class ValueType>
	class TweenerProperty 
	{
	public:
		TweenerProperty();
		void set(ValueType& valueRef);
		void reset();
		void reset(bool reverse);
		void finalize();
		void final(const ValueType& fv);
		void finalInc(const ValueType& fd);
		bool completed() const;
		const ValueType& value() const { return *ptrValue; }
		void bounds(const ValueType& src, const ValueType& dst);
		
		TWEENER_INLINE void evaluate(float t)	{ *ptrValue = Interpolator<ValueType>::evaluate(initialValue, finalValue, t); }

	private:
		ValueType *ptrValue;
		ValueType finalValue;
		ValueType initialValue;
	};

	template<class ValueType, int count>
	class TweenerParamBase : public Base::IntrusiveList<TweenerParamBase<ValueType, count> >::Node
	{
	public:
		TweenerProperty<ValueType> properties[count];
		float time;
		float delay;
		float timeCount;
		int repeat;
		int reverseCount;
		bool delayFinished;
		bool reverse;

		TweenerParamBase();

		const ValueType& value(int index) const { return properties[index].value(); }
		void setRepeatWithReverse(int _n, bool _reverse);

		TweenerProperty<ValueType>& bind(int idx, ValueType& val);
		TweenerProperty<ValueType>& bind(int idx, ValueType& vall, const ValueType& targetVal);
		TweenerProperty<ValueType>& target(int idx, const ValueType& val);
		TweenerProperty<ValueType>& bounds(int idx, const ValueType& src, const ValueType& dst);
		void start(Tweener<ValueType, count>& tweener);

		bool completed(int idx) const;
		void finish();
		
		TWEENER_INLINE TweenerParamBase* evaluate(float dt)
		{
			timeCount += dt;

			if ( delayFinished ) 
			{
				return evaluate();
			} 
			else if(timeCount > delay) 
			{
				delayFinished = true;
				timeCount = 0;
			}

			return (TweenerParamBase*)this->next;
		}

	protected:
		TWEENER_INLINE TweenerParamBase* evaluate()
		{
			if( time > timeCount ) 
			{
				evaluateImpl();
				return (TweenerParamBase*) this->next;
			}
			else
			{
				return complete();
			}
		} 

		virtual void evaluateImpl() = 0;
		void defaults();
		void init(float ptime, float pdelay);
		TweenerParamBase* complete();
		void finalize();
		int decreaseRepeat();
	};

	template<class ValueType, int count>
	class TweenerParam : public TweenerParamBase<ValueType, count>
	{
	public:
		TweenerParam(){};
		TweenerParam(float ptime, EasingEquation pequation = EXPO_EASE_OUT, float pdelay = 0);
		void init(float ptime, EasingEquation pequation = EXPO_EASE_OUT, float pdelay = 0);
		
		EasingEquation equation;
	protected:
		virtual void evaluateImpl();
	};

	template<EasingEquation equation, class ValueType, int count>
	class TweenerParamT : public TweenerParamBase<ValueType, count>
	{
	public:
		TweenerParamT(float ptime, float pdelay = 0)
		{
			defaults();
			init(ptime, pdelay);
		}

	protected:
		virtual inline void evaluateImpl()
		{
			float t = SelectEquation<equation>::ease(timeCount, time);
			for(auto& p : properties) 
			{
				p.evaluate(t);
			}
		}
	};
	

	template<class ValueType, int count>
	class Tweener : public Base::IntrusiveList< TweenerParamBase<ValueType, count> >
	{
	public:
		typedef TweenerParam<ValueType, count> Param;

		static Param param(float time, EasingEquation pequation = EXPO_EASE_OUT, float pdelay = 0) { return Param(time, pequation, pdelay); }

		TWEENER_INLINE void process(float time)
		{
			if(!this->empty())
			{
				for(auto* p = this->first(); p != this->end(); )
				{
					p = p->evaluate(time);
				}
			}			
		}
    };
	
	
	
	
	template<class ValueType>
	TweenerProperty<ValueType>::TweenerProperty() : ptrValue(0)
	{
		makeDefault(initialValue);
		makeDefault(finalValue);
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::set(ValueType& valueRef)
	{
		ptrValue = &valueRef;
		initialValue = valueRef;
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::reset()
	{
		*ptrValue = initialValue;
	}


	template<class ValueType>
	void TweenerProperty<ValueType>::reset(bool reverse)
	{
		if(reverse)
		{
			std::swap(finalValue, initialValue);
		}

		reset();
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::final(const ValueType& fv)
	{
		finalValue = fv;
		initialValue = *ptrValue;
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::finalInc(const ValueType& fd)
	{
		finalValue += fd;
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::finalize()
	{
		*ptrValue = finalValue;
	}

	template<class ValueType>
	bool TweenerProperty<ValueType>::completed() const
	{
		return *ptrValue == finalValue;
	}

	template<class ValueType>
	void TweenerProperty<ValueType>::bounds(const ValueType& src, const ValueType& dst)
	{
		finalValue = dst;
		initialValue = src;
	}

	
	
	
	template<class ValueType, int count>
	void TweenerParam<ValueType, count>::evaluateImpl()
	{
		EasingFunc f = funcs[this->equation];
		float t = f(this->timeCount, this->time);
		for(auto& p : this->properties)
		{
			p.evaluate(t);
		}
	}

	template<class ValueType, int count>
	TweenerParam<ValueType, count>::TweenerParam(float ptime, EasingEquation pequation, float pdelay) 
	{
		init(ptime, pequation, pdelay);
	}

	template<class ValueType, int count>
	void TweenerParam<ValueType, count>::init(float ptime, EasingEquation pequation, float pdelay)
	{
		equation = pequation;

		this->defaults();
		TweenerParamBase<ValueType, count>::init(ptime, pdelay);
	}

	
	
	
	template<class ValueType, int count>
	TweenerParamBase<ValueType, count>::TweenerParamBase() 
	{
		defaults();
	}

	template<class ValueType, int count>
	TweenerParamBase<ValueType, count>* TweenerParamBase<ValueType, count>::complete()
	{
		return (decreaseRepeat() < 0) ? (TweenerParamBase*)this->remove() : (TweenerParamBase*)this->next;
	}
	
	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::finish()
	{
		this->remove();
		defaults();
	}

	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::defaults()
	{
		time = 0.0f;
		timeCount = 0;
		delayFinished = true;
		delay = 0;
		repeat = -1;
		reverse = false;
		reverseCount = -1;
	}

	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::init(float ptime, float pdelay)
	{
		time = ptime;
		delayFinished = !(pdelay > 0);
		delay = pdelay;
	}

	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::start(Tweener<ValueType, count>& tweener) 
	{
		timeCount = 0;
		delayFinished = delay <= 0.0f;	
		tweener.push_back(*this);
	}

	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::setRepeatWithReverse(int _n,bool _reverse) 
	{
		repeat = _n-1;
		if (_reverse) 
		{
			reverse = _reverse;
			reverseCount = repeat + 1;
		}
	}	

	template<class ValueType, int count>
	int TweenerParamBase<ValueType, count>::decreaseRepeat() 
	{
		if (repeat > -1 || reverseCount > -1) 
		{	
			timeCount = 0; 

			if (repeat == 0)
			{
				finalize();
			}			

			for(auto& p : properties) 
			{
				p.reset(reverse);
			}

			if (reverseCount <= repeat) 
			{
				repeat--;		
			} 
			else 
			{
				reverseCount--;
			}	
		}	

		return repeat;
	}	

	template<class ValueType, int count>
	TweenerProperty<ValueType>& TweenerParamBase<ValueType, count>::bind(int idx, ValueType& val)
	{
		auto& prop = properties[idx];
		prop.set(val);
		prop.final(val);
		return prop;
	}

	template<class ValueType, int count>
	TweenerProperty<ValueType>& TweenerParamBase<ValueType, count>::bind(int idx, ValueType& val, const ValueType& targetVal)
	{
		auto& prop = properties[idx];
		prop.set(val);
		prop.final(targetVal);
		return prop;
	}

	template<class ValueType, int count>
	TweenerProperty<ValueType>& TweenerParamBase<ValueType, count>::bounds(int idx, const ValueType& src, const ValueType& dst)
	{
		properties[idx].bounds(src, dst);
		return properties[idx];
	}

	template<class ValueType, int count>
	TweenerProperty<ValueType>& TweenerParamBase<ValueType, count>::target(int idx, const ValueType& val)
	{
		properties[idx].final(val);
		return properties[idx];
	}

	template<class ValueType, int count>
	bool TweenerParamBase<ValueType, count>::completed(int idx) const
	{
		return properties[idx].completed();
	}

	template<class ValueType, int count>
	void TweenerParamBase<ValueType, count>::finalize()
	{
		for(auto& p : properties) 
		{
			p.finalize();
		}
	}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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