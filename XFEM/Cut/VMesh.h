#pragma once
#include "..\Graphics\Mesh.h"
#include <vector>
#include <map>
#include "Plane.h"
using namespace std;

struct TetrahedronEdgeInfo
{
	unsigned int edgeId;
	VECTOR4D cutPosition;
};

struct TetrahedronNodeInfo
{
	unsigned int nodeId;
	VECTOR4D cutPosition;
};

struct VolumeCutInfo
{
	std::vector<TetrahedronEdgeInfo>	cutEdges;
	std::vector<TetrahedronNodeInfo>	cutNodes;
	unsigned int				type_cut;
};


struct TetrahedronFigure
{
	long long int v1;
	long long int v2;
	long long int v3;
	long long int v4;
	long long int v5;
	long long int v6;
	// 0: nope | 1: a->b | 2: a->c | 3: a->d | 4: b->c | 5: c->d | 6: b->d
	bool bCutEdge[7] = { false, false, false, false, false, false, false };
	// 0: nope | 1: a | 2: b | 3: c | 4: d
	bool bCutNode[5] = { false, false, false, false, false };
	VECTOR4D intersectV1V2;
	VECTOR4D intersectV1V3;
	VECTOR4D intersectV2V3;
	VECTOR4D intersectV1V4;
	VECTOR4D intersectV2V4;
	VECTOR4D intersectV3V4;

	VECTOR4D directionV1V2;
	VECTOR4D directionV1V3;
	VECTOR4D directionV2V3;
	VECTOR4D directionV1V4;
	VECTOR4D directionV2V4;
	VECTOR4D directionV3V4;

	VolumeCutInfo cutInfo;


};

struct TetrahedronFig5V
{
	long long int v1;
	long long int v2;
	long long int v3;
	long long int v4;
	long long int v5;
	long long int v6;
};

struct TetrahedronFig6V
{
	long long int v1;
	long long int v2;
	long long int v3;
	long long int v4;
	long long int v5;
	long long int v6;
};

struct PointsOfControl
{
	long long int top;
	long long int bottom;
};

class CVMesh :
	public CMesh
{
private:
	//vector<TetrahedronEdgeInfo> m_EdgeInfo;
	vector<unsigned long> m_IndicesTetrahedrosBuffer;
	map<long long int, VECTOR4D> m_TetraVertexPos;
	map<long long int, TetrahedronFigure> m_TetrahedronFigures;
	//map<long long int, TetrahedronFigBaseA> m_TetrahedronFiguresDivisions;
	map<VECTOR4D, PointsOfControl> m_NewNodesPointOfControl;
	long long int totalNodes;
	long long int totalTetrahedron;
	char m_cutType[5][4] = {
		{'u','u','u','z'},
		{'u','u','y','u'},
		{'u','x','u','u'},
		{'a','u','u','u'},
		{'b','u','u','u'}
	};
	vector<int> m_IdsTetrahedronsToBeRemoved;
	bool  m_debugPaint = false;
	bool FindNode(VECTOR4D position);
	float displacement = 0.12;
	bool flag = true;
	CDXBasicPainter::VERTEX plane[3];

public:
	CVMesh();
	~CVMesh();
	void LoadMSHFile();
	void IdentifyCutType(CDXBasicPainter* m_pDXPainter);
	void SplitElementTypeA(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures);
	void SplitElementTypeB(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures);
	void SplitElementTypeX(int nIdTetrahedronCut, map<long long int, TetrahedronFigure> m_TetrahedronFigures);
	void CutTetrahedron(CDXBasicPainter* m_pDXPainter, unsigned long m_nFlagsPainter);
	//long long GetTotalEdges() { return m_EdgeInfo.size(); }
	vector<CDXBasicPainter::VERTEX>& GetVertices() { return m_Vertices; }
	void BuildTetrahedrons();
	void AddIndexToBuffer(long long int v1, long long int v2, long long int v3, long long int v4);
	void AddVertexToBuffer(VECTOR4D intersectionPoint, bool top);
	template<typename TAbove, typename TBelow>
	void AddNewPointsOfControl(VECTOR4D intersectionPoint, TAbove& above, TBelow& below, int node, bool top, int offset = 0, int indexTetraBuffer = 0);

	void AddNewPointsOfControlA(VECTOR4D intersectionPoint, TetrahedronFigure& above, TetrahedronFig6V& below, int node, bool top, int offset, int indexTetraBuffer);
	void AddNewPointsOfControlB(VECTOR4D intersectionPoint, TetrahedronFig6V& above, TetrahedronFig6V& below, int node, bool top);

	void SetUpSavedPointsOfControl(VECTOR4D intersectionPoint, long long int& above, long long int& below, bool top);

};

template<typename TAbove, typename TBelow>
inline void CVMesh::AddNewPointsOfControl(VECTOR4D intersectionPoint, TAbove & above, TBelow & below, int node, bool top, int offset, int indexTetraBuffer)
{
	// ARRIBA
	AddVertexToBuffer(intersectionPoint, top);
	node == 1 ? above.v4 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes :
		node == 2 ? above.v2 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes :
		node == 3 ? above.v3 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes :
		node == 11 ? above.v2 = totalNodes :
		node == 12 ? above.v3 = totalNodes :
		node == 13 ? above.v4 = totalNodes :
		node == 14 ? above.v5 = totalNodes :
		node == 21 ? above.v2 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes :
		node == 22 ? above.v3 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes :
		node == 23 ? above.v4 = totalNodes, m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes : totalNodes;


	// ABAJO
	AddVertexToBuffer(intersectionPoint, !top);
	node == 1 ? below.v1 = totalNodes :
		node == 2 ? below.v2 = totalNodes :
		node == 3 ? below.v3 = totalNodes :
		node == 11 ? below.v1 = totalNodes :
		node == 12 ? below.v4 = totalNodes :
		node == 13 ? below.v6 = totalNodes :
		node == 14 ? below.v5 = totalNodes :
		node == 21 ? below.v1 = totalNodes :
		node == 22 ? below.v5 = totalNodes :
		node == 23 ? below.v4 = totalNodes : totalNodes;
	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	// Se sobrescribe lo que habia anteriormente en el indice - offset por el nueva posicion
	//m_IndicesTetrahedrosBuffer[indexTetraBuffer - offset] = totalNodes;
	// Guardamos el nuevo punto de control para futuras busquedas
	// IMPORTANTE totalNode es el de arriba y totalNode+1 el de abajo
	PointsOfControl newPoints;
	if (top)
	{
		newPoints.top = totalNodes - 1;
		newPoints.bottom = totalNodes;
		m_NewNodesPointOfControl[m_Vertices[totalNodes].Position] = newPoints;
	}
	else
	{
		newPoints.top = totalNodes;
		newPoints.bottom = totalNodes-1;
		m_NewNodesPointOfControl[m_Vertices[totalNodes-1].Position] = newPoints;
	}

	//m_NewNodesPointOfControl[m_Vertices[totalNodes].Position] = newPoints;
}


