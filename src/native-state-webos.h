#ifndef GLMARK2_NATIVE_STATE_WEBOS_H_
#define GLMARK2_NATIVE_STATE_WEBOS_H_

#include <wayland-client.h>
#include <wayland-egl.h>

#include "native-state.h"

class NativeStateWebos : public NativeState
{
public:
    NativeStateWebos();
    ~NativeStateWebos();

    bool init_display();
    void* display();
    bool create_window(WindowProperties const& properties);
    void* window(WindowProperties& properties);
    void visible(bool v);
    bool should_quit();
    void flip();

private:
    static void quit_handler(int signum);

    static const struct wl_registry_listener registry_listener_;

    static void
    registry_handle_global(void *data, struct wl_registry *registry,
                           uint32_t id, const char *interface,
                           uint32_t version);
    static void
    registry_handle_global_remove(void *data, struct wl_registry *registry,
                                  uint32_t name);

    struct my_display {
        wl_display *display;
        wl_registry *registry;
        wl_compositor *compositor;
    } *display_;

    struct my_window {
        WindowProperties properties;
        struct wl_surface *surface;
        struct wl_egl_window *native;
    } *window_;

    static volatile bool should_quit_;
};

#endif /* GLMARK2_NATIVE_STATE_WEBOS_H_ */
