#pragma once

#include "hmain.hpp"

class MemoryManager
{
    public:
    class Segment
    {
        friend class MemoryManager;
        protected:
        int position;
        int length;
        bool ignore = false; // whether the segment should be ignored during a memory rearrangment
        public:
        void* data=nullptr;
        int datalen=0;
        int GetPosition()
        {
            return position;
        }
        int GetLength()
        {
            return length;
        }
        Segment(Segment&)=delete;
        Segment(int position, int length)
        {
            this->position = position;
            this->length = length;
        }
    };
    struct Gap
    {
        int position;
        int length;
        Gap()=default;
        Gap(int position, int length)
        {
            this->position = position;
            this->length = length;
        }
    };
    private:

    deque<shared_ptr<Segment>> segments;
    deque<Gap> gaps;
    

    int total_elements;
    int free_elements;

    void RearrangeMemory()
    {
        if(free_elements == 0)
            return;
        gaps = {Gap(total_elements-free_elements, free_elements)};
        int position = 0;
        for(int i = 0; i < segments.size(); i++)
        {
            auto seg = segments[i];
            if(seg->ignore)
                continue;
            if(seg->GetPosition() != position)
            {
                MoveSegment(seg, position, seg->GetPosition());
                seg->position = position;
            }
            position += seg->GetLength();
        }
    }
    void FillGap(int elements, int index)
    {
        assert(index >= 0 && index < gaps.size());
        Gap& gap = gaps[index];
        assert(gap.length >= elements);
        if(gap.length > elements)
        {
            gap.position += elements;
            gap.length -= elements;
        }
        else
        {
            gaps.erase(gaps.begin()+index);
        }
    }
    void MakeGap(int position, int length)
    {
        for(int i = 0; i < gaps.size(); i++)
        {
            Gap& gap = gaps[i];
            if(gap.position+gap.length == position)
            {
                gap.length += length;
                return;
            }
            if(gap.position == position+length)
            {
                gap.position -= length;
                gap.length += length;
                return;
            }
            if(gap.position > position)
            {
                gaps.insert(gaps.begin()+i, Gap(position, length));
                return;
            }
        }
        gaps.push_back(Gap(position, length));
    }
    std::optional<int> FindGap(int elements)
    {
        Gap best_gap = gaps[gaps.size()-1];
        int best_gap_index = gaps.size()-1;

        if(best_gap.length >= elements)
        {
            return best_gap_index;
        }

        bool found_gap = false;
        
        for(int i = 0; i < gaps.size(); i++)
        {
            Gap& gap = gaps[i];
            if(gap.length == elements)
            {
                best_gap = gap;
                best_gap_index = i;
                found_gap = true;
                break;
            }
            if(gap.length > elements)
            {
                if(found_gap)
                {
                    // preferentially fill the largest gaps
                    if(best_gap.length < gap.length)
                    {
                        best_gap = gap;
                        best_gap_index = i;
                    }
                }
                else
                {
                    best_gap = gap;
                    best_gap_index = i;
                    found_gap = true;
                }
            }
        }

        if(!found_gap)
        {
            return std::nullopt;
        }
        return best_gap_index;
    }
    optional<shared_ptr<Segment>> TryAllocate(int elements)
    {
        assert(elements <= free_elements);
        assert(gaps.size() > 0);

        optional<int> gap_index = FindGap(elements);
        if(!gap_index.has_value())
            return std::nullopt;

        Gap& gap = gaps[gap_index.value()];
        auto seg = std::make_shared<Segment>(gap.position, elements);
        segments.push_back(seg);
        FillGap(elements, gap_index.value());
        return seg;
    }
    shared_ptr<Segment> Allocate(int elements)
    {
        optional<shared_ptr<Segment>> seg = TryAllocate(elements);
        if(!seg.has_value())
        {
            RearrangeMemory();
            seg = TryAllocate(elements);
            assert(seg.has_value());
        }
        free_elements -= elements;
        return seg.value();
    }
    void DeAllocate(shared_ptr<Segment> segment)
    {
        if(gaps.size() == 0)
        {
            gaps.push_back(Gap(segment->position, segment->length));
        }
        else
        {
            MakeGap(segment->GetPosition(), segment->GetLength());
        }
        int i = 0;
        for(i = 0; i < segments.size(); i++)
        {
            if(segments[i] == segment)
            {
                segments.erase(segments.begin() + i);
                break;
            }
        }
        assert(i <= segments.size(), "Segment not in segment list!");
        free_elements += segment->length;
    }
    protected:
    virtual void MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos)=0;
    virtual void WriteToSegment(shared_ptr<Segment> segment, uint8_t* data, int pos, int len)=0;
    
    public:
    int GetFreeElements()
    {
        return free_elements;
    }
    void ResizeSegment(shared_ptr<Segment> segment, int new_size)
    {
        if(new_size == segment->GetLength())
            return;
        int dif = new_size - segment->GetLength();
        if(dif < 0)
        {
            MakeGap(segment->position + new_size, -dif);
        }
        else
        {
            bool done = false;

            for(int i = 0; i < gaps.size(); i++)
            {
                Gap& gap = gaps[i];
                if(gap.length == (segment->position + segment->length) && gap.length >= dif)
                {
                    if(gap.length == dif)
                        gaps.erase(gaps.begin()+i);
                    else
                    {
                        gap.length -= dif;
                        gap.position += dif;
                    }
                    done = true;
                    break;
                }
            }
            if(!done)
            {
                MakeGap(segment->GetPosition(), segment->GetLength());
                optional<int> gap = FindGap(new_size);
                if(!gap.has_value())
                {
                    segment->ignore = true;
                    RearrangeMemory();
                    segment->ignore = false;
                    gap = FindGap(new_size);
                    assert(gap.has_value());
                }
                MoveSegment(segment, gaps[gap.value()].position, segment->position);
                segment->position = gaps[gap.value()].position;
                FillGap(new_size, gap.value());
            }
        }
        segment->length = new_size;
        free_elements -= dif;
    }
    void Empty()
    {
        gaps = {Gap(0, total_elements)};
        segments = {};
    }
    void Resize(int new_size)
    {
        if(new_size < total_elements-free_elements)
        {
            throw InvalidParameterException("New size insufficient for allocated elements!");
        }
        int dif = new_size-total_elements;
        if(dif < 0)
        {
            RearrangeMemory();
        }
        free_elements += dif;
        total_elements = new_size;
    }
    shared_ptr<Segment> AllocateSegment(int elements)
    {
        if(GetFreeElements() < elements)
            throw OutOfMemoryException("Attempted to allocate " + std::to_string(elements) + " but only " + std::to_string(GetFreeElements()) + " are free!");
        return Allocate(elements);
    }
    virtual void DeAllocateSegment(shared_ptr<Segment> segment)
    {
        DeAllocate(segment);
    }
    MemoryManager()=delete;
    MemoryManager(MemoryManager&)=delete;
    MemoryManager(int elements)
    {
        this->total_elements = elements;
        this->free_elements = elements;
        gaps = {Gap(0, elements)};
    }
};