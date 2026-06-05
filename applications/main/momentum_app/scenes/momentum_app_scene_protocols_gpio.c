#include "../g4meover_app.h"

enum VarItemListIndex {
    VarItemListIndexSpiCc1101Handle,
    VarItemListIndexSpiNrf24Handle,
    VarItemListIndexUartEspChannel,
    VarItemListIndexUartNmeaChannel,
    VarItemListIndexUartGeneralChannel,
};

#define SPI_DEFAULT  "Default 4"
#define SPI_EXTRA    "Extra 7"
#define UART_DEFAULT "Default 13,14"
#define UART_EXTRA   "Extra 15,16"

void g4meover_app_scene_protocols_gpio_var_item_list_callback(void* context, uint32_t index) {
    G4MEOVERApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void g4meover_app_scene_protocols_gpio_cc1101_handle_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    g4meover_settings.spi_cc1101_handle =
        variable_item_get_current_value_index(item) == 0 ? SpiDefault : SpiExtra;
    variable_item_set_current_value_text(
        item, g4meover_settings.spi_cc1101_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);
    app->save_settings = true;
}

static void g4meover_app_scene_protocols_gpio_nrf24_handle_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    g4meover_settings.spi_nrf24_handle =
        variable_item_get_current_value_index(item) == 0 ? SpiDefault : SpiExtra;
    variable_item_set_current_value_text(
        item, g4meover_settings.spi_nrf24_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);
    app->save_settings = true;
}

static void g4meover_app_scene_protocols_gpio_esp32_channel_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    g4meover_settings.uart_esp_channel = variable_item_get_current_value_index(item) == 0 ?
                                             FuriHalSerialIdUsart :
                                             FuriHalSerialIdLpuart;
    variable_item_set_current_value_text(
        item,
        g4meover_settings.uart_esp_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);
    app->save_settings = true;
}

static void g4meover_app_scene_protocols_gpio_nmea_channel_changed(VariableItem* item) {
    G4MEOVERApp* app = variable_item_get_context(item);
    g4meover_settings.uart_nmea_channel = variable_item_get_current_value_index(item) == 0 ?
                                              FuriHalSerialIdUsart :
                                              FuriHalSerialIdLpuart;
    variable_item_set_current_value_text(
        item,
        g4meover_settings.uart_nmea_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);
    app->save_settings = true;
}

void g4meover_app_scene_protocols_gpio_on_enter(void* context) {
    G4MEOVERApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list,
        "CC1101 SPI",
        2,
        g4meover_app_scene_protocols_gpio_cc1101_handle_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.spi_cc1101_handle);
    variable_item_set_current_value_text(
        item, g4meover_settings.spi_cc1101_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);

    item = variable_item_list_add(
        var_item_list, "NRF24 SPI", 2, g4meover_app_scene_protocols_gpio_nrf24_handle_changed, app);
    variable_item_set_current_value_index(item, g4meover_settings.spi_nrf24_handle);
    variable_item_set_current_value_text(
        item, g4meover_settings.spi_nrf24_handle == SpiDefault ? SPI_DEFAULT : SPI_EXTRA);

    item = variable_item_list_add(
        var_item_list,
        "ESP32/8266 UART",
        2,
        g4meover_app_scene_protocols_gpio_esp32_channel_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.uart_esp_channel);
    variable_item_set_current_value_text(
        item,
        g4meover_settings.uart_esp_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);

    item = variable_item_list_add(
        var_item_list,
        "NMEA GPS UART",
        2,
        g4meover_app_scene_protocols_gpio_nmea_channel_changed,
        app);
    variable_item_set_current_value_index(item, g4meover_settings.uart_nmea_channel);
    variable_item_set_current_value_text(
        item,
        g4meover_settings.uart_nmea_channel == FuriHalSerialIdUsart ? UART_DEFAULT : UART_EXTRA);

    variable_item_list_set_enter_callback(
        var_item_list, g4meover_app_scene_protocols_gpio_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, G4MEOVERAppSceneProtocolsGpio));

    view_dispatcher_switch_to_view(app->view_dispatcher, G4MEOVERAppViewVarItemList);
}

bool g4meover_app_scene_protocols_gpio_on_event(void* context, SceneManagerEvent event) {
    G4MEOVERApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(
            app->scene_manager, G4MEOVERAppSceneProtocolsGpio, event.event);
        consumed = true;
        switch(event.event) {
        default:
            break;
        }
    }

    return consumed;
}

void g4meover_app_scene_protocols_gpio_on_exit(void* context) {
    G4MEOVERApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
