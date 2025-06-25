#pragma once

#include "hmain.hpp"

class EntityManager
{
    protected:
    std::vector<bool> allocations;
    int total_entities;
    int free_entities;
    GPU* gpu;
    public:
    int GetTotalEntities()
    {
        return total_entities;
    }
    int GetFreeEntities()
    {
        return free_entities;
    }
    void WriteEntity(int index, Entity value)
    {
        assert(index < 256 && index >= 0);
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 0, sizeof(Entity), (uint8_t*)&value);
    }
    void WriteEntityType(int index, ENTITY_TYPE value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 0, 1, (uint8_t*)&value);
    }
    void WriteEntityVisible(int index, bool value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 1, 1, (uint8_t*)&value);
    }
    void WriteEntityRotation(int index, uint8_t value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 2, 1, (uint8_t*)&value);
    }
    void WriteEntityLayer(int index, uint8_t value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 3, 1, (uint8_t*)&value);
    }
    void WriteEntityPosition(int index, vec2<uint16_t> value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 4, 4, (uint8_t*)&value);
    }
    void WriteEntitySize(int index, vec2<uint16_t> value)
    {
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 8, 4, (uint8_t*)&value);
    }
    void WriteEntityData(int index, uint8_t* data, int start, int len)
    {
        assert((start+len) <= sizeof(Entity::data));
        gpu->I2CWrite(SOURCE::ENTITY_BUFFER, index, 12 + start, len, data);
    }
    void Reset()
    {
        for(int i = 0; i < allocations.size(); i++)
        {
            if(allocations[i])
            {
                WriteEntityVisible(i, false);
                allocations[i] = false;
                free_entities--;
            }
        }
        assert(free_entities == 0);
    }
    int ClaimEntity()
    {
        assert(free_entities > 0);
        int v = -1;
        for(int i = 0; i < allocations.size(); i++)
        {
            if(!allocations[i])
            {
                allocations[i] = true;
                v = i;
                break;
            }
        }
        assert(v != -1);
        free_entities--;
        return v;
    }
    int AddEntity(Entity entity)
    {
        int v = ClaimEntity();
        WriteEntity(v, entity);
        return v;
    }
    void FreeEntity(int index)
    {
        assert(allocations[index]);
        allocations[index] = false;
        WriteEntityVisible(index, false);
        free_entities++;
    }
    EntityManager(GPU* gpu, int total_entities)
    {
        this->gpu = gpu;
        this->total_entities = total_entities;
        this->free_entities = total_entities;
        allocations = std::vector<bool>(total_entities);
        for(int i = 0; i < allocations.size(); i++)
            allocations[i] = false;
    }
};