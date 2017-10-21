///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/wrui.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
typedef Wrui* (*Get_wrui)(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Application {
	WUFont* font;
	Wrui* wrui;
} Application;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void paint_event(void* user_data) {
	Application* app = (Application*)user_data;

	WUPainter* painter = app->wrui->painter_get();
	
	WUPos pos = { 10.0f, 10.0f }; 

	//WURect rect = { 10.0f, 10.0f, 200.0f, 200.0f }; 
	WUColor color = { 1.0f, 1.0f, 0.0f, 1.0f };

	//painter->draw_rect(painter, rect, color);
	//painter->draw_rect(painter, rect, color);

	const char* text = "test text";

	painter->draw_text(painter, pos, color, text, (int)strlen(text), 0); 

	printf("update\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	void* handle = dlopen("t2-output/macosx-clang-debug-default/libwrui_dimgui.dylib", RTLD_LAZY);
    
    if (!handle) {
        printf("Cannot open library: %s\n", dlerror());
        return 1;
    }

    Get_wrui get_wrui = (Get_wrui) dlsym(handle, "wrui_get");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        printf("Cannot load symbol: %s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    Wrui* wrui = get_wrui();

    Application* user_app = malloc(sizeof(Application));
    user_app->wrui = wrui;

    WUApplication* app = wrui->application_create();
    struct WUWindow* window = wrui->window_create(0);

    wrui->window_funcs->set_paint_event(window, user_app, paint_event); 

	return wrui->application_funcs->run(app);
}
