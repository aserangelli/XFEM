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
	plane[0].Position = { m_TetraVertexPos[10].x + 3.0f, m_TetraVertexPos[10].y + 3.1f, m_TetraVertexPos[10].z + 0.5f,1 };
	plane[1].Position = { m_TetraVertexPos[1].x - 3.5f, m_TetraVertexPos[1].y + 3.8f, m_TetraVertexPos[1].z + 0.5f,1 };
	plane[2].Position = { m_TetraVertexPos[2].x, m_TetraVertexPos[2].y - 5.1f, m_TetraVertexPos[2].z + 0.5f,1 };
	VECTOR4D pointA, pointB, pointC;
	pointA = plane[0].Position;
	pointB = plane[1].Position;
	pointC = plane[2].Position;
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

	for (int i = 0; i < m_TetrahedronFigures.size(); i++)
	{
		VECTOR4D V1 = m_TetraVertexPos[m_TetrahedronFigures[i].v1];
		VECTOR4D V2 = m_TetraVertexPos[m_TetrahedronFigures[i].v2];
		VECTOR4D V3 = m_TetraVertexPos[m_TetrahedronFigures[i].v3];
		VECTOR4D V4 = m_TetraVertexPos[m_TetrahedronFigures[i].v4];


		VECTOR4D intersect;
		VECTOR4D intersectV1V2;
		VECTOR4D intersectV1V3;
		VECTOR4D intersectV2V3;
		VECTOR4D intersectV1V4;
		VECTOR4D intersectV2V4;
		VECTOR4D intersectV3V4;

		VECTOR4D direction;
		VECTOR4D directionV1V2;
		VECTOR4D directionV1V3;
		VECTOR4D directionV2V3;
		VECTOR4D directionV1V4;
		VECTOR4D directionV2V4;
		VECTOR4D directionV3V4;
		VECTOR4D distance;
		directionV1V2 = V2 - V1;
		directionV1V3 = V3 - V1;
		directionV2V3 = V3 - V2;
		directionV1V4 = V4 - V1;
		directionV2V4 = V4 - V2;
		directionV3V4 = V4 - V3;

		// CARA V1 -> V2 -> V3
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3) || RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3)))
		{
			distance = V1 - intersectV1V2;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2) || RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3)))
		{
			distance = V1 - intersectV1V3;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3) || RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2)))
		{
			distance = V2 - intersectV2V3;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}


		// CARA V1 ->  V2 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4) || RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4)))
		{
			distance = V1 - intersectV1V2;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V4;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2) || RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4)))
		{
			distance = V1 - intersectV1V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V4;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4) || RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V2, intersectV1V2)))
		{
			distance = V2 - intersectV2V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}



		// CARA V1 -> V3 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4) || RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)))
		{
			distance = V1 - intersectV1V3;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3) || RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)))
		{
			distance = V1 - intersectV1V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}
		if ((RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V4, intersectV1V4) || RayCastOnTriangle(pointA, pointB, pointC, V1, directionV1V3, intersectV1V3)))
		{
			distance = V3 - intersectV3V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}

		// CARA V2 -> V3 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3)) && (RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4) || RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)))
		{
			distance = V2 - intersectV2V3;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}
		if ((RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3) || RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)))
		{
			distance = V2 - intersectV2V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}
		if ((RayCastOnTriangle(pointA, pointB, pointC, V3, directionV3V4, intersectV3V4)) && (RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V4, intersectV2V4) || RayCastOnTriangle(pointA, pointB, pointC, V2, directionV2V3, intersectV2V3)))
		{
			distance = V3 - intersectV3V4;
			if (abs(distance.x) < 0.15 && abs(distance.y) < 0.15 && abs(distance.z) < 0.15)
			{
				line[0].Position = V1;
				line[1].Position = V2;
				line[2].Position = V3;
				m_pDXPainter->DrawIndexed(line, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
		}
	}
}

void CVMesh::LoadMSHFile()
{
	bool node = 0;
	bool element = 0;
	ifstream tourus("..\\Assets\\TetraCube.msh");
	string line;
	if (tourus.is_open())
	{
		while (getline(tourus, line))
		{
			if (line == "$Nodes")
			{
				tourus >> totalNodes;
				m_Vertices.resize(totalNodes + 1);
				for (int i = 1; i < totalNodes + 1; i++)
				{
					int indice;
					tourus >> indice;
					tourus >> m_Vertices[i].Position.x;
					tourus >> m_Vertices[i].Position.y;
					tourus >> m_Vertices[i].Position.z;
					m_Vertices[i].Position.w = 1;
					m_TetraVertexPos._Insert_or_assign(indice, m_Vertices[i].Position);
				}
			}

			else if (line == "$Elements")
			{
				tourus >> totalTetrahedron;
				m_IndicesTetrahedrosBuffer.resize(totalTetrahedron * 4);
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

	m_Indices.resize(m_IndicesTetrahedrosBuffer.size() * 3);

	for (unsigned long int i = 0, j = 0; i < m_IndicesTetrahedrosBuffer.size(); i += 4, j += 12)
	{
		int v0, v1, v2, v3;
		v0 = m_IndicesTetrahedrosBuffer[i];
		v1 = m_IndicesTetrahedrosBuffer[i + 1];
		v2 = m_IndicesTetrahedrosBuffer[i + 2];
		v3 = m_IndicesTetrahedrosBuffer[i + 3];


		m_Indices[j] = v0;
		m_Indices[j + 1] = v2;
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
