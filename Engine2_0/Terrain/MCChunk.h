#pragma once

static const unsigned int chunkSizeX = 32;
static const unsigned int chunkSizeY = 32;
static const unsigned int chunkSizeZ = 32;

//Marching cube chunk
struct MCChunk
{
	//This equates to:		//bytes. div by 1024 to get  //kilobytes //div by 1024 again to get //megabytes.   This is how many megabytes a single chunk is.
	//(4 bytes * 32*64*32) == 33554432 / 1024 ==           32768 / 1024 ==						  32
	//unsigned int voxels[32*64*32];




	//This equates to: (2 bytes * 32*32*32) = 65536 bytes / 1024 == 64 kilobytes == like 0.03 of a megabyte or whatever. Per chunk.
	unsigned short voxels[chunkSizeX * chunkSizeY * chunkSizeZ];
	//unsigned short leaves 255 different chunk types and 0-255 density value
};

struct TerrainData
{
	//Mesh...handle? Renderable? Probably a renderable handle. 
	//That means we need to have a MeshHandler pointer inside our terrain manager. 
	// Maybe do MeshHandler->CreateMesh(triangleArray);			<<<< This means that we can probably own a physics system ptr 
	//																inside meshHandler that also creates bounding volume at the same time?		
	
	
	//Collidable for culling etc.
	//Biome?
	//Weather/wind/vegetation density texture?
};