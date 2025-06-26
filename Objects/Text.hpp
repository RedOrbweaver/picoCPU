#pragma once

#include "hmain.hpp"

class Text : public Object
{
    TextManager* text_manager;
    std::string text;
    shared_ptr<TextManager::Segment> segment;
    TextManager::OnMoveSegmentFunction setfunc;
    public:
    FONT GetFont()
    {
        return (FONT)data[0];
    }
    void SetFont(FONT font)
    {
        data[0] = (uint8_t)font;
        manager->WriteEntityData(index, data, 0, 1);
    }
    TEXT_ALIGNMENT GetAlignment()
    {
        return (TEXT_ALIGNMENT)data[5];
    }
    void SetAlignment(TEXT_ALIGNMENT alignment)
    {
        data[5] = (uint8_t)alignment;
        manager->WriteEntityData(index, data+5, 5, 1);
    }
    bool IsWhite()
    {
        return data[6];
    }
    void SetWhite(bool value)
    {
        data[6] = value;
        manager->WriteEntityData(index, data+6, 6, 1);
    }
    std::string GetText()
    {
        return text;
    }
    void SetText(std::string text)
    {
        if(segment == nullptr || text.size() != this->text.size())
        {
            if(segment != nullptr)
                text_manager->DeAllocateSegment(segment);
            segment = text_manager->AllocateSegment(text.size());
            setfunc = [&](int newpos)
            {
                uint16_t pos = this->segment->GetPosition();
                memcpy(data+1, &pos, 2);
                manager->WriteEntityData(index, data+1, 1, 2);
                text_manager->WriteToSegment(segment, (uint8_t*)text.c_str(), 0, text.size());
            };
            segment->data = &setfunc;
        }
        this->text = text;
        text_manager->WriteToSegment(segment, (uint8_t*)this->text.c_str(), 0, this->text.size());
        uint16_t pos = segment->GetPosition();
        uint16_t size = segment->GetLength();
        memcpy(data+1, &pos, 2);
        memcpy(data+3, &size, 2);
        manager->WriteEntityData(index, data+1, 1, 4);
    }
    Text(EntityManager* manager, TextManager* text_manager, std::string text, FONT font, TEXT_ALIGNMENT alignment, 
        bool iswhite, bool visible = false, uint8_t layer = 0, vec2<int> position = {0, 0})
        : Object(manager, ENTITY_TYPE::TEXT, visible, layer, 0, position)
    {
        this->text_manager = text_manager;
        SetFont(font);
        SetAlignment(alignment);
        SetWhite(iswhite);
        SetText(text);
    }
    virtual ~Text()
    {
        if(segment != nullptr)
            text_manager->DeAllocateSegment(segment);
    }
};