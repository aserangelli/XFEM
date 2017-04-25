#include "stdafx.h"
#include "VMesh.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#define TRESHOLD 0.006// antes 0.02,,, 0.01 ,,,,
#define TRESHOLD2 0.006// antes 0.1,,,, 0.03,,, 0.01




CVMesh::CVMesh()
{
}


CVMesh::~CVMesh()
{
}

void CVMesh::CutTetrahedron(CDXBasicPainter* m_pDXPainter, unsigned long m_nFlagsPainter, CVMesh m_plane)
{


	/*plane[0].Position = { m_TetraVertexPos[10].x + 3.0f, m_TetraVertexPos[10].y + 3.1f, m_TetraVertexPos[10].z + 0.5f,1 };
	plane[1].Position = { m_TetraVertexPos[1].x - 3.5f, m_TetraVertexPos[1].y + 3.8f, m_TetraVertexPos[1].z + 0.5f,1 };
	plane[2].Position = { m_TetraVertexPos[2].x, m_TetraVertexPos[2].y - 5.1f, m_TetraVertexPos[2].z + 0.5f,1 };*/

	plane[0].Position = m_plane.m_Vertices[0].Position;
	plane[1].Position = m_plane.m_Vertices[1].Position;
	plane[2].Position = m_plane.m_Vertices[2].Position;
	/*plane[0].Position = { 3.50000000f, 4.09999990f, 1.60000000f,1 };
	plane[1].Position = { -3.50000000f, 3.79999995f,1.60000000f,1 };
	plane[2].Position = { 1.00000000f, -5.09999990f,1.60000000f,1 };*/

	//plane[0].Position = { 3.50000000f, 4.09999990f, 3.20000000f,1 };
	//plane[1].Position = { -3.50000000f, 3.79999995f,1.00000000f,1 };
	//plane[2].Position = { 1.00000000f, -5.09999990f,1.00000000f,1 };

	// Plano que corta a Y usando el tetraedro 281
	/*
	plane[0].Position = { m_Vertices[93].Position.x -0.003f , m_Vertices[93].Position.y -0.003f , m_Vertices[93].Position.z -0.002f,1 };
	plane[1].Position = { m_Vertices[101].Position.x +0.003f , m_TetraVertexPos[101].y +0.003f, m_TetraVertexPos[101].z - 0.002f,1 };
	plane[2].Position = { m_Vertices[105].Position.x , m_Vertices[105].Position.y + 1.2f, m_Vertices[105].Position.z  ,1 };
	*/

	//CDXBasicPainter pl = {  3.50000000f, 4.09999990f, 1.60000000f,1,
	//				 -3.50000000f, 3.79999995f,1.60000000f,1,
	//				 1.00000000f, -5.09999990f,1.60000000f,1,
	//							0,			0,			0,	1,};


	VECTOR4D pointA, pointB, pointC;
	pointA = plane[0].Position;
	pointB = plane[1].Position;
	pointC = plane[2].Position;
	plane[0].Color = { 1,0,0,0 };
	plane[1].Color = { 1,0,0,0 };
	plane[2].Color = { 1,0,0,0 };
	unsigned long   m_lIndicesFrame[6];
	m_lIndicesFrame[0] = 0;
	m_lIndicesFrame[1] = 1;
	m_lIndicesFrame[2] = 2;


	m_pDXPainter->m_Params.Flags1 |= DRAW_JUST_WITH_COLOR;
	m_pDXPainter->DrawIndexed(plane, 3, m_lIndicesFrame, 3, m_nFlagsPainter); // PAINTER_DRAW_WIREFRAME

	CDXBasicPainter::VERTEX triangle[3];
	triangle[0].Color = { 0,1,1,0 };
	triangle[1].Color = { 0,1,1,0 };
	triangle[2].Color = { 0,1,1,0 };

	CDXBasicPainter::VERTEX triangle2[3];
	triangle2[0].Color = { 1,0,1,0 };
	triangle2[1].Color = { 1,0,1,0 };
	triangle2[2].Color = { 1,0,1,0 };

	unsigned long   m_lIndicesF[3];
	m_lIndicesF[0] = 0;
	m_lIndicesF[1] = 1;
	m_lIndicesF[2] = 2;

	vector<CDXBasicPainter::VERTEX> cutTetraVertex;

	for (int i = 0; i < m_TetrahedronFigures.size(); i++)
	{
		VECTOR4D V1 = m_TetraVertexPos[m_TetrahedronFigures[i].v1];
		VECTOR4D V2 = m_TetraVertexPos[m_TetrahedronFigures[i].v2];
		VECTOR4D V3 = m_TetraVertexPos[m_TetrahedronFigures[i].v3];
		VECTOR4D V4 = m_TetraVertexPos[m_TetrahedronFigures[i].v4];


		VECTOR4D distanceNode1;
		VECTOR4D distanceNode2;
		m_TetrahedronFigures[i].directionV1V2 = Normalize( V2 - V1);
		m_TetrahedronFigures[i].directionV1V3 = Normalize(V3 - V1);
		m_TetrahedronFigures[i].directionV2V3 = Normalize(V3 - V2);
		m_TetrahedronFigures[i].directionV1V4 = Normalize(V4 - V1);
		m_TetrahedronFigures[i].directionV2V4 = Normalize(V4 - V2);
		m_TetrahedronFigures[i].directionV3V4 = Normalize(V4 - V3);

		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool topV1 = planeEquation.OnAboveOrBelow(V1);
		bool topV2 = planeEquation.OnAboveOrBelow(V2);
		bool topV3 = planeEquation.OnAboveOrBelow(V3);
		bool topV4 = planeEquation.OnAboveOrBelow(V4);


		if ((topV1 && topV2 && topV3 && topV4) )
			continue;
		else if ((!topV1 && !topV2 && !topV3 && !topV4))
		{
			continue;
		}
		// CARA V1 -> V2 -> V3
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2)) &&
			(RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3) ||
			 RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V2;
			distanceNode2 = V2 - m_TetrahedronFigures[i].intersectV1V2;
			// Revizar si el corte esta cerca del Nodo V1
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if(m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			// Revizar si el corte esta cerca del Nodo V2
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V2.x);
				maxX = max(V1.x, V2.x);

				minY = min(V1.y, V2.y);
				maxY = max(V1.y, V2.y);

				minZ = min(V1.z, V2.z);
				maxZ = max(V1.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV1V2.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V2.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V2.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V3;
					m_TetrahedronFigures[i].bCutEdge[1] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}

		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V3;
			distanceNode2 = V3 - m_TetrahedronFigures[i].intersectV1V3;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V3.x);
				maxX = max(V1.x, V3.x);

				minY = min(V1.y, V3.y);
				maxY = max(V1.y, V3.y);

				minZ = min(V1.z, V3.z);
				maxZ = max(V1.z, V3.z);

				if ((m_TetrahedronFigures[i].intersectV1V3.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V3.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V3.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V3;
					m_TetrahedronFigures[i].bCutEdge[2] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}

		 if ((RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2)))
		{
			distanceNode1 = V2 - m_TetrahedronFigures[i].intersectV2V3;
			distanceNode2 = V3 - m_TetrahedronFigures[i].intersectV2V3;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V3;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V3.x, V2.x);
				maxX = max(V3.x, V2.x);

				minY = min(V3.y, V2.y);
				maxY = max(V3.y, V2.y);

				minZ = min(V3.z, V2.z);
				maxZ = max(V3.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV2V3.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V3.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V3.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V3;
					m_TetrahedronFigures[i].bCutEdge[4] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}

		// CARA V1 ->  V2 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2)) &&
			(RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4) ||
			 RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V2;
			distanceNode2 = V2 - m_TetrahedronFigures[i].intersectV1V2;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V2.x);
				maxX = max(V1.x, V2.x);

				minY = min(V1.y, V2.y);
				maxY = max(V1.y, V2.y);

				minZ = min(V1.z, V2.z);
				maxZ = max(V1.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV1V2.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V2.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V2.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V2.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[1] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}

		 if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV1V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V4.x);
				maxX = max(V1.x, V4.x);

				minY = min(V1.y, V4.y);
				maxY = max(V1.y, V4.y);

				minZ = min(V1.z, V4.z);
				maxZ = max(V1.z, V4.z);

				if ((m_TetrahedronFigures[i].intersectV1V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[3] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}
			}
		}

		 if ((RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V2, m_TetrahedronFigures[i].intersectV1V2)))
		{
			distanceNode1 = V2 - m_TetrahedronFigures[i].intersectV2V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV2V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V2;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V4.x, V2.x);
				maxX = max(V4.x, V2.x);

				minY = min(V4.y, V2.y);
				maxY = max(V4.y, V2.y);

				minZ = min(V4.z, V2.z);
				maxZ = max(V4.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV2V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V2;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[6] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}



		// CARA V1 -> V3 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3)) &&
			(RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4) ||
			 RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V3;
			distanceNode2 = V3 - m_TetrahedronFigures[i].intersectV1V3;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V3.x);
				maxX = max(V1.x, V3.x);

				minY = min(V1.y, V3.y);
				maxY = max(V1.y, V3.y);

				minZ = min(V1.z, V3.z);
				maxZ = max(V1.z, V3.z);

				if ((m_TetrahedronFigures[i].intersectV1V3.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V3.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V3.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V3.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[2] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}

		 if ((RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)))
		{
			distanceNode1 = V1 - m_TetrahedronFigures[i].intersectV1V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV1V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[1] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V1.x, V4.x);
				maxX = max(V1.x, V4.x);

				minY = min(V1.y, V4.y);
				maxY = max(V1.y, V4.y);

				minZ = min(V1.z, V4.z);
				maxZ = max(V1.z, V4.z);

				if ((m_TetrahedronFigures[i].intersectV1V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV1V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV1V4.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V1;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[3] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}

			}
		}
		 if ((RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V4, m_TetrahedronFigures[i].intersectV1V4) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V1, m_TetrahedronFigures[i].directionV1V3, m_TetrahedronFigures[i].intersectV1V3)))
		{
			distanceNode1 = V3 - m_TetrahedronFigures[i].intersectV3V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV3V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V1;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V3.x, V4.x);
				maxX = max(V3.x, V4.x);

				minY = min(V3.y, V4.y);
				maxY = max(V3.y, V4.y);

				minZ = min(V3.z, V4.z);
				maxZ = max(V3.z, V4.z);

				if ((m_TetrahedronFigures[i].intersectV3V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV3V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV3V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.z < maxZ + TRESHOLD2))
				 {
					triangle2[0].Position = V1;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[5] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}
			}
		}

		// CARA V2 -> V3 -> V4
		if ((RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3)) &&
			(RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4) ||
			 RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)))
		{
			distanceNode1 = V2 - m_TetrahedronFigures[i].intersectV2V3;
			distanceNode2 = V3 - m_TetrahedronFigures[i].intersectV2V3;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V3.x, V2.x);
				maxX = max(V3.x, V2.x);

				minY = min(V3.y, V2.y);
				maxY = max(V3.y, V2.y);

				minZ = min(V3.z, V2.z);
				maxZ = max(V3.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV2V3.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V3.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V3.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V3.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V2;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[4] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}
			}
		}
		 if ((RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)))
		{
			distanceNode1 = V2 - m_TetrahedronFigures[i].intersectV2V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV2V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[2] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V4.x, V2.x);
				maxX = max(V4.x, V2.x);

				minY = min(V4.y, V2.y);
				maxY = max(V4.y, V2.y);

				minZ = min(V4.z, V2.z);
				maxZ = max(V4.z, V2.z);

				if ((m_TetrahedronFigures[i].intersectV2V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV2V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV2V4.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V2;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[6] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}
			}
		}
		 if ((RayCastOnTriangle(pointA, pointB, pointC, V3, m_TetrahedronFigures[i].directionV3V4, m_TetrahedronFigures[i].intersectV3V4)) &&
				 (RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V4, m_TetrahedronFigures[i].intersectV2V4) ||
				  RayCastOnTriangle(pointA, pointB, pointC, V2, m_TetrahedronFigures[i].directionV2V3, m_TetrahedronFigures[i].intersectV2V3)))
		{
			distanceNode1 = V3 - m_TetrahedronFigures[i].intersectV3V4;
			distanceNode2 = V4 - m_TetrahedronFigures[i].intersectV3V4;
			// Si esta dentro del treshold es un corte cerca del nodo, de lo contrario es un corte en un edge
			if (abs(distanceNode1.x) < TRESHOLD && abs(distanceNode1.y) < TRESHOLD && abs(distanceNode1.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[3] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else if (abs(distanceNode2.x) < TRESHOLD && abs(distanceNode2.y) < TRESHOLD && abs(distanceNode2.z) < TRESHOLD)
			{
				triangle[0].Position = V2;
				triangle[1].Position = V3;
				triangle[2].Position = V4;
				m_TetrahedronFigures[i].bCutNode[4] = true;
				if (m_debugPaint)
					m_pDXPainter->DrawIndexed(triangle, 3, m_lIndicesF, 3, PAINTER_DRAW);
			}
			else
			{
				float minX, maxX, minY, maxY, minZ, maxZ;
				minX = min(V3.x, V4.x);
				maxX = max(V3.x, V4.x);

				minY = min(V3.y, V4.y);
				maxY = max(V3.y, V4.y);

				minZ = min(V3.z, V4.z);
				maxZ = max(V3.z, V4.z);

				if ((m_TetrahedronFigures[i].intersectV3V4.x > minX - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.x < maxX + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV3V4.y > minY - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.y < maxY + TRESHOLD2) &&
					(m_TetrahedronFigures[i].intersectV3V4.z > minZ - TRESHOLD2 && m_TetrahedronFigures[i].intersectV3V4.z < maxZ + TRESHOLD2))
				{
					triangle2[0].Position = V2;
					triangle2[1].Position = V3;
					triangle2[2].Position = V4;
					m_TetrahedronFigures[i].bCutEdge[5] = true;
					if (m_debugPaint)
						m_pDXPainter->DrawIndexed(triangle2, 3, m_lIndicesF, 3, PAINTER_DRAW);
				}
			}
		}
	}
	//m_pDXPainter->DrawIndexed(&cutTetraVertex[0], cutTetraVertex.size(), m_lIndicesF, 135, PAINTER_DRAW);
}

void CVMesh::LoadPlane()
{
	totalNodes = 3;
	m_Vertices.resize(3);
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		if (i == 0)
		{
			m_Vertices[i].Position = { 3.50000000f, 4.09999990f, 1.60000000f,1 };
		}
		else if (i == 1)
		{
			m_Vertices[i].Position = { -3.50000000f, 3.79999995f,1.60000000f,1 };
		}
		else
		{
			m_Vertices[i].Position = { 1.00000000f, -5.09999990f,1.60000000f,1 };
		}
	}

	m_Indices.resize(3);
	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;

	for (unsigned long j = 0; j < m_Vertices.size(); j++)
	{
		VECTOR4D TexCoord = { 0,0,0,0 };
		TexCoord.x = m_Vertices[j].Position.x;
		TexCoord.y = m_Vertices[j].Position.y;
		TexCoord.z = m_Vertices[j].Position.z;
		TexCoord = Normalize(TexCoord);
		TexCoord.x = TexCoord.x * 0.5 + 0.5;
		TexCoord.y = TexCoord.y * 0.5 + 0.5;

		m_Vertices[j].TexCoord = TexCoord;
	}
	BuildTangentSpaceFromTexCoordsIndexed(true);
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
	//int aux = m_TetrahedronFigures.size();
	tourus.close();
	BuildTetrahedrons();


}

void CVMesh::UpdatePlane()
{
}

void CVMesh::IdentifyCutType(CDXBasicPainter* m_pDXPainter)
{
	int a = 0; int b = 0;  int x = 0; int  y = 0; int z = 0; int u = 0;

	for (int i = 0; i < m_TetrahedronFigures.size(); i++)
	{
		int nNodes = 0;
		int nEdges = 0;

		for (int j = 1; j < 5; j++)
		{
			if (m_TetrahedronFigures[i].bCutNode[j] == true)
			{
				nNodes += 1;
			}

		}
		for (int h = 1; h < 7; h++)
		{
			if (m_TetrahedronFigures[i].bCutEdge[h] == true)
			{
				nEdges += 1;
			}
		}
		m_cutType[nEdges][nNodes] == 'a' ? a++ : m_cutType[nEdges][nNodes] == 'b' ? b++ : m_cutType[nEdges][nNodes] == 'x' ? x++ :
			m_cutType[nEdges][nNodes] == 'y' ? y++ : m_cutType[nEdges][nNodes] == 'z' ? z++ : m_cutType[nEdges][nNodes] == 'u' ? u++ : 1;

		m_TetrahedronFigures[i].cutInfo.type_cut = m_cutType[nEdges][nNodes];
		if (flag)
		{
			cout << "Tetrahedron No: " << i << " is type: " << (char)m_TetrahedronFigures[i].cutInfo.type_cut << endl;
		}
		if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'a' )
		{
			if(flag)
				SplitElementTypeA(i, m_TetrahedronFigures);
		}
		else if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'b')
		{
			if (flag)
				SplitElementTypeB(i, m_TetrahedronFigures);
		}
		else if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'x')
		{
			if (flag)
				SplitElementTypeX(i, m_TetrahedronFigures);
		}
		else if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'y')
		{
			if (flag)
				SplitElementTypeY(i, m_TetrahedronFigures);
		}
		else if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'z')
		{
			if (flag)
				SplitElementTypeZ(i, m_TetrahedronFigures);
		}

	}
	if (flag)
	{
		cout << "Total type A tetrahedrons: " << a << endl;
		cout << "Total type B tetrahedrons: " << b << endl;
		cout << "Total type X tetrahedrons: " << x << endl;
		cout << "Total type Y tetrahedrons: " << y << endl;
		cout << "Total type Z tetrahedrons: " << z << endl;
		cout << "Total type Undifined tetrahedrons: " << u << endl;
		for (int i = 0; i < m_TetrahedronFigures.size(); i++)
		{
			VECTOR4D V1 = m_TetraVertexPos[m_TetrahedronFigures[i].v1];
			VECTOR4D V2 = m_TetraVertexPos[m_TetrahedronFigures[i].v2];
			VECTOR4D V3 = m_TetraVertexPos[m_TetrahedronFigures[i].v3];
			VECTOR4D V4 = m_TetraVertexPos[m_TetrahedronFigures[i].v4];

			CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
			bool topV1 = planeEquation.OnAboveOrBelow(V1);
			bool topV2 = planeEquation.OnAboveOrBelow(V2);
			bool topV3 = planeEquation.OnAboveOrBelow(V3);
			bool topV4 = planeEquation.OnAboveOrBelow(V4);
			if ((!topV1 && !topV2 && !topV3 && !topV4))
			{
				DisplaceBottomVertexes(topV1, m_TetrahedronFigures[i].v1, m_TetrahedronFigures[i].v2, m_TetrahedronFigures[i].v3);
				DisplaceBottomVertexes(topV1, m_TetrahedronFigures[i].v2, m_TetrahedronFigures[i].v3, m_TetrahedronFigures[i].v4);
			}
		}

	}

	flag = false;

}

void CVMesh::SplitElementTypeY(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures)
{
	TetrahedronFigure above;
	TetrahedronFigure below;
	CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
	int indexTetraBuffer = nIdTetrahedronCut * 4;

	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, 1, 3, 2, above, below, top, indexTetraBuffer, 31);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;
	}
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1])
	{

		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, 3, 2, 1, above, below, top, indexTetraBuffer,31);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		/*m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;*/

	}
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, 2, 1, 3, above, below, top, indexTetraBuffer, 31);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		/*m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;*/

	}
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position);
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, 1, 2, 4, above, below, top, indexTetraBuffer, 31);
		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		/*m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;*/

	}
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, 3, 1, 4, above, below, top, indexTetraBuffer, 31);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		/*m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;*/

	}
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ARRIBA
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, 1, 4, 3, above, below, top, indexTetraBuffer, 31);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		/*m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;*/
	}


	m_TetrahedronFigures._Insert_or_assign(nIdTetrahedronCut, above);

	AddIndexToBuffer(below.v1, below.v2, below.v3, below.v4);
	BuildTetrahedrons();

}

void CVMesh::SplitElementTypeZ(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures)
{
	int indexTetraBuffer = nIdTetrahedronCut * 4;
	CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4])
	{
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		DisplaceTopVertex(top, m_TetrahedronFigures[nIdTetrahedronCut].v1);
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = totalNodes;
	}

	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4])
	{
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position);
		DisplaceTopVertex(top, m_TetrahedronFigures[nIdTetrahedronCut].v2);
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 4] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = totalNodes;
	}

	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3])
	{
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position);
		DisplaceTopVertex(top, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 4] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = totalNodes;
	}

	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4])
	{
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);
		DisplaceTopVertex(top, m_TetrahedronFigures[nIdTetrahedronCut].v3);
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 4] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = totalNodes;
		AddVertexToBuffer(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, top);
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = totalNodes;
	}
	BuildTetrahedrons();
}

void CVMesh::SplitElementTypeX(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures)

{
	TetrahedronFig5V below; //= { 0,0,0,0,0 };
	TetrahedronFigure above; //= { 0,0,0,0 };

	CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
	int indexTetraBuffer = nIdTetrahedronCut * 4;
#pragma region V2 Cut
	// Nodo 2, Edge 2 and 3
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, 2, 1, 3, above, below, top, indexTetraBuffer, 21);


		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;
	}

	// Nodo 2 Edge 2 and 5
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		//above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);

		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, 1, 4, 3, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v4, m_TetrahedronFigures[nIdTetrahedronCut].v1, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
	}
	// Nodo 2 Edge 3 and 5
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position, 4, 2, 3, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v1, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
	}
#pragma endregion

#pragma region V3 Cut
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, 1, 3, 2, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}

	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, 4, 1, 2, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}

	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position, 3, 4, 2, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v2, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;

	}
#pragma endregion

#pragma region V4 Cut
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, 3, 2, 1, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}

	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, 4, 3, 1, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v2, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}

	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v1; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position, 2, 4, 1, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}
#pragma endregion

#pragma region V1 Cut
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, 3, 1, 4, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v2, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;

	}


	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ARRIBA
		top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ABAJO
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, 2, 3, 4, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v2, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;

	}
	// Nodo 1, Edge 4, Edge 6
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutNode[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// Which Node is on Top
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position);
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v2; // ARRIBA
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v4; // ABAJO
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v3; // ABAJO

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position, 1, 2, 4, above, below, top, indexTetraBuffer, 21);

		// Se agrega un displacement a la base para que se vea como se divide
		DisplaceBottomVertexes(top, below.v2, below.v3, below.v4);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v3, below.v4);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v1].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;
	}
#pragma endregion

	m_TetrahedronFigures._Insert_or_assign(nIdTetrahedronCut, above);

	AddIndexToBuffer(below.v5, below.v1, below.v3, below.v4);
	AddIndexToBuffer(below.v1, below.v2, below.v3, below.v4);
	BuildTetrahedrons();

}

void CVMesh::SplitElementTypeB(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures)
{
	TetrahedronFig6V above = { 0,0,0,0,0,0 };
	TetrahedronFig6V below = { 0,0,0,0,0,0 };
	int indexTetraBuffer = nIdTetrahedronCut * 4;
	// V1 & V4 Top  (Ya)
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// ARRIBA
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		above.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v4;
		// ABAJO
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v2;
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v3;

		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		DisplaceTopVertex(top, above.v6);
		bool nodeExist;
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, above, below, 11, top);
		}
		else
		{
			above.v2 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2].top;
			below.v1 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, above, below, 12, top);
		}
		else
		{
			above.v3 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3].top;
			below.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, above, below, 13, top);
		}
		else
		{
			above.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4].top;
			below.v6 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, above, below, 14, top);
		}
		else
		{
			above.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4].top;
			below.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4].bottom;
		}

		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = above.v2;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = above.v3;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = above.v6;
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v3);
		DisplaceBottomVertexes(top, below.v2, below.v2, below.v3);
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v2].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v3].Position.z -= top ? displacement : 0;
		//m_Vertices[m_TetrahedronFigures[nIdTetrahedronCut].v4].Position.z -= top ? displacement : 0;
	}
	// V1 & V2 Top (Ya)
	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		// ARRIBA
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		above.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v2;
		// ABAJO
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v4;

		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		DisplaceTopVertex(top, above.v6);
		bool nodeExist;
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, above, below, 11, top);
		}
		else
		{
			above.v2 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3].top;
			below.v1 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3].bottom;

		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4);
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, above, below, 12, top);
		}
		else
		{
			above.v3 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4].top;
			below.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3);
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, above, below, 13, top);
		}
		else
		{
			above.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3].top;
			below.v6 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4);
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, above, below, 14, top);
		}
		else
		{
			above.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4].top;
			below.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4].bottom;
		}
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = above.v2;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = above.v3;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = above.v6;

		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		DisplaceBottomVertexes(top, below.v2, below.v2, below.v3);
	}
	// V1 & V3 Top   (YA)
	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		// ARRIBA
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		above.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		// ABAJO
		below.v2 = m_TetrahedronFigures[nIdTetrahedronCut].v4;
		below.v3 = m_TetrahedronFigures[nIdTetrahedronCut].v2;

		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		DisplaceTopVertex(top, above.v6);
		bool nodeExist;
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, above, below, 11, top);
		}
		else
		{
			above.v2 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4].top;
			below.v1 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, above, below, 12, top);
		}
		else
		{
			above.v3 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2].top;
			below.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, above, below, 13, top);
		}
		else
		{
			above.v4 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4].top;
			below.v6 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4].bottom;
		}
		nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3);
		// No lo encontro
		if (!nodeExist)
		{
			AddNewPointsOfControl(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, above, below, 14, top);
		}
		else
		{
			above.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3].top;
			below.v5 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3].bottom;
		}
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = above.v2;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = above.v3;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = above.v6;

		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		DisplaceBottomVertexes(top, below.v2, below.v2, below.v3);

	}




	//AddIndexToBuffer(above.v1, above.v2, above.v3, above.v6);
	AddIndexToBuffer(above.v2, above.v4, above.v3, above.v6);
	AddIndexToBuffer(above.v3, above.v4, above.v5, above.v6);


	AddIndexToBuffer(below.v4, below.v3, below.v5, below.v1);
	AddIndexToBuffer(below.v1, below.v6, below.v2, below.v5);
	AddIndexToBuffer(below.v2, below.v1, below.v3, below.v5);
	BuildTetrahedrons();
}

void CVMesh::SplitElementTypeA(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures)
{
	TetrahedronFigure above;
	TetrahedronFig6V below = {0,0,0,0,0,0};
	int indexTetraBuffer = nIdTetrahedronCut * 4;

	//totalTetrahedron += 1;
	//m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));

#pragma region V1 Arriba
	if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3])
	{
		// totalNodes es el total de nodos que existe en mi malla en un principio.
		// de esta forma si le quitamos totalNodes - 3  llegamos al nodo de arriba
		// El Nodo de arriba no se toca en este caso, ya que pasa de forma
		// transparente para el nuevo nodo y no lo comparte con nadie mas.
		bool nodeExist;
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);

		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v2;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v4;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, 3, 2, 1, above, below, top, indexTetraBuffer, 1);

		// Se agrega un displacement a la base para que se vea como se divide
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		DisplaceBottomVertexes(top, below.v4, below.v5, below.v6);
	}
#pragma endregion

#pragma region V4 arriba
	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[3] &&
			 m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6] &&
		     m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		bool nodeExist;
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v4;
		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v2;
		//////////////////////////Nodo V1//////////////////////////////////////
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, 4, 2, 3, above, below, top, indexTetraBuffer, 1);

		// Se agrega un displacement a la base para que se vea como se divide
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v2);
		DisplaceBottomVertexes(top, below.v4, below.v5, below.v6);
	}
#pragma endregion

#pragma region V2 arriba
	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[1] &&
			 m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
			 m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[6])
	{
		bool nodeExist;
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v2;
		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v4;
		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V2, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V4, 2, 4, 1, above, below, top, indexTetraBuffer, 1);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v3, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		DisplaceBottomVertexes(top, below.v4, below.v5, below.v6);
	}
#pragma endregion

#pragma region V3 Arriba
	else if (m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[2] &&
			 m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[4] &&
			 m_TetrahedronFigures[nIdTetrahedronCut].bCutEdge[5])
	{
		// El Nodo de arriba no se toca en este caso, ya que pasa de forma
		// transparente para el nuevo nodo y no lo comparte con nadie mas.
		bool nodeExist;
		above.v1 = m_TetrahedronFigures[nIdTetrahedronCut].v3;
		CPlane planeEquation = CPlane(plane[0].Position, plane[1].Position, plane[2].Position);
		bool top = planeEquation.OnAboveOrBelow(m_Vertices[above.v1].Position);
		//top == false ? m_Vertices[above.v1].Position.z -= displacement : top;
		DisplaceTopVertex(top, above.v1);
		below.v4 = m_TetrahedronFigures[nIdTetrahedronCut].v4;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronCut].v1;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronCut].v2;

		AnalizeCut(m_TetrahedronFigures[nIdTetrahedronCut].intersectV3V4, m_TetrahedronFigures[nIdTetrahedronCut].intersectV1V3, m_TetrahedronFigures[nIdTetrahedronCut].intersectV2V3, 1, 4, 3, above, below, top, indexTetraBuffer, 1);
		//DisplaceBottomVertexes(top, m_TetrahedronFigures[nIdTetrahedronCut].v1, m_TetrahedronFigures[nIdTetrahedronCut].v2, m_TetrahedronFigures[nIdTetrahedronCut].v4);
		DisplaceBottomVertexes(top, below.v4, below.v5, below.v6);
	}
#pragma endregion
	m_TetrahedronFigures._Insert_or_assign(nIdTetrahedronCut, above);

	AddIndexToBuffer(below.v1, below.v2, below.v3, below.v6);
	AddIndexToBuffer(below.v2, below.v4, below.v3, below.v6);
	AddIndexToBuffer(below.v3, below.v4, below.v5, below.v6);

	BuildTetrahedrons();
}

bool CVMesh::FindNode(VECTOR4D position)
{
	bool exist = false;
	exist = m_NewNodesPointOfControl.count(position);
	return exist;
}

void CVMesh::AddNewPointsOfControlA(VECTOR4D intersectionPoint, TetrahedronFigure& above, TetrahedronFig6V& below, int node, bool top, int offset, int indexTetraBuffer)
{
	// ARRIBA
	AddVertexToBuffer(intersectionPoint, top);
	node == 1 ? above.v4 = totalNodes : node == 2 ? above.v2 = totalNodes : node == 3 ? above.v3 = totalNodes : totalNodes;
	// Se sobrescribe lo que habia anteriormente en el indice - offset por el nueva posicion
	m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes;
	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	PointsOfControl newPoints;
	newPoints.top = totalNodes;
	// ABAJO
	AddVertexToBuffer(intersectionPoint, !top);
	node == 1 ? below.v1 = totalNodes : node == 2 ? below.v2 = totalNodes : node == 3 ? below.v3 = totalNodes : totalNodes;

	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	newPoints.bottom = totalNodes;
	m_NewNodesPointOfControl[m_Vertices[totalNodes].Position] = newPoints;
}

void CVMesh::AddNewPointsOfControlB(VECTOR4D intersectionPoint, TetrahedronFig6V& above, TetrahedronFig6V& below, int node, bool top)
{

	// ARRIBA
	totalNodes++;
	m_Vertices.resize(totalNodes + 1);
	m_Vertices[totalNodes].Position.x = intersectionPoint.x;
	m_Vertices[totalNodes].Position.y = intersectionPoint.y;
	m_Vertices[totalNodes].Position.z = intersectionPoint.z - ((top) ? 0 : displacement);
	m_Vertices[totalNodes].Position.w = 1;
	m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
	switch (node)
	{
	case 11:
		above.v2 = totalNodes;
		break;
	case 12:
		above.v3 = totalNodes;
		break;
	case 13:
		above.v4 = totalNodes;
		break;
	case 14:
		above.v5 = totalNodes;
		break;
	}

	// Se sobrescribe lo que habia anteriormente en el indice - offset por el nueva posicion
	//m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes;
	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	PointsOfControl newPoints;
	newPoints.top = totalNodes;

	// ABAJO
	totalNodes++;
	m_Vertices.resize(totalNodes + 1);
	m_Vertices[totalNodes].Position.x = intersectionPoint.x;
	m_Vertices[totalNodes].Position.y = intersectionPoint.y;
	m_Vertices[totalNodes].Position.z = intersectionPoint.z - ((top) ? displacement : 0);
	m_Vertices[totalNodes].Position.w = 1;
	m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
	switch (node)
	{
	case 11:
		below.v1 = totalNodes;
		break;
	case 12:
		below.v4 = totalNodes;
		break;
	case 13:
		below.v6 = totalNodes;
		break;
	case 14:
		below.v5 = totalNodes;
		break;
	}
	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	newPoints.bottom = totalNodes;
	m_NewNodesPointOfControl[m_Vertices[totalNodes].Position] = newPoints;

}

//void CVMesh::AnalizeCutX(VECTOR4D intersectionA, VECTOR4D intersectionB, VECTOR4D intersectionNode, int v1, int v2, int v3, TetrahedronFigure & above, TetrahedronFig5V & below, bool top, int indexTetraBuffer, int node)
//{
//	bool nodeExist;
//	nodeExist = FindNode(intersectionA);
//	// No lo encontro
//	if (!nodeExist)
//		AddNewPointsOfControl(intersectionA, above, below, node, top, v1, indexTetraBuffer);
//	else
//		SetUpSavedPointsOfControl(intersectionA, above, below, node, top);
//
//	nodeExist = FindNode(intersectionB);
//	// No lo encontro
//	if (!nodeExist)
//		AddNewPointsOfControl(intersectionB, above, below, node + 1, top, v2, indexTetraBuffer);
//	else
//		SetUpSavedPointsOfControl(intersectionB, above, below, node + 1, top);
//
//	nodeExist = FindNode(intersectionNode);
//	// No lo encontro
//	if (!nodeExist)
//		AddNewPointsOfControl(intersectionNode, above, below, node + 2, top, v3, indexTetraBuffer);
//	else
//		SetUpSavedPointsOfControl(intersectionNode, above, below, node + 2, top);
//}

void CVMesh::AnalizeCutY(VECTOR4D intersectionA, VECTOR4D intersectionNodeA, VECTOR4D intersectionNodeB, int v1, int v2, int v3, TetrahedronFigure& above, TetrahedronFigure& below, bool top, int indexTetraBuffer)
{
	bool nodeExist;
	nodeExist = FindNode(intersectionA);
	// No lo encontro
	if (!nodeExist)
		AddNewPointsOfControl(intersectionA, above, below, 31, top, v1, indexTetraBuffer);
	else
		SetUpSavedPointsOfControl(intersectionA, above.v4, below.v1, top);


	nodeExist = FindNode(intersectionNodeA);
	// No lo encontro
	if (!nodeExist)
		AddNewPointsOfControl(intersectionNodeA, above, below, 32, top, v2, indexTetraBuffer);
	else
		SetUpSavedPointsOfControl(intersectionNodeA, above.v2, below.v2, top);

	nodeExist = FindNode(intersectionNodeB);
	// No lo encontro
	if (!nodeExist)
		AddNewPointsOfControl(intersectionNodeB, above, below, 33, top, v3, indexTetraBuffer);
	else
		SetUpSavedPointsOfControl(intersectionNodeB, above.v3, below.v3, top);
}

void CVMesh::DisplaceTopVertex(bool top, const long long int v1)
{
	if (top == false)
	{
		auto found = m_VertexAlreadyMoved.find(v1);
		if (found == m_VertexAlreadyMoved.end())
		{
			m_Vertices[v1].Position.z -= displacement;
			m_VertexAlreadyMoved._Insert_or_assign(v1, true);
		}
	}
}

void CVMesh::DisplaceBottomVertexes(bool top, long long int v1, long long int v2, long long int v3)
{
	if (top)
	{
		auto found = m_VertexAlreadyMoved.find(v1);
		if (found == m_VertexAlreadyMoved.end())
		{
			m_Vertices[v1].Position.z -= displacement;
			m_VertexAlreadyMoved._Insert_or_assign(v1, true);
		}
		found = m_VertexAlreadyMoved.find(v2);
		if (found == m_VertexAlreadyMoved.end())
		{
			m_Vertices[v2].Position.z -= displacement;
			m_VertexAlreadyMoved._Insert_or_assign(v2, true);
		}
		found = m_VertexAlreadyMoved.find(v3);
		if (found == m_VertexAlreadyMoved.end())
		{
			m_Vertices[v3].Position.z -= displacement;
			m_VertexAlreadyMoved._Insert_or_assign(v3, true);
		}
	}
}

void CVMesh::AddVertexToBuffer(VECTOR4D intersectionPoint, bool top)
{
	totalNodes++;
	m_Vertices.resize(totalNodes + 1);
	m_Vertices[totalNodes].Position.x = intersectionPoint.x;
	m_Vertices[totalNodes].Position.y = intersectionPoint.y;
	m_Vertices[totalNodes].Position.z = intersectionPoint.z - ((top) ? 0 : displacement);
	m_Vertices[totalNodes].Position.w = 1;
	m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
}

void CVMesh::AddIndexToBuffer(long long int v1, long long int v2, long long int v3, long long int v4)
{
	int position = m_TetrahedronFigures.size();
	TetrahedronFigure auxTetraFig;
	totalTetrahedron += 1;
	m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));
	auxTetraFig.v1 = v1;
	auxTetraFig.v2 = v2;
	auxTetraFig.v3 = v3;
	auxTetraFig.v4 = v4;

	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 4] = auxTetraFig.v1;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 3] = auxTetraFig.v2;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 2] = auxTetraFig.v3;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 1] = auxTetraFig.v4;
	position++;
	m_TetrahedronFigures._Insert_or_assign(position, auxTetraFig);

}

void CVMesh::SetUpSavedPointsOfControl(VECTOR4D intersectionPoint, long long int& above, long long int& below, bool top)
{
	if (top)
	{
		above = m_NewNodesPointOfControl[intersectionPoint].top;		// ARRIBA
		below = m_NewNodesPointOfControl[intersectionPoint].bottom;	// ABAJO
	}
	else
	{
		below = m_NewNodesPointOfControl[intersectionPoint].top;		// ARRIBA
		above = m_NewNodesPointOfControl[intersectionPoint].bottom;	// ABAJO
	}
}

void CVMesh::BuildTetrahedrons()
{
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
		TexCoord.y = m_Vertices[j].Position.y;
		TexCoord.z = m_Vertices[j].Position.z;
		TexCoord = Normalize(TexCoord);
		TexCoord.x = TexCoord.x * 0.5 + 0.5;
		TexCoord.y = TexCoord.y * 0.5 + 0.5;

		m_Vertices[j].TexCoord = TexCoord;
	}
	BuildTangentSpaceFromTexCoordsIndexed(true);
}