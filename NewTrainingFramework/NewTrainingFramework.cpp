// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>

#include <fstream>
#include <cassert>

GLuint vboId;

GLuint hIndexBuffer;
GLuint hVertexBuffer;
FILE * pFile;
float m_time;

const int nIndexes = 1902;

Shaders myShaders;
//Matrix matrix;



int Init ( ESContext *esContext )
{
	glEnable( GL_DEPTH_TEST );

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//matrix.SetIdentity();

	//triangle data (heap)
	//Vertex verticesData[3];

	//verticesData[0].pos.x =  0.0f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z = 0.0f;
	//verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
	//verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;

	////triangle colors
	//verticesData[0].m_color.x = 0.0f;  verticesData[0].m_color.y = 1.0f;  verticesData[0].m_color.z = 0.0f;
	//verticesData[1].m_color.x = 1.0f;  verticesData[1].m_color.y = 0.0f;  verticesData[1].m_color.z = 0.0f;
	//verticesData[2].m_color.x = 0.0f;  verticesData[2].m_color.y = 0.0f;  verticesData[2].m_color.z = 1.0f;


	pFile = fopen("C:/Users/Aganurych/Desktop/Gameloft/Gameloft [ hw ]/Homework/TrainingFramework/NewTrainingFramework/Resources/Models/bus.nfg", "r");
	assert( pFile );

	int nVertexs = 0;

	fscanf_s( pFile, "NrVertices: %d", &nVertexs );
	assert( nVertexs == 683 );


	Vertex * verticesData = new Vertex[ nVertexs ];
	const char form[] = " %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f,%f];";

	for ( int i = 0; i < nVertexs; i++ )
		fscanf_s( pFile, form, &verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z, &verticesData[i].m_uv.x, &verticesData[i].m_uv.y );
		

	fscanf_s( pFile, "NrIndeces: %d", &nIndexes );
		assert( nIndexes == 1902 );

	float * indexes = new float [ nIndexes ];
	
	const char formForVector3[] = "%*d.%d,%d,%d";

	//считыват мусор
	for ( int i = 0; i < nIndexes; i+= 3 )
		fscanf_s(pFile, formForVector3, &indexes[i], &indexes[i + 1], &indexes[i + 2]);
		

	//buffer object
	//glGenBuffers(1, &vboId); //buffer object name generation
	//glBindBuffer(GL_ARRAY_BUFFER, vboId); //buffer object binding
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), &verticesData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &hVertexBuffer); //buffer object name generation
	glBindBuffer(GL_ARRAY_BUFFER, hVertexBuffer); //buffer object binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), &verticesData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//index buffer
	glGenBuffers( 1,  &hIndexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, hIndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * nIndexes, indexes, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}


void Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glUniformMatrix4fv( myShaders.m_matrixTransformKey, 1, false, ( GLfloat * ) &matrix );

	glUseProgram(myShaders.program);

	//glBindBuffer( GL_ARRAY_BUFFER, vboId );

	glBindBuffer( GL_ARRAY_BUFFER, hVertexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, hIndexBuffer );

	GLfloat *ptr = (GLfloat *)0;

	//if (myShaders.positionAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	//{
	//	glEnableVertexAttribArray(myShaders.positionAttribute);
	//	glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//}

	//if (myShaders.m_colorAttribute != -1)
	//{
	//	glEnableVertexAttribArray(myShaders.m_colorAttribute);
	//	glVertexAttribPointer( myShaders.m_colorAttribute, 3, GL_FLOAT, 
	//		GL_FALSE, sizeof( Vertex ), ptr+3 );
	//}

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	if (myShaders.positionAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer( myShaders.positionAttribute, 683, GL_UNSIGNED_SHORT, GL_FALSE, sizeof( Vertex ), 0 );
	}

	if (myShaders.m_colorAttribute != -1)
	{
		glEnableVertexAttribArray( myShaders.m_colorAttribute );
		glVertexAttribPointer( myShaders.m_colorAttribute, 3, GL_FLOAT, 
			GL_FALSE, sizeof( Vertex ), ptr+3 );
	}

	glDrawElements( GL_TRIANGLES, nIndexes, GL_UNSIGNED_SHORT, 0 );

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	//m_time += 0.0015;
	//matrix.SetRotationZ( m_time );
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Model", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

