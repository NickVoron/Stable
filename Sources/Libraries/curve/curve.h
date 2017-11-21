#pragma once

#include <vector>
#include <algorithm>

#include "math/library.include.h"

namespace nm
{
	namespace Private
	{
		template<class ValueType, bool ReturnReference = (sizeof(ValueType) > sizeof(ValueType&) ) > struct ReturnTypeDeterminant;
		template<class ValueType> struct ReturnTypeDeterminant<ValueType, true> {	typedef const ValueType& ReturnType;};
		template<class ValueType> struct ReturnTypeDeterminant<ValueType, false>{	typedef ValueType ReturnType;		};

		//
		template<class ValueType>
		struct Key
		{
			float time;
			ValueType value;

			struct Comparer
			{
				inline bool operator() (const Key& lhs, const Key& rhs) const { return lhs.time < rhs.time; }
				inline bool operator() (const Key& lhs, float time) const { return lhs.time < time; }
				inline bool operator() (float time, const Key& rhs) const { return time < rhs.time; }
			};
		};

		//
		template<class ValueType>
		struct KeysContainer
		{
			typedef Key<ValueType> KeyType;
			typedef std::vector< KeyType > StoreType;
			typedef typename ReturnTypeDeterminant<ValueType>::ReturnType ReturnType;

			inline void addKey(float time, const ReturnType value)
			{
				KeyType k = {time, value};
				keys.push_back(k);
			}

			inline void sort()
			{
				std::sort(keys.begin(), keys.end(), KeyType::Comparer() );
				range.minValue = keys.begin()->time;
				range.maxValue = keys.rbegin()->time;
			}

			Base::Interval<float> range;
			StoreType keys;
		};

		//
		template<class ValueType> 
		class KeySampler 
		{
		public:
			typedef typename KeysContainer<ValueType> Container;
			typedef typename Container::KeyType KeyType;
			typedef typename Container::StoreType StoreType;
			typedef typename ReturnTypeDeterminant<ValueType>::ReturnType ReturnType;

			struct Cache
			{
				typename StoreType::iterator Min;
				typename StoreType::iterator Max;

				Cache() { invalidate(); }
				void invalidate()
				{
					static StoreType fakeStore;
					if(fakeStore.empty())
					{
						fakeStore.reserve(4);
						{KeyType k = {1000000.0f}; fakeStore.push_back(k);}
						{KeyType k = {1000000.0f}; fakeStore.push_back(k);}
						{KeyType k = {-1000000.0f}; fakeStore.push_back(k);}
						{KeyType k = {-1000000.0f}; fakeStore.push_back(k);}
					}

					Min = fakeStore.begin() + 1;
					Max = Min + 1;
				}
			};

			KeySampler() : emptyValue() {}

			inline ReturnType getValue(KeysContainer<ValueType>& keys, float time) 
			{ 
				return near_getValue(keys, time);
			}

			void invalidateCache()
			{
				cache.invalidate();
			}

		private:

			inline ReturnType random_getValue(KeysContainer<ValueType>& keys, float time)
			{
				cache.Min = std::lower_bound(keys.keys.begin(), keys.keys.end(), time, KeyType::Comparer() );
				cache.Max = cache.Min + 1;
				if(time < cache.Min->time)
				{
					if(cache.Min != keys.keys.begin())
					{
						--cache.Min;					
						--cache.Max;
					}
					else
					{
						time = cache.Min->time;
					}
				}

				return sampleFromCache(time, cache.Min->time, cache.Max->time);
			}

			inline ReturnType near_getValue(KeysContainer<ValueType>& keys, float time)
			{
				if(keys.keys.size() == 1)
				{
					return keys.keys[0].value;
				}

				typename StoreType::iterator cMin = cache.Min;
				typename StoreType::iterator cMax = cache.Max;
				float minT = cMin->time;
				float maxT = cMax->time;
				if(time < minT)
				{
					--cMin;
					--cMax;

					minT = cMin->time;

					if(time < minT)
					{
						return random_getValue(keys, time);
					}
					else
					{
						maxT = cMax->time;
					}
				}
				else if(time > maxT)
				{
					++cMin;// = cMax;
					++cMax;

					maxT = cMax->time;
					if(time > maxT)
					{
						return random_getValue(keys, time);
					}
					else
					{
						minT = cMin->time;
					}
				}

				cache.Min = cMin;
				cache.Max = cMax;

				return sampleFromCache(time, minT, maxT);
			}

			inline ReturnType sampleFromCache(float time, float minT, float maxT)
			{
				ReturnType minV = cache.Min->value;
				ReturnType maxV = cache.Max->value;
				float d = maxT - minT;

				return nm::lerp( minV, maxV, (time - minT) / d );
			}
			Cache cache;
			ReturnType emptyValue;
		};
	}



	//
	template<class ValueType>
	class curve
	{
	public:
		typedef Private::KeySampler<ValueType> Sampler;
		typedef Private::KeysContainer<ValueType> Container;
		typedef typename Sampler::ReturnType ReturnType;

		inline void getParameterRange(Range<float>& r) const { /*ENFORCE(!container.keys.empty()); */r.minValue = container.keys.front().time; r.maxValue = container.keys.back().time; }
		inline ReturnType getValue(float time) { return sampler.getValue(container, time);	}
		inline void getValue(int keyIdx, float& keyVal, ReturnType& val) const 
		{ 
			keyVal	= container.keys[keyIdx].time;	
			val		= container.keys[keyIdx].value;	
		}
		inline void addKey(float time, const ReturnType value) { container.addKey(time, value); sampler.invalidateCache(); }
		inline void sort(){ container.sort(); }
		inline int size() const { return container.keys.size(); }

		inline void getValueRange(Range<float>& r) const
		{ 
			r.minValue = FLT_MAX;
			r.maxValue = -FLT_MAX;
			for(const Private::Key<ValueType>& key : container.keys)
			{
				if(r.minValue > key.value) r.minValue = key.value;
				if(r.maxValue < key.value) r.maxValue = key.value;
			}
		}

		Sampler sampler;
		Container container;		
	};
}