#include "native-state-webos.h"

NativeStateWebos::NativeStateWebos()
{
}

NativeStateWebos::~NativeStateWebos()
{
}

bool
NativeStateWebos::init_display()
{
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
