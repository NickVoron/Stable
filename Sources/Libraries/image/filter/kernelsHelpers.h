#pragma once

namespace image
{
	namespace filter
	{

		//Blur
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


		//Motion Blur
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

		//Find Edges
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

		//Sharpen
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

		//Emboss
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

		//Mean and Median
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