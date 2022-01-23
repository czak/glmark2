#include "native-state-webos.h"

#include <cstring>
#include <csignal>

const struct wl_registry_listener NativeStateWebos::registry_listener_ = {
    NativeStateWebos::registry_handle_global,
    NativeStateWebos::registry_handle_global_remove
};

void
NativeStateWebos::registry_handle_global(void *data, struct wl_registry *registry,
                                         uint32_t id, const char *interface,
                                         uint32_t version)
{
    NativeStateWebos *that = static_cast<NativeStateWebos *>(data);
    if (strcmp(interface, "wl_compositor") == 0) {
        that->display_->compositor =
                static_cast<struct wl_compositor *>(
                    wl_registry_bind(registry,
                                     id, &wl_compositor_interface, std::min(version, 4U)));
    }
}

void
NativeStateWebos::registry_handle_global_remove(void * /*data*/,
                                                struct wl_registry * /*registry*/,
                                                uint32_t /*name*/)
{
}

NativeStateWebos::NativeStateWebos() : display_(0), window_(0)
{
}

NativeStateWebos::~NativeStateWebos()
{
    if (window_) {
        if (window_->native)
            wl_egl_window_destroy(window_->native);
        if (window_->surface)
            wl_surface_destroy(window_->surface);
        delete window_;
    }

    if (display_) {
        if (display_->compositor)
            wl_compositor_destroy(display_->compositor);
        if (display_->registry)
            wl_registry_destroy(display_->registry);
        if (display_->display) {
            wl_display_flush(display_->display);
            wl_display_disconnect(display_->display);
        }
        delete display_;
    }
}

bool
NativeStateWebos::init_display()
{
    struct sigaction sa;
    sa.sa_handler = &NativeStateWebos::quit_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    display_ = new struct my_display();

    if (!display_) {
        return false;
    }

    display_->display = wl_display_connect(NULL);

    if (!display_->display) {
        return false;
    }

    display_->registry = wl_display_get_registry(display_->display);

    wl_registry_add_listener(display_->registry, &registry_listener_, this);

    wl_display_roundtrip(display_->display);

    return true;
}

void*
NativeStateWebos::display()
{
    return 0;
}

bool
NativeStateWebos::create_window(WindowProperties const& properties)
{
    return true;
}

void*
NativeStateWebos::window(WindowProperties &properties)
{
    return 0;
}

void
NativeStateWebos::visible(bool /*v*/)
{
}

bool
NativeStateWebos::should_quit()
{
    return false;
}

void
NativeStateWebos::flip()
{
}

void
NativeStateWebos::quit_handler(int /*signum*/)
{
    should_quit_ = true;
}
