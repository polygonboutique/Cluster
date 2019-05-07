#pragma once

#include <bigg.hpp>
#include <bx/string.h>
#include <memory>

class ClusterUI;
class Config;
class Scene;
class Renderer;

class Cluster : public bigg::Application
{
    friend class ClusterUI;

public:
    Cluster();
    ~Cluster();

    int run(int argc, char* argv[]);

    // bigg callbacks

    void initialize(int _argc, char* _argv[]) override;
    void onReset() override;
    void onKey(int key, int scancode, int action, int mods) override;
    void onCursorPos(double xpos, double ypos) override;
    void onCursorEnter(int entered) override;
    void update(float dt) override;
    int shutdown() override;

    //

    void close();
    void toggleFullscreen();

    enum RenderPath : int
    {
        Forward,
        Deferred,
        Clustered
    };

    void saveFrameBuffer(bgfx::FrameBufferHandle frameBuffer, const char* path);

private:
    class BgfxCallbacks : public bgfx::CallbackI
    {
    public:
        BgfxCallbacks(Cluster& app) : app(app) {}
        virtual ~BgfxCallbacks() {}
        virtual void fatal(const char*, uint16_t, bgfx::Fatal::Enum, const char*) override;
        virtual void traceVargs(const char*, uint16_t, const char*, va_list) override;
        virtual void profilerBegin(const char*, uint32_t, const char*, uint16_t) override {}
        virtual void profilerBeginLiteral(const char*, uint32_t, const char*, uint16_t) override {}
        virtual void profilerEnd() override {}
        virtual uint32_t cacheReadSize(uint64_t) override { return 0; }
        virtual bool cacheRead(uint64_t, void*, uint32_t) override { return false; }
        virtual void cacheWrite(uint64_t, const void*, uint32_t) override {}
        virtual void captureBegin(uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, bool) override {}
        virtual void captureEnd() override {}
        virtual void captureFrame(const void*, uint32_t) override {}

        virtual void screenShot(const char*, uint32_t, uint32_t, uint32_t, const void*, uint32_t, bool yflip) override;

    private:
        Cluster& app;
    };

    float deltaTime;
    double mouseX, mouseY;

    void setRenderPath(RenderPath path);

    static bx::DefaultAllocator allocator;
    static bx::AllocatorI* iAlloc;

    // screenshots
    uint32_t saveFrame;
    void* saveData;

    BgfxCallbacks callbacks;

    // pointers to avoid circular dependencies
    // Cluster has Config member, Config needs enum definitions

    std::unique_ptr<Config> config;
    std::unique_ptr<ClusterUI> ui;
    std::unique_ptr<Scene> scene;

    std::unique_ptr<Renderer> renderer;
};
