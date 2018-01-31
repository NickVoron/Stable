// Copyright (C) 2015-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "renderers.h"
#include "../internalState.h"

namespace unigui
{
	namespace draw
	{
		template<class TexExtractor>
		struct BucketKeyProcessor
		{
			static void call(void* data)
			{
				TextureSetterAPI setter(TexExtractor::extract(data));
			}
		};

		
		struct StatesBucketBase
		{
			StatesBucketBase()
			{
				renderers().push_back(this);
			}

			virtual void flush() = 0;
		};


		
		template<class Renderer, class Key, class TexExtractor, PrimitiveRenderType primitiveRenderType>
		struct StatesBucket : public StatesBucketBase
		{
			typedef Key key;
			typedef InternalState<Renderer, Key, primitiveRenderType> IS;

			virtual void flush()
			{
				for (auto& p : data)
				{
					BucketKeyProcessor<TexExtractor>::call(p.first);
					p.second->flush();
				}
			}


			IS& get(key k)
			{
				auto& is = data[k];
				if (!is)
				{
					is = std::make_unique<IS>();
					is->data() = k;
				}

				is->invsize() = TexExtractor::invsize(k);
				return *is;
			}

			void clear(key k)
			{
				data.erase(k);
			}

			std::map<Key, std::unique_ptr<IS>> data;
		};

		struct SolidRect {};
		struct SolidLine {};

		
		template<class Key> struct StateDeterminator;
#if defined(USE_WINDOWS)
		template<> struct StateDeterminator<LPDIRECT3DTEXTURE9> { typedef StatesBucket<TexRectRenderer, void*, NativeTexture, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Texture*> { typedef StatesBucket<TexRectRenderer, void*, TextureExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Texture11*> { typedef StatesBucket<TexRectRenderer, void*, TextureExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<multiapi::Texture*> { typedef StatesBucket<TexRectRenderer, void*, MultiapiTexture, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Atlas*> { typedef StatesBucket<TexRectRenderer, void*, AtlasExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<RectAtlasGAPI*> { typedef StatesBucket<TexRectRenderer, void*, NativeAtlasExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<::dx11::PlainTarget*> { typedef StatesBucket<TexRectRenderer, void*, RenderTargetExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<::dx11::DepthStencil*> { typedef StatesBucket<TexRectRenderer, void*, DepthStencilExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Font*> { typedef StatesBucket<TexRectRenderer, void*, FontExtrator, PRT_FONT>			States;	static States& determine() { static States states; return states; } };
#endif
		template<> struct StateDeterminator<SolidRect> { typedef StatesBucket<SolidRectRenderer, void*, VoidTexture, PRT_SOLID_RECT>		States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<SolidLine> { typedef StatesBucket<LineRenderer, void*, VoidTexture, PRT_LINE>			States;	static States& determine() { static States states; return states; } };

	
		
		template<class Key> RendererUsingInterface& renderer() { return StateDeterminator<Key>::determine().get(0); }
		template<class Key> RendererUsingInterface& renderer(Key key) { return StateDeterminator<Key>::determine().get(key); }
		template<class Key> void renderer_clear(Key key) { return StateDeterminator<Key>::determine().clear(key); }

	}
}//



// Copyright (C) 2015-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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