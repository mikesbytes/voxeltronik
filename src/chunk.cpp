#include "chunk.h"
#include "world.h"

#include <iostream>
#include <queue>

namespace vtk {

Chunk::Chunk(World& world) :
	mLinkedWorld(world),
	mLoaded(false),
	mQueuedForMeshRebuild(false)
{
    mPos = glm::ivec3(0,0,0);

    //fill voxels with 0
    for (unsigned i = 0; i < mData.size(); ++i) {
	    mData[i].store(0);
    }
    for (unsigned i = 0; i < mLighting.size(); ++i) {
	    mLighting[i].store(0xFFFF);
    }

    mLoaded.store(true);
}

bool Chunk::isLoaded() {
	return mLoaded.load();
}

unsigned Chunk::breakVoxel(const glm::ivec3& pos) {
	unsigned voxelType = getVoxelType(pos);
	if (voxelType == 0) return voxelType; //return early if nothing broken

	setVoxelType(pos, 0);

	//update heightmap
	mLinkedWorld.getHeightMap(glm::ivec2(mPos.x, mPos.z))
		->unblockHeight(glm::ivec3(pos.x, pos.y + mPos.y * 16, pos.z));

	removeLight(pos);
	mLinkedWorld.queueChunkUpdate(mPos);

	//update the neightboring chunks if voxel lies along border
	glm::ivec3 neighborPos(0,0,0);

	if (pos.x == 0) neighborPos.x--;
	else if (pos.x == 15) neighborPos.x++;

	if (pos.y == 0) neighborPos.y--;
	else if (pos.y == 15) neighborPos.y++;

	if (pos.z == 0) neighborPos.z--;
	else if (pos.z == 15) neighborPos.z++;

	if (neighborPos.x != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x + neighborPos.x,
		                                         mPos.y,
		                                         mPos.z));
	if (neighborPos.y != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y + neighborPos.y,
		                                         mPos.z));
	if (neighborPos.z != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y,
		                                         mPos.z + neighborPos.z));

	return voxelType;
}

bool Chunk::placeVoxel(const glm::ivec3& pos, const unsigned& type) {
	unsigned curType = getVoxelType(pos);
	if (curType != 0) // return false if the voxel is NOT air
		return false;
	setVoxelType(pos, type);

	//update heightmap
	mLinkedWorld.getHeightMap(glm::ivec2(mPos.x, mPos.z))
		->blockHeight(glm::ivec3(pos.x, pos.y + mPos.y * 16, pos.z));

	unsigned short emission = mLinkedWorld.voxelInfo.getEmission(type);
	
	removeLight(pos);
	if (emission != 0) {
		addLight(pos, emission);
	}
	mLinkedWorld.queueChunkUpdate(mPos, true);


	//update the neightboring chunks if voxel lies along border

	// TODO: make this a function in voxelutils(voxelmath?)
	glm::ivec3 neighborPos(0,0,0);

	if (pos.x == 0) neighborPos.x--;
	else if (pos.x == 15) neighborPos.x++;

	if (pos.y == 0) neighborPos.y--;
	else if (pos.y == 15) neighborPos.y++;

	if (pos.z == 0) neighborPos.z--;
	else if (pos.z == 15) neighborPos.z++;
	
	if (neighborPos.x != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x + neighborPos.x,
		                                         mPos.y,
		                                         mPos.z));
	if (neighborPos.y != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y + neighborPos.y,
		                                         mPos.z));
	if (neighborPos.z != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y,
		                                         mPos.z + neighborPos.z));

	return true;
}



bool Chunk::isVoxelSolid(const int& x, const int& y, const int& z) {
    if (x < 0 || x > 15 ||
        y < 0 || y > 15 ||
        z < 0 || z > 15 ) 
	{ //position is outside of the chunk
		return mLinkedWorld.isVoxelSolid(mPos.x * 16 + x,
                                         mPos.y * 16 + y,
                                         mPos.z * 16 + z);
    }

    return !mLinkedWorld.voxelInfo.isTransparent(getVoxelType((unsigned)x, (unsigned)y, (unsigned)z));
}

bool Chunk::isVoxelSolid(const glm::ivec3& pos) {
	return isVoxelSolid(pos.x, pos.y, pos.z);
}

void Chunk::setVoxelType(const glm::ivec3 &pos, const unsigned& type) {
	setVoxelType(pos.x, pos.y, pos.z, type);
}

void Chunk::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update) {
    auto index = x + 16 * (y + 16 * z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (set voxel): Out of range, doing nothing\n";
        return;
    }
    mData[index].store(type, std::memory_order_release);

    if (!update) return;
    mLinkedWorld.queueChunkUpdate(mPos);
}

unsigned Chunk::getVoxelType(const glm::ivec3& pos) {
    auto index = pos.x + 16 * (pos.y + 16 * pos.z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (get voxel): Out of range, returning type 0\n";
        return 0;
    }
    return mData[index].load(std::memory_order_consume);
}

unsigned Chunk::getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z) {
	return getVoxelType(glm::ivec3(x,y,z));
}

void Chunk::rebuildLighting() {
	std::queue<LightIndexTup> sunBFSQueue;
	unsigned short mask2 = 0x000F;
	//FIRST PASS, block out all solid blocks
	for (short i = 0; i < 4096; ++i) {
		glm::ivec3 pos(i % 16, (i / 16) % 16,(i / 256)); // get current coord
		auto lightVal = mLinkedWorld.voxelInfo.getEmission(getVoxelType(pos));

		//set sunlighting if at the top of chunk and above the ground
		if(pos.y == 15 &&
		   mLinkedWorld.getHeightMap(glm::ivec2(mPos.x, mPos.z))
		   ->getHeight(glm::ivec2(pos.x, pos.z)) <
		   pos.y + mPos.y * 16)
		{
			lightVal = lightVal | 0xF; //max out the sun lighting
		}
		mLighting[i].store(lightVal, std::memory_order_release);

		if ((lightVal & 0xF) != 0) {
			sunBFSQueue.push(std::make_tuple(i,mask2,this));
		}

	}

	//iterate through sunlight
	while (!sunBFSQueue.empty()) {
		short index;
		unsigned short mask;
		Chunk* chunk;
		std::tie(index, mask, chunk) = sunBFSQueue.front();
		glm::ivec3 pos( index % 16,
		               (index / 16) % 16,
		                index / 256);

		unsigned short light;
		light = chunk->getLightPacked(pos) & 0xF;

		auto propogateSun =
			[&](glm::ivec3 nPos, Chunk* nChunk)
			{
				bool straightDown = (pos.y > nPos.y && light == 0xF);
				auto checkPos = localPosToLocalPos(nChunk->getPos(), nPos);
				nChunk = mLinkedWorld.getChunk(checkPos.first);
				nPos = checkPos.second;
				if (nChunk == nullptr) {
					return;
				}
				auto newLight = nChunk->getLightPacked(nPos) & 0xF;
				if (!nChunk->isVoxelSolid(nPos)) {
					if (light >= newLight + 2) {
						if (straightDown) {
							nChunk->setLightPacked(nPos, (newLight & 0xFFF0) | 0xF);
						} else {
							nChunk->setLightPacked(nPos, ((newLight & 0xFFF0) | light) - 1);
						}
						if (light - 1 > 1) {
							sunBFSQueue.push(std::make_tuple((short)(nPos.x + 16 * (nPos.y + 16 * nPos.z)), mask, nChunk));
						}
					}
				}
			};
		
		//visit neighbors
		propogateSun(glm::ivec3(pos.x-1, pos.y, pos.z), this);
		propogateSun(glm::ivec3(pos.x+1, pos.y, pos.z), this);
		propogateSun(glm::ivec3(pos.x, pos.y-1, pos.z), this);
		propogateSun(glm::ivec3(pos.x, pos.y+1, pos.z), this);
		propogateSun(glm::ivec3(pos.x, pos.y, pos.z-1), this);
		propogateSun(glm::ivec3(pos.x, pos.y, pos.z+1), this);

		sunBFSQueue.pop();
	}
}

void Chunk::addLight(const glm::ivec3 &pos, const unsigned short &light) {
	setLightPacked(pos, light);
	//check channels
	if (light >> 12 > 0) {
		mLightBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0xF000, this));
	}
	if (((light >> 8) & 0xF) > 0) {
		mLightBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x0F00, this));
	}
	if (((light >> 4) & 0xF) > 0) {
		mLightBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x00F0, this));
	}
	if ((light & 0xF) > 0) {
		mLightBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x000F, this));
	}
	propogateLight();
}

void Chunk::removeLight(const glm::ivec3 &pos) {
	//propogate all 4 channels
	/*mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0xF000, this));
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x0F00, this));
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x00F0, this));
	*/
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x000F, this)); //sunlight
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0xF000, this)); //R
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x0F00, this)); //G
	mDarkBFSQueue.push(std::make_tuple(vecToIndex(pos), (unsigned short)0x00F0, this)); //B
	propogateLight();
}

glm::ivec3 Chunk::getWorldCoords(const int& x, const int& y, const int& z) {
    return glm::ivec3(mPos.x * 16 + x,
                      mPos.y * 16 + y,
                      mPos.z * 16 + z);
}

unsigned Chunk::getLightLevel(const glm::ivec3 &pos) {
	if (isVoxelSolid(pos.x, pos.y, pos.z)) {
		return 0;
	}
	return 15;
}

unsigned short Chunk::getLightPacked(const glm::ivec3 &pos) {
	//checking if pos is inside current chunk
	auto lPos = localPosToLocalPos(mPos, pos); //returns a chunk/local pos pair

	if (lPos.first != mPos) { //if it's not THIS chunk
		auto chunk = mLinkedWorld.getChunk(lPos.first);
		if (chunk)
			return chunk->fastGetLightPacked(lPos.second);
		else
			return 0xFFFF;
	}
	
	int index = pos.x + 16 * (pos.y + 16 * pos.z);

	return mLighting[index].load(std::memory_order_consume);
}

void Chunk::setLightPacked(const glm::ivec3& pos, const unsigned short& light) {
	//checking if pos is inside current chunk
	auto lPos = localPosToLocalPos(mPos, pos); //returns a chunk/local pos pair

	if (lPos.first != mPos) { //if it's not THIS chunk
		auto chunk = mLinkedWorld.getChunk(lPos.first);
		if (chunk)
			return chunk->setLightPacked(lPos.second, light);
		else
			return;
	}
	
	int index = pos.x + 16 * (pos.y + 16 * pos.z);

	mLighting[index].store(light, std::memory_order_release);
}

HeightMap* Chunk::getHeightMap() {
	return mLinkedWorld.getHeightMap(glm::ivec2(mPos.x,mPos.z));
}

void Chunk::setPos(const glm::ivec3& pos) {
	mPos = pos;
}


glm::ivec3 Chunk::getPos() {
	return mPos;
}

World& Chunk::getWorld() {
	return mLinkedWorld;
}

unsigned short Chunk::fastGetLightPacked(const glm::ivec3 &pos) {
	int index = vecToIndex(pos);
	//std::cout << mPos.x << "," << mPos.y << "," << mPos.z << std::endl;
	return mLighting[index].load(std::memory_order_acquire);
}
void Chunk::setQueuedForMeshRebuild(const bool& rebuild) {
	mQueuedForMeshRebuild = rebuild;
}

bool Chunk::isQueuedForMeshRebuild() {
	return mQueuedForMeshRebuild.load();
}

void Chunk::propogateLight() {
	if (mPropogating.load(std::memory_order_acq_rel)) return;
	mPropogating.store(true, std::memory_order_acq_rel);

	propogateLightTask();
}

void Chunk::propogateLightTask() {
	while(!mDarkBFSQueue.empty()) {

		//TODO  Mask channels properly
		
		//load front of queue
		short index;
		unsigned short mask;
		Chunk* chunk;
		std::tie(index, mask, chunk) = mDarkBFSQueue.front();

		mDarkBFSQueue.pop();

		glm::ivec3 pos = indexToVec(index);
		unsigned short light = chunk->getLightPacked(pos);
		chunk->setLightPacked(pos, light & (~ mask)); //mask out correct channel

		auto propogateDark =
			[&](glm::ivec3 nPos, Chunk* nChunk)
			{
				auto checkPos = localPosToLocalPos(nChunk->getPos(), nPos);
				nChunk = mLinkedWorld.getChunk(checkPos.first);
				nPos = checkPos.second;
				//return early if chunk doesn't exist or voxel is solid
				if (nChunk == nullptr || nChunk->isVoxelSolid(nPos)) return;
				auto newLight = nChunk->fastGetLightPacked(nPos);
				if ((newLight & mask) == 0) return;
				
				//sunglight (to handle special case with straight down
				if (mask == 0x000F && ((newLight & 0xF) < (light & 0xF) || (pos.y + chunk->getPos().y * 16 > nPos.y + nChunk->getPos().y * 16 && (light & 0xF) == 0xF))) {
					mDarkBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				} else if (mask == 0x000F) {
					mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				}

				// Red
				if (mask == 0xF000 && ((newLight >> 12) < (light >> 12))) {
					mDarkBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				} else if (mask == 0xF000) {
					mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				}
				// Green
				if (mask == 0x0F00 && ((newLight & mask) >> 8 < (light & mask) >> 8)) {
					mDarkBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				} else if (mask == 0x0F00) {
					mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				}
				// Blue
				if (mask == 0x00F0 && ((newLight & mask) >> 4 < (light & mask) >> 4)) {
					mDarkBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				} else if (mask == 0x00F0) {
					mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
				}
			};

		//visit neighbors
		propogateDark(glm::ivec3(pos.x-1, pos.y, pos.z), chunk);
		propogateDark(glm::ivec3(pos.x+1, pos.y, pos.z), chunk);
		propogateDark(glm::ivec3(pos.x, pos.y-1, pos.z), chunk);
		propogateDark(glm::ivec3(pos.x, pos.y+1, pos.z), chunk);
		propogateDark(glm::ivec3(pos.x, pos.y, pos.z-1), chunk);
		propogateDark(glm::ivec3(pos.x, pos.y, pos.z+1), chunk);
		
	}

	while(!mLightBFSQueue.empty()) {

		//TODO  Mask channels properly
		
		//load front of queue
		short index;
		unsigned short mask;
		Chunk* chunk;
		std::tie(index, mask, chunk) = mLightBFSQueue.front();

		mLightBFSQueue.pop();

		glm::ivec3 pos = indexToVec(index);
		unsigned short light = chunk->getLightPacked(pos);
		//std::cout << light << std::endl;
		//std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;

		auto propogateLight =
			[&](glm::ivec3 nPos, Chunk* nChunk)
			{
				auto checkPos = localPosToLocalPos(nChunk->getPos(), nPos);
				nChunk = mLinkedWorld.getChunk(checkPos.first);
				nPos = checkPos.second;
				//return early if chunk doesn't exist or voxel is solid
				if (nChunk == nullptr || nChunk->isVoxelSolid(nPos)) return;
				auto newLight = nChunk->fastGetLightPacked(nPos);

				//red channel
				if (mask == 0xF000 && (light >> 12) > (newLight >> 12) + 1) {
					nChunk->setLightPacked(nPos, ((newLight & (~ mask)) | (((light >> 12) - 1)) << 12)); //set new light level with proper masking
					if (((light & mask) >> 12) - 1 > 1) {
						mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
					}
				}
				//green channel
				if (mask == 0x0F00 && ((light >> 8) & 0xF) > ((newLight >> 8) & 0xF) + 1) {
					nChunk->setLightPacked(nPos, ((newLight & (~ mask)) | ((((light >> 8) & 0xF) - 1)) << 8)); //set new light level with proper masking
					if (((light & mask) >> 8) - 1 > 1) {
						mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
					}
				}
				//blue channel
				if (mask == 0x00F0 && ((light >> 4) & 0xF) > ((newLight >> 4) & 0xF) + 1) {
					nChunk->setLightPacked(nPos, ((newLight & (~ mask)) | ((((light >> 4) & 0xF) - 1)) << 4)); //set new light level with proper masking
					if (((light & mask) >> 4) - 1 > 1) {
						mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
					}
				}
				
				//sunglight channel
				if (mask == 0x000F && (light & 0xF) > (newLight & 0xF) + 1) {
					//handle straight down propogation
					if (pos.y + chunk->getPos().y * 16 > nPos.y + nChunk->getPos().y * 16 && (light & 0xF) == 0xF) {
						nChunk->setLightPacked(nPos, newLight | 0xF);
					} else {
						nChunk->setLightPacked(nPos, (((newLight & (~ mask)) | (light & 0xF)) - 1));
					}
 					if ((light & mask) - 1 > 1) {
						mLightBFSQueue.push(std::make_tuple(vecToIndex(nPos), mask, nChunk));
					}
				}
			};

		//visit neighbors
		propogateLight(glm::ivec3(pos.x-1, pos.y, pos.z), chunk);
		propogateLight(glm::ivec3(pos.x+1, pos.y, pos.z), chunk);
		propogateLight(glm::ivec3(pos.x, pos.y-1, pos.z), chunk);
		propogateLight(glm::ivec3(pos.x, pos.y+1, pos.z), chunk);
		propogateLight(glm::ivec3(pos.x, pos.y, pos.z-1), chunk);
		propogateLight(glm::ivec3(pos.x, pos.y, pos.z+1), chunk);
	}

	//release the the task
	mPropogating.store(false);
	//restart it if any light updates slipped through the cracks
	if (!mDarkBFSQueue.empty() ) {//|| !mLightBFSQueue.empty()) {
		propogateLight();
	}
}

short Chunk::vecToIndex(const glm::ivec3& pos) {
	return pos.x + 16 * (pos.y + 16 * pos.z);
}

glm::ivec3 Chunk::indexToVec(const short& index) {
	return glm::ivec3(index % 16,
	                  (index / 16) % 16,
					  index / 256);

}

}
