#include "hmain.hpp"

void TextureManager::MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos)
{
    Texture* tex = (Texture*)segment->data;
    assert(tex->GetSize().area() == segment->GetLength());
    SendData(tex->data, newpos, segment->GetLength());
}
void TextureManager::WriteToSegment(shared_ptr<Segment> segment, uint8_t* data, int pos, int len)
{
    Texture* tex = (Texture*)segment->data;
    SendData(tex->data, pos, len);
}
void TextureManager::AddTexture(shared_ptr<Texture> texture)
{
    auto seg = AllocateSegment(texture->GetSize().area());
    seg->data = texture.get();
    texture->segment = seg;
    WriteToSegment(seg, texture->data, seg->GetPosition(), seg->GetLength());
}