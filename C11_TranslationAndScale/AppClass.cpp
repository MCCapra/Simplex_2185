#include "AppClass.h"
void Application::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(0.0f, 0.0f, 5.0f), vector3(0,0,0), AXIS_Y);
	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh2 = new MyMesh(); 
	m_pMesh->GenerateCone(1.0f, 3.0f, 10,C_BLUE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
	//m_pMesh2->GenerateTorus(1.0f, 0.5f, 20, 10, C_BLUE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();


	//Invader is 8x11

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();


	static float value = 0.0f; //To be used to make everything move to the right.

	matrix4 m4Model = glm::rotate(IDENTITY_M4, glm::radians(value), vector3(0, 0, 0));

	m_pMesh->Render(m4Projection, m4View, m4Model);

	//Start making the invader

	/*//Top layer
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, 4.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, 4.0f, 0.0f));
	invaderList.push_back(m4Translate);

	//Second Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f, 3.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f, 3.0f, 0.0f));
	invaderList.push_back(m4Translate);

	//Third Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, 2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list

	//Fourth Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-4.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(4.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, 1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list

	//Fifth Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-4.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(4.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list

	//Sixth Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, -1.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list

	//Seventh Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-5.0f, -2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(5.0f, -2.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-3.0f, -2.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(3.0f, -2.0f, 0.0f));
	invaderList.push_back(m4Translate);

	//Eigth Layer
	m4Translate = glm::translate(IDENTITY_M4, vector3(-2.0f, -3.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(2.0f, -3.0f, 0.0f));
	invaderList.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(-1.0f, -3.0f, 0.0f));
	invaderList.push_back(m4Translate); //Push it to our list.
	m4Translate = glm::translate(IDENTITY_M4, vector3(1.0f, -3.0f, 0.0f));
	invaderList.push_back(m4Translate);

	for (matrix4 mat : invaderList)
	{
		mat = glm::translate(mat, vector3(value, 0.0f, 0.0f));
		m_pMesh->Render(m4Projection, m4View, mat);
	}*/

	/*
	Modifiying by saving in a v4
	v4.x = 1.0f;
	m4Model[3] = v4;
	*/

	//m4Model[3][0] = 1.0f; Modifiying by editting directly


	//Scaling directly
	/*for (uint i = 0; i < 3; i++)
	{
		m4Model[i][i] = 2.0f;
	}*/

	//Using GLM

	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddGridToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();

	//invaderList.clear();
	value += 1.0f; //Increase value 
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);
	SafeDelete(m_pMesh2);
	//release GUI
	ShutdownGUI();
}