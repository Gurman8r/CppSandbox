#include <MemeGraphics/Shapes.h>

namespace ml
{
	/*	Triangle
	* * * * * * * * * */
	const VertexList Shapes::Triangle::Vertices({
		Vertex({  0.0f,  0.5f, 0.0f }, Color::White, { 0.5f, 1.0f }), // top right
		Vertex({  0.5f, -0.5f, 0.0f }, Color::White, { 1.0f, 0.0f }), // bottom right
		Vertex({ -0.5f, -0.5f, 0.0f }, Color::White, { 0.0f, 0.0f }), // bottom left
	});
	const IndexList Shapes::Triangle::Indices({
		0, 1, 2
	});


	/*	Quad
	* * * * * * * * * */
	const VertexList Shapes::Quad::Vertices({
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 1.0f, 1.0f }),
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 1.0f, 0.0f }),
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0.0f, 0.0f }),
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0.0f, 1.0f }),
	});
	const IndexList Shapes::Quad::Indices({
		0, 1, 3,
		1, 2, 3
	});


	/*	Cube
	* * * * * * * * * */
	const VertexList Shapes::Cube::Vertices({
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 1, 1 }), // 0 front
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 1, 0 }), // 1
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0, 0 }), // 2
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0, 1 }), // 3
											 
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1, 1 }), // 4 back
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1, 0 }), // 5
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 0, 0 }), // 6
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 0, 1 }), // 7
											 
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1, 1 }), // 8 right
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1, 0 }), // 9
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 0, 0 }), // 10
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 0, 1 }), // 11
											 
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 1, 1 }), // 12 left
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 1, 0 }), // 13
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0, 0 }), // 14
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0, 1 }), // 15
											 
		Vertex({ -0.5f, +0.5f, +0.5f }, Color::White, { 1, 1 }), // 16 top
		Vertex({ +0.5f, +0.5f, +0.5f }, Color::White, { 1, 0 }), // 17
		Vertex({ +0.5f, +0.5f, -0.5f }, Color::White, { 0, 0 }), // 18
		Vertex({ -0.5f, +0.5f, -0.5f }, Color::White, { 0, 1 }), // 19
											 
		Vertex({ -0.5f, -0.5f, +0.5f }, Color::White, { 1, 1 }), // 20 bottom
		Vertex({ +0.5f, -0.5f, +0.5f }, Color::White, { 1, 0 }), // 21
		Vertex({ +0.5f, -0.5f, -0.5f }, Color::White, { 0, 0 }), // 22
		Vertex({ -0.5f, -0.5f, -0.5f }, Color::White, { 0, 1 }), // 23
	});
	const IndexList Shapes::Cube::Indices({
		0, 1, 3,
		1, 2, 3,

		7, 5, 4,
		7, 6, 5,

		8, 9, 11,
		9, 10, 11,

		15, 13, 12,
		15, 14, 13,

		17, 18, 16,
		18, 19, 16,

		22, 21, 20,
		20, 23, 22,
	});
}