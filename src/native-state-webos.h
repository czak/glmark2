#ifndef GLMARK2_NATIVE_STATE_WEBOS_H_
#define GLMARK2_NATIVE_STATE_WEBOS_H_

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
};

#endif /* GLMARK2_NATIVE_STATE_WEBOS_H_ */
