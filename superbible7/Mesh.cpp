#include "Mesh.h"

AssimpScene::~AssimpScene()
{
	for (auto &mesh : m_meshes)
	{
		glDeleteBuffers(1, &mesh.handleToIndexBuffer);
		glDeleteBuffers(1, &mesh.handleToNormalBuffer);
		glDeleteBuffers(1, &mesh.handleToVertexBuffer);
		glDeleteVertexArrays(1, &mesh.handleToVertexArray);
	}
}

void AssimpScene::load(const char* filePath)
{
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	aiScene const* scene = importer.ReadFile(filePath, aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	
	if (scene == nullptr) return;

	for ( unsigned m=0; m < scene->mNumMeshes; ++m )
	{
		aiMesh *aimesh = scene->mMeshes[ m ];

		std::vector< unsigned > indexData; indexData.reserve( aimesh->mNumFaces * 3 );

		for ( unsigned f=0; f<aimesh->mNumFaces; ++f )
		{
			aiFace aiface = aimesh->mFaces[ f ];
			assert( aiface.mNumIndices == 3 );
			indexData.push_back( aiface.mIndices[ 0 ] );
			indexData.push_back( aiface.mIndices[ 1 ] );
			indexData.push_back( aiface.mIndices[ 2 ] );
		}

		Mesh mesh;
		mesh.indexCount = aimesh->mNumFaces * 3;
		mesh.vertexCount = aimesh->mNumVertices;
		mesh.faceCount = aimesh->mNumFaces;

		glGenBuffers( 1, std::addressof( mesh.handleToVertexBuffer ) );
		glBindBuffer( GL_ARRAY_BUFFER, mesh.handleToVertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( aiVector3D ) * aimesh->mNumVertices, aimesh->mVertices, GL_STATIC_DRAW );

		glGenBuffers( 1, std::addressof( mesh.handleToNormalBuffer ) );
		glBindBuffer( GL_ARRAY_BUFFER, mesh.handleToNormalBuffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( aiVector3D ) * aimesh->mNumVertices, aimesh->mNormals, GL_STATIC_DRAW );

		glGenBuffers( 1, std::addressof( mesh.handleToIndexBuffer ) );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.handleToIndexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * indexData.size(), reinterpret_cast< GLvoid const* >( indexData.data() ), GL_STATIC_DRAW );

		glGenVertexArrays( 1, std::addressof( mesh.handleToVertexArray ) );
		glBindVertexArray( mesh.handleToVertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, mesh.handleToVertexBuffer );
		glVertexAttribPointer( 0, 3, GL_FLOAT, false, sizeof( aiVector3D ), nullptr );
		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, mesh.handleToNormalBuffer );
		glVertexAttribPointer( 1, 3, GL_FLOAT, false, sizeof( aiVector3D ), nullptr );
		glEnableVertexAttribArray( 1 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.handleToIndexBuffer );

		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		glDisableVertexAttribArray( 0 );
		glDisableVertexAttribArray( 1 );

		m_meshes.push_back( mesh );
	}
}

void AssimpScene::draw()
{
	for (auto const& mesh : m_meshes)
	{
		glBindVertexArray(mesh.handleToVertexArray);
		glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
	}
}