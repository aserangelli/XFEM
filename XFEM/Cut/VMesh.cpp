#include "stdafx.h"
#include "VMesh.h"
#include <fstream>
#include <string>
#include <iostream>

CVMesh::CVMesh()
{
}


CVMesh::~CVMesh()
{
}

void CVMesh::CutTetrahedron(CDXBasicPainter* m_pDXPainter)
{
	CDXBasicPainter::VERTEX plane[3];
	unsigned long   m_lIndicesFrame[6];
	plane[0].Position = { m_VerticesTetrahedron[0].x, m_VerticesTetrahedron[0].y + .1f, m_VerticesTetrahedron[0].z + 1.1f,1 };
	plane[1].Position = { m_VerticesTetrahedron[1].x, m_VerticesTetrahedron[1].y + .1f, m_VerticesTetrahedron[1].z + 1.1f,1 };
	plane[2].Position = { m_VerticesTetrahedron[2].x, m_VerticesTetrahedron[2].y + .1f, m_VerticesTetrahedron[2].z + 1.1f,1 };

	plane[0].Color = { 1,0,0,0 };
	plane[1].Color = { 1,0,0,0 };
	plane[2].Color = { 1,0,0,0 };

	m_lIndicesFrame[0] = 0;
	m_lIndicesFrame[1] = 1;
	m_lIndicesFrame[2] = 2;

	m_pDXPainter->m_Params.Flags1 |= DRAW_JUST_WITH_COLOR;
	m_pDXPainter->DrawIndexed(plane, 3, m_lIndicesFrame, 3, PAINTER_DRAW);
	CDXBasicPainter::VERTEX line[3];
	line[0].Color = { 0,1,1,0 };
	line[1].Color = { 0,1,1,0 };
	line[2].Color = { 0,1,1,0 };
	unsigned long   m_lIndicesF[3];
	m_lIndicesF[0] = 0;
	m_lIndicesF[1] = 1;
	m_lIndicesF[2] = 2;
	if (1)
	{
		//cout << "=====================================" << endl;
		//cout << "=============CUTING===================" << endl;
		//cout << "=====================================" << endl;
		for (int i = 0; i < m_TetrahedronFigures.size(); i++)
		{
			VECTOR4D V1 = m_VerticesTetrahedron[m_TetrahedronFigures[i].v1];
			VECTOR4D V2 = m_VerticesTetrahedron[m_TetrahedronFigures[i].v2];
			VECTOR4D V3 = m_VerticesTetrahedron[m_TetrahedronFigures[i].v3];
			VECTOR4D V4 = m_VerticesTetrahedron[m_TetrahedronFigures[i].v4];

			//cout << "Figure Number: " << i << endl;
			//cout << "V1 x: " << V1.x << ", V1 y: " << V1.y << ", V1 z: " << V1.z << endl;
			//cout << "V2 x: " << V2.x << ", V2 y: " << V2.y << ", V2 z: " << V2.z << endl;
			//cout << "V3 x: " << V3.x << ", V3 y: " << V3.y << ", V3 z: " << V3.z << endl;
			//cout << "V4 x: " << V4.x << ", V4 y: " << V4.y << ", V4 z: " << V4.z << endl;
			VECTOR4D intersect;

			VECTOR4D direction;
			VECTOR4D distance;
			direction = V2 - V1;

			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V1, direction, intersect))
			{
				distance = V1 -intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V1;
					line[1].Position = V2;
					line[2].Position = V3;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
			direction = V3 - V1;
			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V1, direction, intersect))
			{
				distance = V1 - intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V1;
					line[1].Position = V2;
					line[2].Position = V3;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}

			direction = V4 - V1;
			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V1, direction, intersect))
			{
				distance = V1 - intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V1;
					line[1].Position = V2;
					line[2].Position = V4;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}

			direction = V3 - V2;
			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V2, direction, intersect))
			{
				distance = V2 - intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V2;
					line[1].Position = V3;
					line[2].Position = V4;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}

			direction = V4 - V2;
			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V2, direction, intersect))
			{
				distance = V2 - intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V2;
					line[1].Position = V3;
					line[2].Position = V4;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}

			direction = V4 - V3;
			if (RayCastOnTriangle(plane[0].Position, plane[1].Position, plane[2].Position, V3, direction, intersect))
			{
				distance = V3 - intersect;
				if (distance.x < 0.15 && distance.y <0.15 && distance.z < 0.15)
				{
					line[0].Position = V2;
					line[1].Position = V3;
					line[2].Position = V4;
					//m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}


		}
		//cout << "=====================================" << endl;
		//cout << "=============CUTING===================" << endl;
		//cout << "=====================================" << endl;
		//asd = false;
	}
}

void CVMesh::LoadMSHFile(char * filename)
{
	bool node = 0;
	bool element = 0;
	ifstream tourus("..\\Assets\\cube.msh");
	string line;
	if (tourus.is_open())
	{
		while (getline(tourus, line))
		{
			if (line == "$Nodes")
			{
				tourus >> totalNodes;
				m_Vertices.resize(totalNodes +1);
				for (int i = 1; i < totalNodes +1; i++)
				{
					int indice;
					tourus >> indice;
					tourus >> m_Vertices[i].Position.x;
					tourus >> m_Vertices[i].Position.y;
					tourus >> m_Vertices[i].Position.z;
					m_Vertices[i].Position.w = 1;
					m_VerticesTetrahedron._Insert_or_assign(indice, m_Vertices[i].Position);
				}
			}

			else if (line == "$Elements")
			{
				tourus >> totalTetrahedron;
				m_IndicesTetrahedrosBuffer.resize(totalTetrahedron *4);
				for (int i = 0; i < totalTetrahedron; i++)
				{
					int v0;
					int v1;
					int v2;
					int v3;
					int v4;

					int offset = i * 4;
					tourus >> v0;
					tourus >> v1;
					tourus >> v2;
					tourus >> v3;
					tourus >> v4;
					tourus >> m_IndicesTetrahedrosBuffer[offset];
					tourus >> m_IndicesTetrahedrosBuffer[offset + 1];
					tourus >> m_IndicesTetrahedrosBuffer[offset + 2];
					tourus >> m_IndicesTetrahedrosBuffer[offset + 3];
					TetrahedronFigure aux;
					aux.v1 = m_IndicesTetrahedrosBuffer[offset];
					aux.v2 = m_IndicesTetrahedrosBuffer[offset + 1];
					aux.v3 = m_IndicesTetrahedrosBuffer[offset + 2];
					aux.v4 = m_IndicesTetrahedrosBuffer[offset + 3];
					m_TetrahedronFigures._Insert_or_assign(v0, aux);
				}

			}

		}
	}
	tourus.close();

	m_Indices.resize(m_IndicesTetrahedrosBuffer.size()*3);

	for (unsigned long int i = 0, j = 0; i < m_IndicesTetrahedrosBuffer.size(); i+= 4, j += 12)
	{
		int v0, v1, v2, v3;
		v0 = m_IndicesTetrahedrosBuffer[i];
		v1 = m_IndicesTetrahedrosBuffer[i+1];
		v2 = m_IndicesTetrahedrosBuffer[i+2];
		v3 = m_IndicesTetrahedrosBuffer[i+3];


		m_Indices[j] = v0;
		m_Indices[j+1] = v2;
		m_Indices[j + 2] = v1;

		m_Indices[j + 3] = v2;
		m_Indices[j + 4] = v3;
		m_Indices[j + 5] = v1;

		m_Indices[j + 6] = v3;
		m_Indices[j + 7] = v0;
		m_Indices[j + 8] = v1;

		m_Indices[j + 9] = v0;
		m_Indices[j + 10] = v3;
		m_Indices[j + 11] = v2;
	}


	for (unsigned long j = 0; j < m_Vertices.size(); j++)
	{
		VECTOR4D TexCoord = { 0,0,0,0 };
		TexCoord.x = m_Vertices[j].Position.x;
		TexCoord.y = m_Vertices[j].Position.z;
		TexCoord.z = m_Vertices[j].Position.y;
		TexCoord = Normalize(TexCoord);
		TexCoord.x = TexCoord.x * 0.5 + 0.5;
		TexCoord.y = TexCoord.y * 0.5 + 0.5;

		m_Vertices[j].TexCoord = TexCoord;
	}

	BuildTangentSpaceFromTexCoordsIndexed(true);

}
