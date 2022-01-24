#include "native-state-webos.h"

#include <cstdlib>
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
                                     id, &wl_compositor_interface, 1));
    } else if (strcmp(interface, "wl_shell") == 0) {
        that->display_->shell =
                static_cast<struct wl_shell *>(
                    wl_registry_bind(registry,
                                     id, &wl_shell_interface, 1));
    } else if (strcmp(interface, "wl_webos_shell") == 0) {
        that->display_->webos_shell =
                static_cast<struct wl_webos_shell *>(
                    wl_registry_bind(registry,
                                     id, &wl_webos_shell_interface, 1));
    }
}

void
NativeStateWebos::registry_handle_global_remove(void * /*data*/,
                                                struct wl_registry * /*registry*/,
                                                uint32_t /*name*/)
{
}

volatile bool NativeStateWebos::should_quit_ = false;

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
    return static_cast<void *>(display_->display);
}

bool
NativeStateWebos::create_window(WindowProperties const& properties)
{
    window_ = new struct my_window();
    window_->properties = properties;

    window_->surface = wl_compositor_create_surface(display_->compositor);

    window_->shell_surface = wl_shell_get_shell_surface(display_->shell, window_->surface);
    wl_shell_surface_set_toplevel(window_->shell_surface);

    window_->webos_shell_surface =
      wl_webos_shell_get_shell_surface(display_->webos_shell, window_->surface);
    wl_webos_shell_surface_set_property(window_->webos_shell_surface,
        "appId", (getenv("APP_ID") ? getenv("APP_ID") : "com.github.glmark2.glmark2"));
    wl_webos_shell_surface_set_property(window_->webos_shell_surface,
        "displayAffinity", (getenv("DISPLAY_ID") ? getenv("DISPLAY_ID") : "0"));

    window_->native = wl_egl_window_create(window_->surface, properties.width, properties.height);

    return true;
}

void*
NativeStateWebos::window(WindowProperties &properties)
{
    if (window_) {
        properties = window_->properties;
        return window_->native;
    }

    return 0;
}

void
NativeStateWebos::visible(bool /*v*/)
{
}

bool
NativeStateWebos::should_quit()
{
    return should_quit_;
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
