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

		//StatesBucketBase
		struct StatesBucketBase
		{
			StatesBucketBase()
			{
				renderers().push_back(this);
			}

			virtual void flush() = 0;
		};


		//StatesBucket
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

		//
		template<class Key> struct StateDeterminator;
		template<> struct StateDeterminator<LPDIRECT3DTEXTURE9> { typedef StatesBucket<TexRectRenderer, void*, NativeTexture, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Texture*> { typedef StatesBucket<TexRectRenderer, void*, TextureExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Texture11*> { typedef StatesBucket<TexRectRenderer, void*, TextureExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<multiapi::Texture*> { typedef StatesBucket<TexRectRenderer, void*, MultiapiTexture, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Atlas*> { typedef StatesBucket<TexRectRenderer, void*, AtlasExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<RectAtlasGAPI*> { typedef StatesBucket<TexRectRenderer, void*, NativeAtlasExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<::dx11::PlainTarget*> { typedef StatesBucket<TexRectRenderer, void*, RenderTargetExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<::dx11::DepthStencil*> { typedef StatesBucket<TexRectRenderer, void*, DepthStencilExtrator, PRT_TEXTURED_RECT>	States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<Resources::Font*> { typedef StatesBucket<TexRectRenderer, void*, FontExtrator, PRT_FONT>			States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<SolidRect> { typedef StatesBucket<SolidRectRenderer, void*, VoidTexture, PRT_SOLID_RECT>		States;	static States& determine() { static States states; return states; } };
		template<> struct StateDeterminator<SolidLine> { typedef StatesBucket<LineRenderer, void*, VoidTexture, PRT_LINE>			States;	static States& determine() { static States states; return states; } };

	
		//renderer
		template<class Key> RendererUsingInterface& renderer() { return StateDeterminator<Key>::determine().get(0); }
		template<class Key> RendererUsingInterface& renderer(Key key) { return StateDeterminator<Key>::determine().get(key); }
		template<class Key> void renderer_clear(Key key) { return StateDeterminator<Key>::determine().clear(key); }

	}//
}//