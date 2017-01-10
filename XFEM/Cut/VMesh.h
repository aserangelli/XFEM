#pragma once
#include "..\Graphics\Mesh.h"
#include <vector>
#include <map>

using namespace std;
struct TetrahedronEdgeInfo
{
	unsigned int edgeId;
	VECTOR4D cutPosition;
};

struct TetrahedronFigure
{
	long long int v1;
	long long int v2;
	long long int v3;
	long long int v4;
	bool bCutEdge[5] = { false, false, false, false, false };
	bool bCutVertex[5] = { false, false, false, false, false };

};

struct VolumeCutInfo
{
	std::vector<TetrahedronEdgeInfo>	cutEdges;
	std::vector<unsigned int>	cutNodes;
	unsigned int				type_cut;
};

class CVMesh :
	public CMesh
{
private:
	vector<TetrahedronEdgeInfo> m_EdgeInfo;
	vector<unsigned long> m_IndicesTetrahedrosBuffer;
	map<long long int, VECTOR4D> m_VerticesTetrahedron;
	map<long long int, TetrahedronFigure> m_TetrahedronFigures;
	long long int totalNodes;
	long long int totalTetrahedron;
public:
	bool asd = true;
	CVMesh();
	~CVMesh();
	void LoadMSHFile(char * filename);
	void CutTetrahedron(CDXBasicPainter* m_pDXPainter);
	long long GetTotalEdges() { return m_EdgeInfo.size(); }
	vector<CDXBasicPainter::VERTEX>& GetVertices() { return m_Vertices; }
};

