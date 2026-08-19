#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(etiqueta, LOG_LEVEL_INF);

int main(void)
{       
        /* Aqui crea la estructura del device
         lo que no estoy seguro es el por qué de
         esos nombres */
        const struct device *dev;


        /*Elegir mi pantalla y lo guardo en mi dev*/
        dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
        /*Verificamos si esta ready, es un device*/
        if(!device_is_ready(dev)){
                LOG_INF("Pantalla lista\n");
                return 0;};
        /*apago el blanking*/
        display_blanking_off(dev);
        //inizializo
        if(cfb_framebuffer_init(dev)){
                LOG_INF("Pantala init\n");
                return 0;};
        //limpio
        cfb_framebuffer_clear(dev,true);
        //escribo 
        cfb_print(dev,"Hola Mundo",0,0);
        //finalizo
        cfb_framebuffer_finalize(dev);

        



}
