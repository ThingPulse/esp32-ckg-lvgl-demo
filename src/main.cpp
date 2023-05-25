
#include "lvgl.h"
#include "app_hal.h"

#include "demos/lv_demos.h"

#if defined( NATIVE )
    /**
     * for non arduino
     */                 
    void setup( void );
    void loop( void );

    int main( void ) {
        setup();
        while( 1 ) { loop(); };
        return( 0 );
    }
#endif // NATIVE


void setup() {
	lv_init();

	hal_setup();

  	lv_demo_widgets();
}

void loop() {
	hal_loop();
}
