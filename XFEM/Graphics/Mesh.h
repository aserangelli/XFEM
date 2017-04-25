#pragma once
#include "Matrix4D.h"
#include <vector>
#include <map>
#include "DXBasicPainter.h"
#include "../Cut/VolumeMeshGenericID.h"

using namespace std;

struct centroid
{
	int id;
	int code;
	VECTOR4D position;
	VECTOR4D normal;
	VECTOR4D max;
	VECTOR4D min;
};



class CMesh
{
public:

	struct INTERSECTIONINFO
	{
		int Face;
		VECTOR4D LocalPosition;
	};
	vector<CDXBasicPainter::VERTEX> m_Vertices;
	vector<unsigned long> m_Indices;
	MATRIX4D m_World; //
	vector<centroid> m_Centroides;

	char m_cName[128];
public:
	CMesh();
	bool RayCast(VECTOR4D& RayOrigin,
		VECTOR4D & RayDir,
		multimap<float, INTERSECTIONINFO>& Faces //Lista de caras que se intersectan
		);
	bool RayCast(VECTOR4D& RayOrigin,
		VECTOR4D& RayDir,
		multimap<float, unsigned long>& Vertices,float radius);
	void VertexShade(CDXBasicPainter::VERTEX(*pVS)(CDXBasicPainter::VERTEX V));
	void LoadSuzanne();
	void BuildTangentSpaceFromTexCoordsIndexed(void);
	void BuildTangentSpaceFromTexCoordsIndexed(bool bGenerateNormal);
	void Optimize();
	void GenerarCentroides();
	vector<centroid>& getCentroides() { return m_Centroides; }
	void ApplyTransformation(MATRIX4D & m);
	void ApplyRotation(float theta);

	~CMesh();
};

