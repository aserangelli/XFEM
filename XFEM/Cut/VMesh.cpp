#include "stdafx.h"
#include "VMesh.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#define TRESHOLD 0.02
#define TRESHOLD2 0.1




CVMesh::CVMesh()
{
}


CVMesh::~CVMesh()
{
}

void CVMesh::CutTetrahedron(CDXBasicPainter* m_pDXPainter, unsigned long m_nFlagsPainter)
{
	CDXBasicPainter::VERTEX plane[3];
	unsigned long   m_lIndicesFrame[6];
	/*plane[0].Position = { m_TetraVertexPos[10].x + 3.0f, m_TetraVertexPos[10].y + 3.1f, m_TetraVertexPos[10].z + 0.5f,1 };
	plane[1].Position = { m_TetraVertexPos[1].x - 3.5f, m_TetraVertexPos[1].y + 3.8f, m_TetraVertexPos[1].z + 0.5f,1 };
	plane[2].Position = { m_TetraVertexPos[2].x, m_TetraVertexPos[2].y - 5.1f, m_TetraVertexPos[2].z + 0.5f,1 };*/

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
		m_TetrahedronFigures[i].directionV1V2 = V2 - V1;
		m_TetrahedronFigures[i].directionV1V3 = V3 - V1;
		m_TetrahedronFigures[i].directionV2V3 = V3 - V2;
		m_TetrahedronFigures[i].directionV1V4 = V4 - V1;
		m_TetrahedronFigures[i].directionV2V4 = V4 - V2;
		m_TetrahedronFigures[i].directionV3V4 = V4 - V3;

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

void CVMesh::IdentifyCutType()
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
		if (variable)
		{
			cout << "Tetrahedron No: " << i << " is type: " << (char)m_TetrahedronFigures[i].cutInfo.type_cut << endl;
		}
		if ((char)m_TetrahedronFigures[i].cutInfo.type_cut == 'a' )
		{
			if(variable)
				SplitElementTypeA(i, m_TetrahedronFigures);
		}

	}
	if (variable)
	{
		cout << "Total type A tetrahedrons: " << a << endl;
		cout << "Total type B tetrahedrons: " << b << endl;
		cout << "Total type X tetrahedrons: " << x << endl;
		cout << "Total type Y tetrahedrons: " << y << endl;
		cout << "Total type Z tetrahedrons: " << z << endl;
		cout << "Total type Undifined tetrahedrons: " << u << endl;
	}

	variable = false;

}

void CVMesh::SplitElementTypeA(int nIdTetrahedronFiguresToRemove, map<long long int, TetrahedronFigure> m_TetrahedronFigures)
{

	m_IdsTetrahedronsToBeRemoved.push_back(nIdTetrahedronFiguresToRemove);
	int vertexBase;
	vector<VECTOR4D> vIndex;
	vIndex.resize(3);

	TetrahedronFigure above;
	TetrahedronFigureDivision below = {0,0,0,0,0,0};

	int indexTetraBuffer = nIdTetrahedronFiguresToRemove * 4;

	//totalTetrahedron += 1;
	//m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));
	// Si bCutEdge[1] && bCutEdge[2] && bCutEdge[3] entonces V1 esta arriba
	if (m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[3])
	{
		// totaNodes es el total de nodos que existe en mi malla en un principio.
		// de esta forma si le quitamos totalNodes - 3  llegamos al nodo de arriba
		// El Nodo de arriba no se toca en este caso, ya que pasa de forma
		// transparente para el nuevo nodo y no lo comparte con nadie mas.
		above.v1 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v1;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer -4] = totalNodes - 3;
		// Se crea dos nuevos nodos, el de la parte de arriba y el de la parte
		// de abajo. Por esta razon se hace totalNodes++ dos veces.
		// Parte de Arriba
		///////////////////////////////////////////////////////////////////////
		//////////////////////////Nodo V2//////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		bool nodeExist = FindNode(m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2);
		// No lo encontro
		if (!nodeExist)
		{
			// ARRIBA
			totalNodes++;
			m_Vertices.resize(totalNodes + 1);
			m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.x;
			m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.y;
			m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.z;
			m_Vertices[totalNodes].Position.w = 1;
			m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
			above.v2 = totalNodes;
			// Se sobrescribe lo que habia anteriormente en el indice - 3 por el nueva posicion
			m_IndicesTetrahedrosBuffer[indexTetraBuffer - 3] = totalNodes;
			// Guardamos el nuevo punto de control para futuras busquedas
			// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
			PointsOfControl newPoints;
			newPoints.top = totalNodes;

			// ABAJO
			/**/
			totalNodes++;
			m_Vertices.resize(totalNodes + 1);
			m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.x;
			m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.y;
			m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.z;
			m_Vertices[totalNodes].Position.w = 1;
			m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
			below.v2 = totalNodes;
			// Guardamos el nuevo punto de control para futuras busquedas
			// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
			newPoints.bottom = totalNodes;
			m_NewNodesPointOfControl[m_Vertices[totalNodes].Position] = newPoints;
		}
		else {
			// ARRIBA
			above.v2 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2].top;
			// ABAJO
			below.v2 = m_NewNodesPointOfControl[m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2].bottom;
		}

		/**/


		///////////////////////////////////////////////////////////////////////
		//////////////////////////Nodo V3//////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		// ARRIBA
		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v3 = totalNodes;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 2] = totalNodes;
		// ABAJO
		/**/
		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		below.v3 = totalNodes;
		/**/
		///////////////////////////////////////////////////////////////////////
		//////////////////////////Nodo V4//////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		// ARRIBA
		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v4 = totalNodes;
		m_IndicesTetrahedrosBuffer[indexTetraBuffer - 1] = totalNodes;
		// ABAJO
		/**/
		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		below.v1 = totalNodes;
		/**/
		//below.v2 = above.v2;
		//below.v3 = above.v3;
		//below.v1 = above.v4;


		below.v4 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v2;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v3;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v4;

	}// Then V4 is a top
	else if (m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[3] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[6] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[5])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v4;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v2 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v4 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v3 = totalNodes;

		below.v1 = above.v4;
		below.v2 = above.v2;
		below.v3 = above.v3;

		below.v4 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v1;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v3;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v2;

	}// Then V2 is a top
	else if (m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[1] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[6])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v2;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v2 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V2.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v3 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v4 = totalNodes;


		below.v1 = above.v4;
		below.v2 = above.v2;
		below.v3 = above.v3;



		below.v4 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v3;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v1;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v4;

	}// Then V3 is a top
	else if (m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[2] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[4] &&
		m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].bCutEdge[5])
	{
		above.v1 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v3;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV1V3.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v2 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV2V3.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v3 = totalNodes;

		totalNodes++;
		m_Vertices.resize(totalNodes + 1);
		m_Vertices[totalNodes].Position.x = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.x;
		m_Vertices[totalNodes].Position.y = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.y;
		m_Vertices[totalNodes].Position.z = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].intersectV3V4.z;
		m_Vertices[totalNodes].Position.w = 1;
		m_TetraVertexPos._Insert_or_assign(totalNodes, m_Vertices[totalNodes].Position);
		above.v4 = totalNodes;


		below.v1 = above.v4;
		below.v2 = above.v2;
		below.v3 = above.v3;

		below.v4 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v1;
		below.v5 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v2;
		below.v6 = m_TetrahedronFigures[nIdTetrahedronFiguresToRemove].v4;

	}

	m_TetrahedronFigures._Insert_or_assign(nIdTetrahedronFiguresToRemove, above);
	int position = m_TetrahedronFigures.size();
	//position++;

	TetrahedronFigure auxTetra1;
	TetrahedronFigure auxTetra2;
	TetrahedronFigure auxTetra3;


	totalTetrahedron += 1;
	m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));
	auxTetra1.v1 = below.v1;
	auxTetra1.v2 = below.v2;
	auxTetra1.v3 = below.v3;
	auxTetra1.v4 = below.v4;

	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 4] = auxTetra1.v1;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 3] = auxTetra1.v2;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 2] = auxTetra1.v3;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 1] = auxTetra1.v4;
	position++;
	m_TetrahedronFigures._Insert_or_assign(position, auxTetra1);


	totalTetrahedron += 1;
	m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));
	auxTetra2.v1 = below.v3;
	auxTetra2.v2 = below.v4;
	auxTetra2.v3 = below.v5;
	auxTetra2.v4 = below.v6;

	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 4] = auxTetra2.v1;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 3] = auxTetra2.v2;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 2] = auxTetra2.v3;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 1] = auxTetra2.v4;
	position++;
	m_TetrahedronFigures._Insert_or_assign(position, auxTetra2);

	totalTetrahedron += 1;
	m_IndicesTetrahedrosBuffer.resize((totalTetrahedron * 4));
	auxTetra3.v1 = below.v1;
	auxTetra3.v2 = below.v3;
	auxTetra3.v3 = below.v4;
	auxTetra3.v4 = below.v6;

	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 4] = auxTetra3.v1;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 3] = auxTetra3.v2;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 2] = auxTetra3.v3;
	m_IndicesTetrahedrosBuffer[m_IndicesTetrahedrosBuffer.size() - 1] = auxTetra3.v4;
	position++;
	m_TetrahedronFigures._Insert_or_assign(position, auxTetra3);

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

bool CVMesh::FindNode(VECTOR4D position)
{
	//int nodePosition = -1;
	bool exist = false;
	exist = m_NewNodesPointOfControl.count(position);
	//if (exist)
	//{
	//	nodePosition = m_NewNodesPointOfControl[position];
	//}

	return exist;
}
