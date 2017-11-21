#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "DXUT/library.include.h"
#include "resourceManager/library.include.h"

namespace Resources
{
	class Font : public Resource
	{
	public:
		typedef CDXUTTextHelper* ConcreteResource;

		Font(const std::string& newName);

		virtual void Clear();
		virtual bool IsValid() const;

		ConcreteResource GetResource() { return text; }

	protected:
		virtual void Compile(stream::ostream& os);
		virtual void LoadSource();
		virtual void LoadCompiled(stream::istream& is);
		virtual void OnLoadCompiledComplete();

	private:
		ConcreteResource text;
		ID3DXFont* font;        
		ID3DXSprite* sprite;
		std::string fontName;
	};
}