template<typename T>
inline void Renderer::SafeRelease(T*& p)
{
    if (p)
    {
        p->Release();
        p = nullptr;
    }
    else 
    {
        p = nullptr;
	}
}
