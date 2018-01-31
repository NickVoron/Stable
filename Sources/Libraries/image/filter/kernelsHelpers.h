// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace image
{
	namespace filter
	{

		
		struct Blur3
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{0.0f, 0.2f,  0.0f },
					{0.2f, 0.2f,  0.2f },
					{0.0f, 0.2f, 0.0f  },
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return 0.0f; }
		};

		struct Blur5
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 0, 0, 1, 0, 0},
					{ 0, 1, 1, 1, 0},
					{ 1, 1, 1, 1, 1},
					{ 0, 1, 1, 1, 0},
					{ 0, 0, 1, 0, 0},
				}; 			   

				return kernelData;
			}

			static float factor() { return 1.0f / 13.0f; }
			static float bias() { return 0.0f; }
		};


		
		struct MotionBlur
		{
			static const int size = 9;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 1, 0, 0, 0, 0, 0, 0, 0, 0},
					{ 0, 1, 0, 0, 0, 0, 0, 0, 0},
					{ 0, 0, 1, 0, 0, 0, 0, 0, 0},
					{ 0, 0, 0, 1, 0, 0, 0, 0, 0},
					{ 0, 0, 0, 0, 1, 0, 0, 0, 0},
					{ 0, 0, 0, 0, 0, 1, 0, 0, 0},
					{ 0, 0, 0, 0, 0, 0, 1, 0, 0},
					{ 0, 0, 0, 0, 0, 0, 0, 1, 0},
					{ 0, 0, 0, 0, 0, 0, 0, 0, 1},
				};							   

				return kernelData;
			}

			static float factor() { return 1.0f / 9.0f; }
			static float bias() { return  0.0f; }
		};

		
		struct FindEdgesHorizontal
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 0,  0,  0,  0,  0 },
					{ 0,  0,  0,  0,  0 },
					{ -1, -1,  2, 0,  0 },
					{ 0,  0,  0,  0,  0 },
					{ 0,  0,  0,  0,  0 },
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return 0.0f; }
		};

		struct FindEdgesVertical
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 0,  0,  0,  0,  0 },
					{ 0,  0,  0,  0,  0 },
					{ -1, -1,  2,  0, 0 },
					{ 0,  0,  0,  0,  0 },
					{ 0,  0,  0,  0,  0 },
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return 0.0f; }
		};

		struct FindEdges45
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ -1,  0,  0,  0, 0 },
					{ 0, -2,  0,  0,  0 },
					{ 0,  0,  6,  0,  0 },
					{ 0,  0,  0, -2,  0 },
					{ 0,  0,  0,  0, -1 },
				};

				return kernelData;
			}

			static float factor() { return  1.0f; }
			static float bias() { return  0.0f; }
		};

		struct FindEdgesAllDirections
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{-1, -1, -1 },
					{-1,  8, -1 },
					{-1, -1, -1 }
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return  0.0f; }
		};

		
		struct Sharpen3
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{-1, -1, -1 },
					{-1,  9, -1 },
					{-1, -1, -1 }
				};

				return kernelData;
			}

			static float factor() { return  1.0f; }
			static float bias() { return 0.0f; }
		};

		struct Sharpen5
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ -1, -1, -1, -1, -1 },
					{ -1,  2,  2,  2, -1 },
					{ -1,  2,  8,  2, -1 },
					{ -1,  2,  2,  2, -1 },
					{ -1, -1, -1, -1, -1 },
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return  0.0f; }
		};

		struct SharpenEdgesExcessively
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 1,  1,  1 },
					{ 1, -7,  1 },
					{ 1,  1,  1 }
				};

				return kernelData;
			}

			static float factor() { return  1.0f; }
			static float bias() { return 0.0f; }
		};

		
		struct Emboss3
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ -1, -1,  0 },
					{ -1,  0,  1 },
					{ 0,  1,  1 }
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return  128.0f; }
		};

		struct Emboss5
		{
			static const int size = 5;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ -1, -1, -1, -1,  0 },
					{ -1, -1, -1,  0,  1 },
					{ -1, -1,  0,  1,  1 },
					{ -1,  0,  1,  1,  1 },
					{ 0,  1,  1,  1,  1 }
				};

				return kernelData;
			}

			static float factor() { return 1.0f; }
			static float bias() { return 128.0f; }
		}; 

		
		struct MeanAndMedian
		{
			static const int size = 3;
			typedef float KernelMatrix[size][size];


			static const KernelMatrix& data() {
				static const KernelMatrix kernelData =
				{
					{ 1, 1, 1 },
					{ 1, 1, 1 },
					{ 1, 1, 1 }
				};

				return kernelData;
			}

			static float factor() { return 1.0f / 9.0f; }
			static float bias() { return  0.0f; }
		};
	}
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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