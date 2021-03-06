#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//Make a circle, connect all vertexes on end to the point with tris
	float angle = (2 * PI) / a_nSubdivisions;

	vector3 bottomMidPoint(0, 0, 0); //Center of the circle
	vector3 topConePoint(0, a_fHeight, 0); //Point of the cone
	vector3 basePoint1(a_fRadius, 0, 0); //First point on the circle
	matrix4 rotMat = glm::rotate(IDENTITY_M4, angle, vector3(0,1,0)); //Rotation Matrix 

	vector3 basePoint2 = vector3(rotMat * vector4(basePoint1, 0)); //Apply rotation to make a second point for the circle
	
	AddTri(basePoint2, basePoint1, bottomMidPoint); //Create first subsection of the cone
	AddTri(basePoint1, basePoint2, topConePoint); 

	for (uint i = 0; i < a_nSubdivisions; i++) //Loop through and make rest of cone
	{
		basePoint1 = vector3(rotMat * vector4(basePoint1, 0));
		basePoint2 = vector3(rotMat * vector4(basePoint2, 0));
		AddTri(basePoint1, basePoint2, topConePoint);
		AddTri(basePoint2, basePoint1, bottomMidPoint);
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//Make two circles, connect with quads
	// -------------------------------

	//Angle of rotation
	float angle = (2 * PI) / a_nSubdivisions;
	//Rotation matrix
	matrix4 rotMat = glm::rotate(IDENTITY_M4, angle, vector3(0, 1, 0));

	//First Circle
	vector3 bottomCircleCenter(0, 0, 0);
	vector3 bottomCirclePoint1(a_fRadius, 0, 0);
	vector3 bottomCirclePoint2 = vector3(rotMat * vector4(bottomCirclePoint1, 0));

	//Second Circle
	vector3 topCircleCenter(0, a_fHeight, 0);
	vector3 topCirclePoint1(a_fRadius, a_fHeight, 0);
	vector3 topCirclePoint2 = vector3(rotMat * vector4(topCirclePoint1, 0));

	AddTri(bottomCirclePoint1, bottomCirclePoint2, bottomCircleCenter);
	AddTri(topCirclePoint1, topCirclePoint2, topCircleCenter);
	AddQuad(bottomCirclePoint1, bottomCirclePoint2, topCirclePoint1, topCirclePoint2);

	for (uint i = 0; i < a_nSubdivisions; i++)
	{
		//First Circle
		bottomCirclePoint1 = vector3(rotMat * vector4(bottomCirclePoint1, 0));
		bottomCirclePoint2 = vector3(rotMat * vector4(bottomCirclePoint2, 0));

		//Second Circle
		topCirclePoint1 = vector3(rotMat * vector4(topCirclePoint1, 0));
		topCirclePoint2 = vector3(rotMat * vector4(topCirclePoint2, 0));

		AddTri(bottomCircleCenter, bottomCirclePoint2, bottomCirclePoint1);
		AddTri(topCirclePoint1, topCirclePoint2, topCircleCenter);
		AddQuad(bottomCirclePoint1, bottomCirclePoint2, topCirclePoint1, topCirclePoint2);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code'
	//make inner circle, outer circle. Both top and bottom.
	//Connect with quads.

	float angle = (2 * PI) / a_nSubdivisions;

	//Rotation matrix
	matrix4 rotMat = glm::rotate(IDENTITY_M4, angle, vector3(0, 1, 0));

	//Center points, DO NOT USE FOR QUADS/TRIS
	vector3 bottomCenterPoint(0, 0, 0);
	vector3 topCenterPoint(0, a_fHeight, 0);

	//Outer Radius and Inner Radius for bottom circle
	vector3 bIRadiusPoint1(a_fInnerRadius, 0, 0);
	vector3 bIRadiusPoint2 = vector3(rotMat * vector4(bIRadiusPoint1, 0));

	vector3 bORadiusPoint1(a_fOuterRadius, 0, 0);
	vector3 bORadiusPoint2 = vector3(rotMat * vector4(bORadiusPoint1, 0));

	//Outer Radius and Inner Radius for top circle
	vector3 tIRadiusPoint1(a_fInnerRadius, a_fHeight, 0);
	vector3 tIRadiusPoint2 = vector3(rotMat * vector4(tIRadiusPoint1, 0));

	vector3 tORadiusPoint1(a_fOuterRadius, a_fHeight, 0);
	vector3 tORadiusPoint2 = vector3(rotMat * vector4(tORadiusPoint1, 0));

	//create our quads
	AddQuad(bIRadiusPoint1, bIRadiusPoint2, bORadiusPoint1, bORadiusPoint2);
	AddQuad(tORadiusPoint1, tORadiusPoint2, tIRadiusPoint1, tIRadiusPoint2);
	AddQuad(bIRadiusPoint2, bIRadiusPoint1, tIRadiusPoint2, tIRadiusPoint1);
	AddQuad(bORadiusPoint1, bORadiusPoint2, tORadiusPoint1, tORadiusPoint2);

	for (uint i = 0; i < a_nSubdivisions; i++)
	{
		//Outer Radius and Inner Radius for bottom circle
		bIRadiusPoint1 = vector3(rotMat * vector4(bIRadiusPoint1, 0));
		bIRadiusPoint2 = vector3(rotMat * vector4(bIRadiusPoint1, 0));

		bORadiusPoint1 = vector3(rotMat * vector4(bORadiusPoint1, 0));
		bORadiusPoint2 = vector3(rotMat * vector4(bORadiusPoint1, 0));

		//Outer Radius and Inner Radius for top circle
		tIRadiusPoint1 = vector3(rotMat * vector4(tIRadiusPoint1, 0));
		tIRadiusPoint2 = vector3(rotMat * vector4(tIRadiusPoint1, 0));

		tORadiusPoint1 = vector3(rotMat * vector4(tORadiusPoint1, 0));
		tORadiusPoint2 = vector3(rotMat * vector4(tORadiusPoint1, 0));

		AddQuad(bIRadiusPoint1, bIRadiusPoint2, bORadiusPoint1, bORadiusPoint2);
		AddQuad(tORadiusPoint1, tORadiusPoint2, tIRadiusPoint1, tIRadiusPoint2);
		AddQuad(bIRadiusPoint2, bIRadiusPoint1, tIRadiusPoint2, tIRadiusPoint1);
		AddQuad(bORadiusPoint1, bORadiusPoint2, tORadiusPoint1, tORadiusPoint2);
	}


	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 10)
		a_nSubdivisions = 10;

	Release();
	Init();

	// Replace this with your code
	float hzStep = (2 * PI) / a_nSubdivisions; //Angle of horizontal rotation
	float vrtStep = PI / a_nSubdivisions; //Angle of vertical rotation
	
	std::vector<vector3> points;

	float hzAngle, vrtAngle;
	float x, y, z, xz; //These components will help us make each vertex; xy is the projection of z over the plane xy

	//Make the sphere
	for (uint i = 0; i <= a_nSubdivisions; i++)
	{
		//This gets us the Z value of any given point on the sphere
		vrtAngle = PI / 2 - i * vrtStep; //Rotate from 180 to 0
		xz = a_fRadius * cosf(vrtAngle);
		y = a_fRadius * sinf(vrtAngle);
		
		//get the x and y values of any given point on the sphere.
		for (uint j = 0; j <= a_nSubdivisions; j++)
		{
			hzAngle = j * hzStep;

			//Get x and Y values;
			x = xz * cosf(hzAngle);
			z = xz * sinf(hzAngle);

			vector3 point(x, y, z);
			points.push_back(point);
		}
	}


	int ind1, ind2; //Index values for points to be drawn
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		ind1 = i * (a_nSubdivisions + 1);
		ind2 = ind1 + a_nSubdivisions + 1;

		for (int j = 0; j < a_nSubdivisions; j++, ind1++, ind2++)
		{
			if (i != 0)
			{
				AddTri(points[ind1], points[ind2], points[ind1 + 1]);
			}

			if (i != (a_nSubdivisions - 1))
			{
				AddTri(points[ind1 + 1], points[ind2], points[ind2 + 1]);
			}
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	points.clear();
}