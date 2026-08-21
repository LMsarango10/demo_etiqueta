#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

/*Le metemos los includes del BT*/
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

/*Definimos CONFIG_NAME Y DEVICE NAME LEN*/
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

LOG_MODULE_REGISTER(etiqueta, LOG_LEVEL_INF);

/*Los datos del anuncio*/
static const struct bt_data ad[] = {
        //AD 1: Flags | BT_DATA_BYTES construye la estructura con bytes sueltos
        BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
        //AD 2: Nombre completo. BT_DATA construye desde un buffer + logitud
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};
//Declaro el advparam
static const struct bt_le_adv_param *adv_param = 
        BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONN,800,801,NULL);

int main(void)
{       
        //Inicializo el stack BLE
        int err = bt_enable(NULL);
        if(err){return -1;} else {LOG_INF("BT LISTO\n");};
        
        //arrando el adversting 
        err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad),NULL,0);
        /* Aqui crea la estructura del device
         lo que no estoy seguro es el por qué de
         esos nombres */
        if (err) { LOG_ERR("Adv fallo (err %d)", err); return 0; }
        LOG_INF("Anunciando como %s", DEVICE_NAME);
        const struct device *dev;


        /*Elegir mi pantalla y lo guardo en mi dev*/
        dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
        /*Verificamos si esta ready, es un device*/
        if(!device_is_ready(dev)){
                LOG_ERR("Pantalla no esta lista");
                return 0;};
        /*apago el blanking*/
        display_blanking_off(dev);
        //inizializo
        if(cfb_framebuffer_init(dev)){
                LOG_ERR("Pantala init fallo");
                return 0;};
        //limpio
        cfb_framebuffer_clear(dev,true);
        //escribo 
        cfb_print(dev,"Hola Familia!!",0,0);
        //finalizo
        cfb_framebuffer_finalize(dev);

        



}
