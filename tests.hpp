#pragma once

#include "hmain.hpp"

inline void TestMemoryManager()
{
    class TestMemoryManager : public MemoryManager
    {
        public:
        uint8_t* data;
        std::map<shared_ptr<Segment>, uint8_t*> segmap;
        protected:
        virtual void MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos) override
        {
            assert(segmap.find(segment) != segmap.end());
            memcpy(data + newpos, segmap[segment], segment->GetLength());
        }
        public:
        virtual void WriteToSegment(shared_ptr<Segment> segment, uint8_t* ndata, int pos, int len) override
        {
            uint8_t* buf = new uint8_t[len];
            if(segmap.find(segment) != segmap.end())
            {
                delete[] segmap[segment];
            }
            segmap[segment] = buf;
            memcpy(data + segment->GetPosition() + pos, ndata, len);
            memcpy(buf, ndata, len);
        }
        TestMemoryManager(int elements) : MemoryManager(elements)
        {
            data = new uint8_t[elements];
        }
        ~TestMemoryManager()
        {
            for(auto it : segmap)
            {
                delete[] it.second;
            }
            delete[] data;
        }
    };
    printf("Verifying allocation and deallocation...\n");
    const int buffer_len = 1024;
    TestMemoryManager manager = TestMemoryManager(buffer_len);
    int allocated = 0;
    vector<shared_ptr<MemoryManager::Segment>> segments;
    uint8_t buf[64];
    for(int i = 0; i < ArraySize(buf); i++)
        buf[i] = i;
    while(true)
    {
        int len = (rand() % 31) + 1;
        if(manager.GetFreeElements() < len)
            break;
        auto seg = manager.AllocateSegment(len);
        manager.WriteToSegment(seg, buf, 0, len);
        segments.push_back(seg);
        allocated += len;
    }
    assert((buffer_len - allocated) == manager.GetFreeElements());
    printf("Allocated %i segments for total length of %i\n", (int)segments.size(), allocated);

    printf("Verifying memory...\n");
    
    for(int i = 0; i < segments.size(); i++)
    {
        auto seg = segments[i];
        for(int ii = 0; ii < seg->GetLength(); ii++)
        {
            assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
        }
    }

    int deallocated = 0;
    vector<shared_ptr<MemoryManager::Segment>> nsegments;
    for(int i = 0; i < segments.size(); i++)
    {
        if(rand() % 2 == 0)
        {
            manager.DeAllocateSegment(segments[i]);
            allocated -= segments[i]->GetLength();
            deallocated += segments[i]->GetLength();
        }
        else
        {
            nsegments.push_back(segments[i]);
        }
    }
    assert((buffer_len-allocated) == manager.GetFreeElements());
    printf("Deallocated %i segments for total length of %i\n", (int)(segments.size()-nsegments.size()), deallocated);

    segments = nsegments;

    printf("Verifying memory...\n");
    
    for(int i = 0; i < segments.size(); i++)
    {
        auto seg = segments[i];
        for(int ii = 0; ii < seg->GetLength(); ii++)
        {
            assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
        }
    }

    int nallocated = 0;
    int nsegs = 0;
    while(true)
    {
        int len = (rand() % 31) + 1;
        if(manager.GetFreeElements() < len)
            break;
        auto seg = manager.AllocateSegment(len);
        manager.WriteToSegment(seg, buf, 0, len);
        segments.push_back(seg);
        allocated += len;
        nallocated += len;
        nsegs++;

        for(int i = 0; i < segments.size(); i++)
        {
            auto seg = segments[i];
            for(int ii = 0; ii < seg->GetLength(); ii++)
            {
                assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
            }
        }
    }

    assert((buffer_len-allocated) == manager.GetFreeElements());
    printf("Allocated %i segments for total length of %i\n", nsegs, nallocated);

    printf("Verifying memory...\n");
    
    for(int i = 0; i < segments.size(); i++)
    {
        auto seg = segments[i];
        for(int ii = 0; ii < seg->GetLength(); ii++)
        {
            assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
        }
    }

    printf("Testing resize\n");
    
    nsegments = {};
    for(int i = 0; i < segments.size(); i++)
    {
        if(rand() % 2 == 0)
        {
            manager.DeAllocateSegment(segments[i]);
        }
        else
        {
            nsegments.push_back(segments[i]);
        }
    }

    segments = nsegments;
    int nresized = 0;
    int resizeallocated = 0;
    for(int i = 0; i < segments.size(); i++)
    {
        auto seg = segments[i];
        if(rand() % 2 == 0)
        {
            volatile int newsize = rand() % 63 + 1;
            int size = seg->GetLength();
            if(newsize > size)
            {
                if(manager.GetFreeElements() < (newsize-size))
                    continue;
            }
            manager.ResizeSegment(seg, newsize);
            manager.WriteToSegment(seg, buf, 0, newsize);
            for(int i = 0; i < segments.size(); i++)
            {
                auto seg = segments[i];
                for(int ii = 0; ii < seg->GetLength(); ii++)
                {
                    assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
                }
            }
        }
    }
    
    printf("Verifying memory...\n");
    
    for(int i = 0; i < segments.size(); i++)
    {
        auto seg = segments[i];
        for(int ii = 0; ii < seg->GetLength(); ii++)
        {
            assert(manager.data[seg->GetPosition() + ii] == buf[ii]);
        }
    }

    printf("All good!\n");

}