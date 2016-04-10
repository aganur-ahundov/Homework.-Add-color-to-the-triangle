// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>

//#include <fstream>
#include <cassert>


GLuint hIndexBuffer;
GLuint hVertexBuffer;
GLuint hTextureIndex;

FILE * pFile;
int nIndexes;
int nVertexs;

Shaders myShaders;


void initTexture()
{
	glGenTextures( 1, &hTextureIndex );
	glBindTexture( GL_TEXTURE_2D, hTextureIndex );

	int width, height, bpp;

	char *bufferTGA = LoadTGA("../Resources/Textures/Woman1.tga", &width, &height, &bpp);
	assert(bpp == 24 || bpp == 32);

	if (bpp == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}

	delete[] bufferTGA;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}


int Init ( ESContext *esContext )
{

	const char form[] = " %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f,%f];";
	const char formForVector3[] = "%*d. %hd, %hd, %hd";


	/*-------------------------------------------------------------------------------------*/


	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glEnable(GL_DEPTH_TEST);


	/*-------------------------------------------------------------------------------------*/


	fopen_s(&pFile ,"../Resources/Models/Woman1.nfg", "r");
	assert( pFile );


	fscanf_s(pFile, "NrVertices: %d", &nVertexs);
	fscanf_s(pFile, "%*s %d", &nIndexes);


	/*-------------------------------------------------------------------------------------*/


	Vertex * verticesData = new Vertex[ nVertexs ];
	for (int i = 0; i < nVertexs; i++)
		fscanf(pFile, form, &verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z, &verticesData[i].m_uv.x, &verticesData[i].m_uv.y);

	
	unsigned short * indexes = new unsigned short[nIndexes];
	for (int i = 0; i < nIndexes; i += 3)
		fscanf_s(pFile, formForVector3, &indexes[i], &indexes[i + 1], &indexes[i + 2]);

	
	/*-------------------------------------------------------------------------------------*/


	initTexture();


	/*-------------------------------------------------------------------------------------*/


	//buffer object
	glGenBuffers(1, &hVertexBuffer); //buffer object name generation
	glBindBuffer(GL_ARRAY_BUFFER, hVertexBuffer); //buffer object binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertexs, verticesData, GL_STATIC_DRAW); //creation and initializion of buffer onject storage
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//index buffer
	glGenBuffers( 1,  &hIndexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, hIndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * nIndexes, indexes, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	

	
	/*-------------------------------------------------------------------------------------*/

	delete[] indexes;
	delete[] verticesData;

	fclose(pFile);

	/*-------------------------------------------------------------------------------------*/


	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}


void Draw(ESContext *esContext)
{
	GLint textureUnit = 0;

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);

	//glBindBuffer( GL_ARRAY_BUFFER, vboId );

	glBindBuffer( GL_ARRAY_BUFFER, hVertexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, hIndexBuffer );

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, hTextureIndex);

	GLfloat *ptr = (GLfloat *)0;

	if (myShaders.positionAttribute != -1) //attribute passing to shader, for uniforms use glUniform1f(time, deltaT); glUniformMatrix4fv( m_pShader->matrixWVP, 1, false, (GLfloat *)&rotationMat );
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	}

	if (myShaders.uvAttrib != -1)
	{
		glEnableVertexAttribArray(myShaders.uvAttrib);
		glVertexAttribPointer(myShaders.uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr + 3);
	}

	if (myShaders.textureUniform != -1)
	{
		glUniform1i(myShaders.textureUniform, textureUnit);
	}

	glDrawElements( GL_TRIANGLES, nIndexes, GL_UNSIGNED_SHORT, 0 );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{

}

void CleanUp()
{
	glDeleteBuffers(1, &hVertexBuffer);
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

