#include <MemeGraphics/Shapes.h>

namespace ml
{

	/*	Triangle
	* * * * * * * * * */
	const VertexArray Shapes::TriangleVertices(Primitive::Triangles, {
		Vertex({  0.0f,  0.5f, 0.0f }, Color::White,{ 0.5f, 1.0f }), // top right
		Vertex({  0.5f, -0.5f, 0.0f }, Color::White,{ 1.0f, 0.0f }), // bottom right
		Vertex({ -0.5f, -0.5f, 0.0f }, Color::White,{ 0.0f, 0.0f }), // bottom left
	});
	const IndexArray Shapes::TriangleIndices({
		0, 1, 2
	});


	/*	Quad
	* * * * * * * * * */
	const VertexArray Shapes::QuadVertices(Primitive::Triangles, {
		Vertex({ 1, 1, 0 }, Color::White,{ 1, 1 }),
		Vertex({ 1, 0, 0 }, Color::White,{ 1, 0 }),
		Vertex({ 0, 0, 0 }, Color::White,{ 0, 0 }),
		Vertex({ 0, 1, 0 }, Color::White,{ 0, 1 }),
	});
	const IndexArray Shapes::QuadIndices({
		0, 1, 3,
		1, 2, 3
	});


	/*	Cube
	* * * * * * * * * */
	const VertexArray Shapes::CubeVertices(Primitive::Triangles, {
		Vertex({  1,  1, -1 }, Color::White,{ 1, 1 }), // 0 front
		Vertex({  1, -1, -1 }, Color::White,{ 1, 0 }), // 1
		Vertex({ -1, -1, -1 }, Color::White,{ 0, 0 }), // 2
		Vertex({ -1,  1, -1 }, Color::White,{ 0, 1 }), // 3

		Vertex({  1,  1,  1 }, Color::White,{ 1, 1 }), // 4 back
		Vertex({  1, -1,  1 }, Color::White,{ 1, 0 }), // 5
		Vertex({ -1, -1,  1 }, Color::White,{ 0, 0 }), // 6
		Vertex({ -1,  1,  1 }, Color::White,{ 0, 1 }), // 7

		Vertex({  1,  1,  1 }, Color::White,{ 1, 1 }), // 8 right
		Vertex({  1, -1,  1 }, Color::White,{ 1, 0 }), // 9
		Vertex({  1, -1, -1 }, Color::White,{ 0, 0 }), // 10
		Vertex({  1,  1, -1 }, Color::White,{ 0, 1 }), // 11

		Vertex({ -1,  1,  1 }, Color::White,{ 1, 1 }), // 12 left
		Vertex({ -1, -1,  1 }, Color::White,{ 1, 0 }), // 13
		Vertex({ -1, -1, -1 }, Color::White,{ 0, 0 }), // 14
		Vertex({ -1,  1, -1 }, Color::White,{ 0, 1 }), // 15

		Vertex({ -1,  1,  1 }, Color::White,{ 1, 1 }), // 16 top
		Vertex({  1,  1,  1 }, Color::White,{ 1, 0 }), // 17
		Vertex({  1,  1, -1 }, Color::White,{ 0, 0 }), // 18
		Vertex({ -1,  1, -1 }, Color::White,{ 0, 1 }), // 19

		Vertex({ -1, -1,  1 }, Color::White,{ 1, 1 }), // 20 bottom
		Vertex({  1, -1,  1 }, Color::White,{ 1, 0 }), // 21
		Vertex({  1, -1, -1 }, Color::White,{ 0, 0 }), // 22
		Vertex({ -1, -1, -1 }, Color::White,{ 0, 1 }), // 23
	});
	const IndexArray Shapes::CubeIndices({
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